/*
 * Copyright 2009, 2010 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "esnpapi.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace
{

bool toBoolean(const NPVariant* variant)
{
    switch (variant->type)
    {
    case NPVariantType_Void:
    case NPVariantType_Null:
        return false;
        break;
    case NPVariantType_Bool:
        return NPVARIANT_TO_BOOLEAN(*variant);
        break;
    case NPVariantType_Int32:
        return NPVARIANT_TO_INT32(*variant) ? true : false;
        break;
    case NPVariantType_Double:
    {
        double value = NPVARIANT_TO_DOUBLE(*variant);
        return (value != 0.0 && !isnan(value)) ? true : false;
        break;
    }
    case NPVariantType_String:
        return NPVARIANT_TO_STRING(*variant).utf8length ? true : false;
        break;
    case NPVariantType_Object:
        return true;
        break;
    default:
        return false;
        break;
    }
}

char* skipSpace(const char* str)
{
    static const char* rgsp[] =
    {
        // NBSP
        "\xc2\xa0",
        // LS
        "\xe2\x80\xa8",
        // PS
        "\xe2\x80\xa9",
        // USP
        "\xe1\x9a\x80",
        "\xe1\xa0\x8e",
        "\xe2\x80\x80",
        "\xe2\x80\x81",
        "\xe2\x80\x82",
        "\xe2\x80\x83",
        "\xe2\x80\x84",
        "\xe2\x80\x85",
        "\xe2\x80\x86",
        "\xe2\x80\x87",
        "\xe2\x80\x88",
        "\xe2\x80\x89",
        "\xe2\x80\x8a",
        "\xe2\x80\xaf",
        "\xe2\x81\x9f",
        "\xe3\x80\x80",
        // BOM
        "\xef\xbb\xbf"
    };

    while (char c = *str)
    {
        if (strchr(" \t\v\f\n\r", c))
        {
            ++str;
            continue;
        }
        int i;
        for (i = 0; i < sizeof(rgsp) / sizeof(rgsp[0]); ++i)
        {
            const char* sp = rgsp[i];
            size_t len = strlen(sp);
            if (strncmp(str, sp, len) == 0)
            {
                str += len;
                break;
            }
        }
        if (sizeof(rgsp) / sizeof(rgsp[0]) <= i)
        {
            break;
        }
    }
    return const_cast<char*>(str);
}

double toNumber(const std::string value)
{
    const char* ptr = value.c_str();
    ptr = skipSpace(ptr);
    const char* uptr = ptr;
    bool negative = (*ptr == '-');
    if (negative || *ptr == '+')
    {
        ++uptr;
    }
    char* end;
    double number;
    if (strncmp(uptr, "Infinity", 8) == 0)
    {
        end = const_cast<char*>(uptr) + 8;
        number = negative ? -INFINITY : INFINITY;
    }
    else
    {
        number = strtod(ptr, &end);
        if (end == ptr)
        {
            return NAN;
        }
    }
    end = skipSpace(end);
    if (*end != 0)
    {
        return NAN;
    }
    return number;
};

double toNumber(NPP npp, const NPVariant* variant)
{
    switch (variant->type)
    {
    case NPVariantType_Void:
        return NAN;
    case NPVariantType_Null:
        return 0.0;
    case NPVariantType_Bool:
        return NPVARIANT_TO_BOOLEAN(*variant) ? 1.0 : 0.0;
    case NPVariantType_Int32:
        return NPVARIANT_TO_INT32(*variant);
    case NPVariantType_Double:
        return NPVARIANT_TO_DOUBLE(*variant);
    case NPVariantType_String:
        return toNumber(std::string(NPVARIANT_TO_STRING(*variant).utf8characters,
                                    NPVARIANT_TO_STRING(*variant).utf8length));
    case NPVariantType_Object:
        {
            NPVariant value;
            if (NPN_Invoke(npp, NPVARIANT_TO_OBJECT(*variant), NPN_GetStringIdentifier("valueOf"), 0, 0, &value))
            {
                double number = toNumber(npp, &value);
                NPN_ReleaseVariantValue(&value);
                return number;
            }
        }
        return NAN;
    default:
        return NAN;
    }
}

long long toInteger(NPP npp, const NPVariant* variant)
{
    double value = toNumber(npp, variant);
    if (isnan(value) || value == 0.0 || isinf(value))
    {
        return 0;
    }
    return static_cast<long long>(value);
}

std::string toString(NPP npp, const NPVariant* variant, char attribute = 0)
{
    char buffer[128];

    switch (variant->type)
    {
    case NPVariantType_Void:
        if (attribute & Reflect::kUndefinedIsEmpty)
        {
            return "";
        }
        return "undefined";
        break;
    case NPVariantType_Null:
        if (attribute & Reflect::kNullIsEmpty)
        {
            return "";
        }
        return "null";
        break;
    case NPVariantType_Bool:
        return NPVARIANT_TO_BOOLEAN(*variant) ? "true" : "false";
        break;
    case NPVariantType_Int32:
    {
        int32_t value = NPVARIANT_TO_INT32(*variant);
        sprintf(buffer, "%d", value);
        return buffer;
        break;
    }
    case NPVariantType_Double:
    {
        double value = NPVARIANT_TO_DOUBLE(*variant);
        if (isnan(value))
        {
            return "NaN";
        }
        if (isinf(value))
        {
            return (isinf(value) == 1) ? "Infinity" : "-Infinity";
        }
        sprintf(buffer, "%g", value);
        return buffer;
        break;
    }
    case NPVariantType_String:
        return std::string(NPVARIANT_TO_STRING(*variant).utf8characters,
                            NPVARIANT_TO_STRING(*variant).utf8length);
        break;
    case NPVariantType_Object:
    {
        NPVariant result;
        if (NPN_Invoke(npp, NPVARIANT_TO_OBJECT(*variant), NPN_GetStringIdentifier("toString"), 0, 0, &result))
        {
            if (NPVARIANT_IS_STRING(result))
            {
                return std::string(NPVARIANT_TO_STRING(*variant).utf8characters,
                                    NPVARIANT_TO_STRING(*variant).utf8length);
            }
            NPN_ReleaseVariantValue(&result);
        }
        return "";
        break;
    }
    default:
        return "undefined";
        break;
    }
}

}  // namespace

std::string getInterfaceName(NPP npp, NPObject* object)
{
    std::string className;
    NPVariant result;
    bool asConstructor = true;  // true if object can be a constructor

    VOID_TO_NPVARIANT(result);
    NPN_Invoke(npp, object, NPN_GetStringIdentifier("toString"), 0, 0, &result);
    for (;;)
    {
        if (NPVARIANT_IS_STRING(result))
        {
            className = std::string(NPVARIANT_TO_STRING(result).utf8characters,
                                    NPVARIANT_TO_STRING(result).utf8length);
        }
        NPN_ReleaseVariantValue(&result);
        if (className.compare(0, 9, "function ") == 0)
        {
            // In Chrome, a [Constructor] object is represented as a 'Function'.
            className = className.substr(9);
            size_t pos = className.find('(');
            if (pos != std::string::npos)
            {
                className = className.substr(0, pos);
                break;
            }
            return "Function";
        }
        if (className.compare(0, 8, "[object ", 8) == 0 && className[className.length() - 1] == ']')
        {
            className = className.substr(8, className.length() - 9);
            if (className != "Object")
            {
                break;
            }
        }
        // This object is likely to have a stringifier. Check the constructor name directly.
        NPVariant constructor;
        VOID_TO_NPVARIANT(constructor);
        if (asConstructor && NPN_GetProperty(npp, object, NPN_GetStringIdentifier("constructor"), &constructor))
        {
            if (NPVARIANT_IS_OBJECT(constructor) &&
                NPN_Invoke(npp, NPVARIANT_TO_OBJECT(constructor), NPN_GetStringIdentifier("toString"), 0, 0, &result))
            {
                NPN_ReleaseVariantValue(&constructor);
                asConstructor = false;
                continue;
            }
            NPN_ReleaseVariantValue(&constructor);
        }
        return "Object";
    }
    // In Firefox, the constructor and an instance object cannot be distinguished by toString().
    // Check if object has a 'prototype' to see if it is a constructor.
    if (asConstructor && NPN_HasProperty(npp, object, NPN_GetStringIdentifier("prototype")))
    {
        className += "_Constructor";
    }
    return className;
}

bool convertToBool(NPP npp, const NPVariant* variant)
{
    return toBoolean(variant);
}

int8_t convertToByte(NPP npp, const NPVariant* variant)
{
    return static_cast<int8_t>(toInteger(npp, variant));
}

uint8_t convertToOctet(NPP npp, const NPVariant* variant)
{
    return static_cast<uint8_t>(toInteger(npp, variant));
}

int16_t convertToShort(NPP npp, const NPVariant* variant)
{
    return static_cast<int16_t>(toInteger(npp, variant));
}

uint16_t convertToUnsignedShort(NPP npp, const NPVariant* variant)
{
    return static_cast<uint16_t>(toInteger(npp, variant));
}

int32_t convertToLong(NPP npp, const NPVariant* variant)
{
    return static_cast<int32_t>(toInteger(npp, variant));
}

uint32_t convertToUnsignedLong(NPP npp, const NPVariant* variant)
{
    return static_cast<uint32_t>(toInteger(npp, variant));
}

int64_t convertToLongLong(NPP npp, const NPVariant* variant)
{
    return static_cast<int64_t>(toInteger(npp, variant));
}

uint64_t convertToUnsignedLongLong(NPP npp, const NPVariant* variant)
{
    return static_cast<uint64_t>(toInteger(npp, variant));
}

float convertToFloat(NPP npp, const NPVariant* variant)
{
    return static_cast<float>(toNumber(npp, variant));
}

double convertToDouble(NPP npp, const NPVariant* variant)
{
    return toNumber(npp, variant);
}

std::string convertToString(NPP npp, const NPVariant* variant, unsigned attribute)
{
  return toString(npp, variant, attribute);
}

Object* convertToObject(NPP npp, const NPVariant* variant, const Reflect::Type type)
{
    if (!NPVARIANT_IS_OBJECT(*variant))
    {
        return 0;
    }
    NPObject* object = NPVARIANT_TO_OBJECT(*variant);
    if (!object)
    {
        return 0;
    }
    if (StubObject::isStub(object))
    {
        StubObject* stub = static_cast<StubObject*>(object);
        return stub->getObject();
    }
    PluginInstance* instance = static_cast<PluginInstance*>(npp->pdata);
    if (instance)
    {
        ProxyControl* proxyControl = instance->getProxyControl();
        if (proxyControl)
        {
            return proxyControl->createProxy(object, type);
        }
    }
    return 0;
}

Any convertToAny(NPP npp, const NPVariant* variant)
{
    switch (variant->type)
    {
    case NPVariantType_Void:
    case NPVariantType_Null:
        return Any();
        break;
    case NPVariantType_Bool:
        return convertToBool(npp, variant);
        break;
    case NPVariantType_Int32:
        return convertToLong(npp, variant);
        break;
    case NPVariantType_Double:
        return convertToDouble(npp, variant);
        break;
    case NPVariantType_String:
        return convertToString(npp, variant);
        break;
    case NPVariantType_Object:
    {
        Object* object = convertToObject(npp, variant);
        if (!object)
        {
            return Any();
        }
        else
        {
            return Any(object);
        }
        break;
    }
    default:
        return Any();
        break;
    }
}

namespace
{

template <typename T>
Any convertToSequence(NPP npp, const NPVariant* variant)
{
    unsigned length = 0;
    NPVariant result;
    INT32_TO_NPVARIANT(0, result);
    if (NPVARIANT_IS_OBJECT(*variant) &&
        NPN_GetProperty(npp, NPVARIANT_TO_OBJECT(*variant), NPN_GetStringIdentifier("length"), &result))
    {
        if (NPVARIANT_IS_INT32(result)) {
            length = NPVARIANT_TO_INT32(result);
        }
        NPN_ReleaseVariantValue(&result);
    }
    Sequence<T> sequence(length);
    for (unsigned i = 0; i < length; ++i)
    {
        NPVariant element;
        VOID_TO_NPVARIANT(element);
        NPIdentifier id = NPN_GetIntIdentifier(i);
        if (NPN_GetProperty(npp, NPVARIANT_TO_OBJECT(*variant), id, &element))
        {
            Any value = convertToAny(npp, &element);  // TODO: Should use direct converToXXX()
            sequence.setElement(i, static_cast<T>(value));
        }
    }
    return sequence;
}

}

Any convertToAny(NPP npp, const NPVariant* variant, const Reflect::Type type)
{
    if (type.isSequence())
    {
        Reflect::Sequence sequence(type);
        Reflect::Type elementType(sequence.getType());
        if (elementType.isNullable())
        {
            switch (type.getType())
            {
            case Reflect::kBoolean:
                return convertToSequence<Nullable<bool> >(npp, variant);
                break;
            case Reflect::kByte:
                return convertToSequence<Nullable<int8_t> >(npp, variant);
                break;
            case Reflect::kOctet:
                return convertToSequence<Nullable<uint8_t> >(npp, variant);
                break;
            case Reflect::kShort:
                return convertToSequence<Nullable<int16_t> >(npp, variant);
                break;
            case Reflect::kUnsignedShort:
                return convertToSequence<Nullable<uint16_t> >(npp, variant);
                break;
            case Reflect::kLong:
                return convertToSequence<Nullable<int32_t> >(npp, variant);
                break;
            case Reflect::kUnsignedLong:
                return convertToSequence<Nullable<uint32_t> >(npp, variant);
                break;
            case Reflect::kLongLong:
                return convertToSequence<Nullable<int64_t> >(npp, variant);
                break;
            case Reflect::kUnsignedLongLong:
                return convertToSequence<Nullable<uint64_t> >(npp, variant);
                break;
            case Reflect::kFloat:
                return convertToSequence<Nullable<float> >(npp, variant);
                break;
            case Reflect::kDouble:
                return convertToSequence<Nullable<double> >(npp, variant);
                break;
            case Reflect::kString:
                return convertToSequence<Nullable<std::string> >(npp, variant);
                break;
            }
        }
        else {
            switch (type.getType())
            {
            case Reflect::kBoolean:
                return convertToSequence<bool>(npp, variant);
                break;
            case Reflect::kByte:
                return convertToSequence<int8_t>(npp, variant);
                break;
            case Reflect::kOctet:
                return convertToSequence<uint8_t>(npp, variant);
                break;
            case Reflect::kShort:
                return convertToSequence<int16_t>(npp, variant);
                break;
            case Reflect::kUnsignedShort:
                return convertToSequence<uint16_t>(npp, variant);
                break;
            case Reflect::kLong:
                return convertToSequence<int32_t>(npp, variant);
                break;
            case Reflect::kUnsignedLong:
                return convertToSequence<uint32_t>(npp, variant);
                break;
            case Reflect::kLongLong:
                return convertToSequence<int64_t>(npp, variant);
                break;
            case Reflect::kUnsignedLongLong:
                return convertToSequence<uint64_t>(npp, variant);
                break;
            case Reflect::kFloat:
                return convertToSequence<float>(npp, variant);
                break;
            case Reflect::kDouble:
                return convertToSequence<double>(npp, variant);
                break;
            case Reflect::kString:
                return convertToSequence<std::string>(npp, variant);
                break;
            case Reflect::kAny:
                return convertToSequence<Any>(npp, variant);
                break;
            // TODO: kSequence???
            case Reflect::kObject:
            default:
                return convertToSequence<Object*>(npp, variant);
                break;
            }
        }
    }

    switch (type.getType())
    {
    case Reflect::kBoolean:
        return convertToBool(npp, variant);
        break;
    case Reflect::kByte:
        return convertToByte(npp, variant);
        break;
    case Reflect::kOctet:
        return convertToOctet(npp, variant);
        break;
    case Reflect::kShort:
        return convertToShort(npp, variant);
        break;
    case Reflect::kUnsignedShort:
        return convertToUnsignedShort(npp, variant);
        break;
    case Reflect::kLong:
        return convertToLong(npp,variant);
        break;
    case Reflect::kUnsignedLong:
        return convertToUnsignedLong(npp, variant);
        break;
    case Reflect::kLongLong:
        return convertToLongLong(npp, variant);
        break;
    case Reflect::kUnsignedLongLong:
        return convertToUnsignedLongLong(npp, variant);
        break;
    case Reflect::kFloat:
        return convertToFloat(npp, variant);
        break;
    case Reflect::kDouble:
        return convertToDouble(npp, variant);
        break;
    case Reflect::kString:
        return convertToString(npp, variant);
        break;
    case Reflect::kDate:
        // Note DOM3 Core compliant browser (e.g., Chrome) returns a Date object for DOMTimeStamp
        return convertToUnsignedLongLong(npp, variant);
        break;
    case Reflect::kObject:
    {
        Object* object = convertToObject(npp, variant);
        if (!object)
        {
            return Any();
        }
        else
        {
            return Any(object);
        }
        break;
    }
    default:
        return convertToAny(npp, variant);
        break;
    }
}

void convertToVariant(NPP npp, bool value, NPVariant* variant)
{
    BOOLEAN_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, int8_t value, NPVariant* variant)
{
    INT32_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, uint8_t value, NPVariant* variant)
{
    INT32_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, int16_t value, NPVariant* variant)
{
    INT32_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, uint16_t value, NPVariant* variant)
{
    INT32_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, int32_t value, NPVariant* variant)
{
    INT32_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, uint32_t value, NPVariant* variant)
{
    INT32_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, int64_t value, NPVariant* variant)
{
    DOUBLE_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, uint64_t value, NPVariant* variant)
{
    DOUBLE_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, double value, NPVariant* variant)
{
    DOUBLE_TO_NPVARIANT(value, *variant);
}

void convertToVariant(NPP npp, const std::string& value, NPVariant* variant)
{
    STRINGN_TO_NPVARIANT(value.c_str(), value.length(), *variant);
}

void convertToVariant(NPP npp, Object* value, NPVariant* variant)
{
    assert(value);
    if (ProxyObject* proxy = interface_cast<ProxyObject*>(value))
    {
        OBJECT_TO_NPVARIANT(proxy->getNPObject(), *variant);
        return;
    }
    if (PluginInstance* instance = static_cast<PluginInstance*>(npp->pdata))
    {
        StubControl* stubControl = instance->getStubControl();
        if (stubControl)
        {
            NPObject* object = stubControl->createStub(value);
            if (object)
            {
                OBJECT_TO_NPVARIANT(object, *variant);
                return;
            }
        }
    }
    NULL_TO_NPVARIANT(*variant);
}

namespace
{

NPObject* createArray(NPP npp)
{
    NPObject* array = 0;
    NPObject* window;
    NPN_GetValue(npp, NPNVWindowNPObject, &window);
    NPString script =
    {
        "new Array();",
        12
    };
    NPVariant result;
    if (NPN_Evaluate(npp, window, &script, &result))
    {
        if (NPVARIANT_IS_OBJECT(result))
        {
            array = NPVARIANT_TO_OBJECT(result);
        }
        else
        {
            NPN_ReleaseVariantValue(&result);
        }
    }
    NPN_ReleaseObject(window);
    return array;
}

template <typename T>
void copySequence(NPP npp, NPObject* array, const Sequence<T> sequence)
{
    for (unsigned i = 0; i < sequence.getLength(); ++i)
    {
        NPIdentifier id = NPN_GetIntIdentifier(i);
        NPVariant element;
        convertToVariant(npp, sequence[i], &element);
        NPN_SetProperty(npp, array, id, &element);
    }
}

}

void convertToVariant(NPP npp, const Any& any, NPVariant* variant)
{
    if (any.isSequence())
    {
        NPObject* array = createArray(npp);
        if (array)
        {
            // TODO: Check nullable; Any needs to be fixed, too.
            if (any.isAny())
            {
                copySequence(npp, array, Sequence<Any>(any));
            }
            else switch (any.getType())
            {
            case Any::TypeBool:
                copySequence(npp, array, Sequence<bool>(any));
                break;
            case Any::TypeByte:
                copySequence(npp, array, Sequence<int8_t>(any));
                break;
            case Any::TypeOctet:
                copySequence(npp, array, Sequence<uint8_t>(any));
                break;
            case Any::TypeShort:
                copySequence(npp, array, Sequence<int16_t>(any));
                break;
            case Any::TypeUnsignedShort:
                copySequence(npp, array, Sequence<uint16_t>(any));
                break;
            case Any::TypeLong:
                copySequence(npp, array, Sequence<int32_t>(any));
                break;
            case Any::TypeUnsignedLong:
                copySequence(npp, array, Sequence<uint32_t>(any));
                break;
            case Any::TypeLongLong:
                copySequence(npp, array, Sequence<int64_t>(any));
                break;
            case Any::TypeUnsignedLongLong:
                copySequence(npp, array, Sequence<uint64_t>(any));
                break;
            case Any::TypeFloat:
                copySequence(npp, array, Sequence<float>(any));
                break;
            case Any::TypeDouble:
                copySequence(npp, array, Sequence<double>(any));
                break;
            case Any::TypeString:
                copySequence(npp, array, Sequence<std::string>(any));
                break;
            // TODO: Any::Sequence?
            case Any::TypeObject:
            default:
                copySequence(npp, array, Sequence<Object*>(any));
                break;
            }
            OBJECT_TO_NPVARIANT(array, *variant);
            return;
        }
        NULL_TO_NPVARIANT(*variant);
        return;
    }

    switch (any.getType())
    {
    case Any::TypeVoid:
        NULL_TO_NPVARIANT(*variant);
        break;
    case Any::TypeBool:
        convertToVariant(npp, static_cast<bool>(any), variant);
        break;
    case Any::TypeByte:
        convertToVariant(npp, static_cast<int8_t>(any), variant);
        break;
    case Any::TypeOctet:
        convertToVariant(npp, static_cast<uint8_t>(any), variant);
        break;
    case Any::TypeShort:
        convertToVariant(npp, static_cast<int16_t>(any), variant);
        break;
    case Any::TypeUnsignedShort:
        convertToVariant(npp, static_cast<uint16_t>(any), variant);
        break;
    case Any::TypeLong:
        convertToVariant(npp, static_cast<int32_t>(any), variant);
        break;
    case Any::TypeUnsignedLong:
        convertToVariant(npp, static_cast<uint32_t>(any), variant);
        break;
    case Any::TypeLongLong:
        convertToVariant(npp, static_cast<int64_t>(any), variant);
        break;
    case Any::TypeUnsignedLongLong:
        convertToVariant(npp, static_cast<uint64_t>(any), variant);
        break;
    case Any::TypeFloat:
        convertToVariant(npp, static_cast<float>(any), variant);
        break;
    case Any::TypeDouble:
        convertToVariant(npp, static_cast<double>(any), variant);
        break;
    case Any::TypeString:
        convertToVariant(npp, static_cast<const std::string>(any), variant);
        break;
    case Any::TypeObject:
        convertToVariant(npp, static_cast<Object*>(any), variant);
        break;
    default:
        VOID_TO_NPVARIANT(*variant);
        break;
    }
}
