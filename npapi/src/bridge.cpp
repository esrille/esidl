/*
 * Copyright 2009-2010 Google Inc.
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
#include "reflect.h"
#include "proxyImpl.h"

#include <stdio.h>

#include <algorithm>
#include <map>
#include <ctype.h>
#include <math.h>

namespace
{

// Map from interfaceName to constructors.
std::map<const std::string, Object* (*)(ProxyObject object)> proxyConstructorMap;
std::map<const std::string, NPObject* (*)(NPP npp, Object* object)> stubConstructorMap;
std::map<const std::string, Reflect::Interface> metaDataMap;

bool isStub(const NPObject* object)
{
    return object->_class->deallocate == StubObject::Deallocate;
}

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

// TODO: Check length strictly.
double toNumber(const char* string, size_t length)
{
    const char* ptr = string;
    double number;
    while (*ptr && isspace(*ptr))  // TODO: Check UTF8 space
    {
        ++ptr;
    }
    char* end;
    if (strncasecmp(ptr, "0x", 2) == 0)
    {
        ptr += 2;
        number = strtoll(ptr, &end, 16);
    }
    else if (strncmp(ptr, "Infinity", 8) == 0)
    {
        end = const_cast<char*>(ptr) + 8;
        number = INFINITY;
    } else
    {
        number = strtod(ptr, &end);
    }
    if (end == ptr)
    {
        return NAN;
    }
    while (*end && isspace(*end))  // TODO: Check UTF8 space
    {
        ++end;
    }
    if (*end != 0 || string + length < end)
    {
        return NAN;
    }
    return number;
};

double toNumber(const NPVariant* variant)
{
    switch (variant->type)
    {
        case NPVariantType_Void:
        return NAN;
    break;
        case NPVariantType_Null:
        return 0.0;
    break;
        case NPVariantType_Bool:
        return NPVARIANT_TO_BOOLEAN(*variant) ? 1.0 : 0.0;
    break;
        case NPVariantType_Int32:
        return NPVARIANT_TO_INT32(*variant);
    break;
        case NPVariantType_Double:
        return NPVARIANT_TO_DOUBLE(*variant);
        break;
    case NPVariantType_String:
        return toNumber(NPVARIANT_TO_STRING(*variant).utf8characters,
                        NPVARIANT_TO_STRING(*variant).utf8length);
        break;
    case NPVariantType_Object:
        return NAN; // TODO: Try valueOf, toString, etc.
        break;
    default:
        return NAN;
        break;
    }
}

long long toInteger(const NPVariant* variant)
{
    double value = toNumber(variant);
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
        if (attribute & Reflect::kUndefinedIsNull)
        {
            return "null";  // TODO should be something like NULL
        }
        if (attribute & Reflect::kUndefinedIsEmpty)
        {
            return "";
        }
        return "undefined";
        break;
    case NPVariantType_Null:
        if (attribute & Reflect::kNullIsNull)
        {
            return "null";  // TODO should be something like NULL
        }
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

void addProxyConstructor(const std::string interfaceName, Object* (*createProxy)(ProxyObject object))
{
    proxyConstructorMap[interfaceName] = createProxy;
}

void addStubConstructor(const std::string interfaceName, NPObject* (*createStub)(NPP npp, Object* object))
{
    stubConstructorMap[interfaceName] = createStub;
}

Reflect::Interface* getInterfaceData(const std::string className)
{
    std::map<std::string, Reflect::Interface>::iterator i;

    i = metaDataMap.find(className);
    if (i == metaDataMap.end())
    {
        return 0;
    }
    return &((*i).second);
}

Reflect::Interface* getInterfaceData(const char* iid)
{
    std::map<std::string, Reflect::Interface>::iterator i;

    i = metaDataMap.find(iid);
    if (i == metaDataMap.end())
    {
        return 0;
    }
    return &((*i).second);
}

void addInterfaceData(const char* metaData, std::string name)
{
    metaDataMap[name] = Reflect::Interface(metaData);
}

void registerMetaData(const char* meta,
                      Object* (*createProxy)(ProxyObject object),
                      const char* alias)
{
    Reflect::Interface interface(meta);
    std::string name = interface.getName();
    if (alias)
    {
        name = alias;
    }
    addInterfaceData(meta, name);
    addProxyConstructor(name, createProxy);
    printf("%s\n", name.c_str());
}


NPObject* createStub(NPP npp, const char* interfaceName, Object* object)
{
    // Invokes createInstance method of the corresponding stub class.
    std::map<std::string, NPObject* (*)(NPP npp, Object* object)>::iterator i;
    i = stubConstructorMap.find(interfaceName);
    if (i == stubConstructorMap.end())
    {
        return NULL;
    }
    return (*i).second(npp, object);
}

Object* createProxy(NPP npp, NPObject* object)
{
    if (!object)
    {
        return 0;
    }

    std::string className = getInterfaceName(npp, object);

    if (className == "Function" || className == "Object")
    {
        // TODO: We should define 'Object' interface
        return 0;
    }

    std::map<std::string, Object* (*)(ProxyObject object)>::iterator it;
    it = proxyConstructorMap.find(className);
    if (it != proxyConstructorMap.end())
    {
        std::string interfaceName = getInterfaceName(npp, object);
        ProxyObject browserObject = ProxyObject(object, npp, interfaceName);
        return (*it).second(browserObject);
    }
    return 0;
}

std::string getInterfaceName(NPP npp, NPObject* object)
{
    std::string className;
    NPVariant result;

    VOID_TO_NPVARIANT(result);
    NPN_Invoke(npp, object, NPN_GetStringIdentifier("toString"), 0, 0, &result);
    if (NPVARIANT_IS_STRING(result))
    {
        className = std::string(NPVARIANT_TO_STRING(result).utf8characters,
                                NPVARIANT_TO_STRING(result).utf8length);
    }
    NPN_ReleaseVariantValue(&result);
    if (className.compare(0, 9, "function ") == 0)
    {
        return "Function";
    }
    if (className.compare(0, 8, "[object ", 8) != 0 ||
        className[className.length() - 1] != ']')
    {
        return "Object";  // TODO: should we return a better name?
    }
    return className.substr(8, className.length() - 9);
}

bool convertToBool(const NPVariant* variant)
{
    return toBoolean(variant);
}

uint8_t convertToOctet(const NPVariant* variant)
{
    return static_cast<uint8_t>(toInteger(variant));
}

int16_t convertToShort(const NPVariant* variant)
{
    return static_cast<int16_t>(toInteger(variant));
}

uint16_t convertToUnsignedShort(const NPVariant* variant)
{
    return static_cast<uint16_t>(toInteger(variant));
}

int32_t convertToLong(const NPVariant* variant)
{
    return static_cast<int32_t>(toInteger(variant));
}

uint32_t convertToUnsignedLong(const NPVariant* variant)
{
    return static_cast<uint32_t>(toInteger(variant));
}

int64_t convertToLongLong(const NPVariant* variant)
{
    return static_cast<int64_t>(toInteger(variant));
}

uint64_t convertToUnsignedLongLong(const NPVariant* variant)
{
    return static_cast<uint64_t>(toInteger(variant));
}

float convertToFloat(const NPVariant* variant)
{
    return static_cast<float>(toNumber(variant));
}

double convertToDouble(const NPVariant* variant)
{
    return static_cast<double>(toNumber(variant));
}

std::string convertToString(NPP npp, const NPVariant* variant, unsigned attribute)
{
  return toString(npp, variant, attribute);
}

Object* convertToObject(NPP npp, const NPVariant* variant, char* buffer, size_t length)
{
    // If the embedded NPObject is a stub object, set the original interface
    // pointer to any. Otherwise, call toString() method to retrieve the class
    // name and create a corresponding proxy and set it to any.
    if (!NPVARIANT_IS_OBJECT(*variant))
    {
        return NULL;
    }
    NPObject* object = NPVARIANT_TO_OBJECT(*variant);
    if (!object)
    {
        return NULL;
    }
    if (isStub(object))
    {
        StubObject* stub = static_cast<StubObject*>(object);
        return stub->getObject();
    }
    else
    {
        return createProxy(npp, object);
    }
}

Any convertToAny(NPP npp, const NPVariant* variant, void* buffer, size_t length)
{
    switch (variant->type)
    {
    case NPVariantType_Void:
    case NPVariantType_Null:
        return Any();
        break;
    case NPVariantType_Bool:
        return convertToBool(variant);
        break;
    case NPVariantType_Int32:
        return convertToLong(variant);
        break;
    case NPVariantType_Double:
        return convertToDouble(variant);
        break;
    case NPVariantType_String:
        return convertToString(npp, variant);
        break;
    case NPVariantType_Object:
    {
        Object* object = convertToObject(npp, variant, static_cast<char*>(buffer), length);
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

Any convertToAny(NPP npp, const NPVariant* variant, int type, void* buffer, size_t length)
{
    switch (type)
    {
    case Any::TypeVoid:
        return Any();
        break;
    case Any::TypeBool:
        return convertToBool(variant);
        break;
    case Any::TypeOctet:
        return convertToOctet(variant);
        break;
    case Any::TypeShort:
        return convertToShort(variant);
        break;
    case Any::TypeUnsignedShort:
        return convertToUnsignedShort(variant);
        break;
    case Any::TypeLong:
        return convertToLong(variant);
        break;
    case Any::TypeUnsignedLong:
        return convertToUnsignedLong(variant);
        break;
    case Any::TypeLongLong:
        return convertToLongLong(variant);
        break;
    case Any::TypeUnsignedLongLong:
        return convertToUnsignedLongLong(variant);
        break;
    case Any::TypeFloat:
        return convertToFloat(variant);
        break;
    case Any::TypeDouble:
        return convertToDouble(variant);
        break;
    case Any::TypeString:
        return convertToString(npp, variant);
        break;
    case Any::TypeObject:
    {
        Object* object = convertToObject(npp, variant, static_cast<char*>(buffer), length);
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

void convertToVariant(NPP npp, bool value, NPVariant* variant)
{
    BOOLEAN_TO_NPVARIANT(value, *variant);
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
    if (!value)
    {
        NULL_TO_NPVARIANT(*variant);
        return;
    }
    if (ProxyObject* proxy = dynamic_cast<ProxyObject*>(value))
    {
        OBJECT_TO_NPVARIANT(proxy->getNPObject(), *variant);
    }
    else
    {
        std::string interfaceName = value->getInterfaceName();
        NPObject* object = createStub(npp, interfaceName.c_str(), value);
        OBJECT_TO_NPVARIANT(object, *variant);
    }
}

void convertToVariant(NPP npp, const Any& any, NPVariant* variant)
{
    switch (any.getType())
    {
    case Any::TypeVoid:
        VOID_TO_NPVARIANT(*variant);
        break;
    case Any::TypeBool:
        convertToVariant(npp, static_cast<bool>(any), variant);
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
        NULL_TO_NPVARIANT(*variant);
        break;
    }
}
