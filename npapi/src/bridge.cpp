/*
 * Copyright 2011 Esrille Inc.
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
            break;
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

Any convertToAny(NPP npp, const NPVariant* variant)
{
    switch (variant->type)
    {
    case NPVariantType_Void:
    case NPVariantType_Null:
        return Any();
        break;
    case NPVariantType_Bool:
        return NPVARIANT_TO_BOOLEAN(*variant);
        break;
    case NPVariantType_Int32:
        return NPVARIANT_TO_INT32(*variant);
        break;
    case NPVariantType_Double:
        return NPVARIANT_TO_DOUBLE(*variant);
        break;
    case NPVariantType_String:
        return std::string(NPVARIANT_TO_STRING(*variant).utf8characters, NPVARIANT_TO_STRING(*variant).utf8length);
        break;
    case NPVariantType_Object:
    {
        NPObject* object = NPVARIANT_TO_OBJECT(*variant);
        if (!object)
        {
            break;
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
                return proxyControl->createProxy(object);
            }
        }
        break;
    }
    default:
        break;
    }
    return Any();
}

void convertToVariant(NPP npp, const std::string& value, NPVariant* variant, bool result)
{
    if (value.length() == 0)
    {
        STRINGN_TO_NPVARIANT(0, 0, *variant);
        return;
    }
    if (!result)
    {
        STRINGN_TO_NPVARIANT(value.c_str(), value.length(), *variant);
        return;
    }
    void* buffer = NPN_MemAlloc(value.length());
    if (!buffer)
    {
        STRINGN_TO_NPVARIANT(0, 0, *variant);
        return;
    }
    memmove(buffer, value.c_str(), value.length());
    STRINGN_TO_NPVARIANT(static_cast<NPUTF8*>(buffer), value.length(), *variant);
}

void convertToVariant(NPP npp, Object* value, NPVariant* variant, bool result)
{
    if (!value)
    {
        NULL_TO_NPVARIANT(*variant);
        return;
    }
    if (ProxyObject* proxy = dynamic_cast<ProxyObject*>(value))
    {
        if (result)
        {
            NPN_RetainObject(proxy->getNPObject());
        }
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
                if (result)
                {
                    NPN_RetainObject(object);
                }
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
void copySequence(NPP npp, NPObject* array, const Sequence<T> sequence, bool result)
{
    for (unsigned i = 0; i < sequence.getLength(); ++i)
    {
        NPIdentifier id = NPN_GetIntIdentifier(i);
        NPVariant element;
        convertToVariant(npp, sequence[i], &element, result);
        NPN_SetProperty(npp, array, id, &element);
    }
}

}

void convertToVariant(NPP npp, const Any& any, NPVariant* variant, bool result)
{
    switch (any.getType())
    {
    case Any::Empty:
        NULL_TO_NPVARIANT(*variant);
        break;
    case Any::Bool:
        BOOLEAN_TO_NPVARIANT(any.toBoolean(), *variant);
        break;
    case Any::Int32:
    case Any::Uint32:
        INT32_TO_NPVARIANT(static_cast<int32_t>(any), *variant);
        break;
    case Any::Int64:
    case Any::Uint64:
    case Any::Float32:
    case Any::Float64:
        DOUBLE_TO_NPVARIANT(static_cast<double>(any), *variant);
        break;
    case Any::Dynamic:
        if (any.isString())
        {
            std::string value = any.toString();
printf("%s '%s'\n", __func__, value.c_str());
            if (value.length() == 0)
            {
                STRINGN_TO_NPVARIANT(0, 0, *variant);
            }
            else if (!result)
            {
                STRINGN_TO_NPVARIANT(value.c_str(), value.length(), *variant);
            }
            else {
                void* buffer = NPN_MemAlloc(value.length());
                if (!buffer)
                {
                    STRINGN_TO_NPVARIANT(0, 0, *variant);
                }
                else
                {
                    memmove(buffer, value.c_str(), value.length());
                    STRINGN_TO_NPVARIANT(static_cast<NPUTF8*>(buffer), value.length(), *variant);
                }
            }
        }
        else
        {
            assert(any.isObject());
            convertToVariant(npp, any.toObject(), variant, result);
        }
        break;
    default:
        VOID_TO_NPVARIANT(*variant);
        break;
    }
}