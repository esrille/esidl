/*
 * Copyright 2009 Google Inc.
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

#include <stdio.h>

#include "esnpapi.h"
#include "reflect.h"

namespace
{

NPIdentifier getSpecialIdentifier(const Any& any)
{
    NPIdentifier id;

    if (any.isString())
    {
        id =  NPN_GetStringIdentifier(static_cast<const std::string>(any).c_str());
    }
    else
    {
        id = NPN_GetIntIdentifier(static_cast<uint32_t>(any));
    }
    return id;
}

}  // namespace

Any invoke(ProxyObject& object, unsigned methodNumber, unsigned paramCount, Any* paramArray)
{
    void* buffer;   // TODO for sequence
    size_t length;  // TODO for sequence

    Reflect::Interface* interface = getInterfaceData(object.getInterfaceName());
    while (interface)
    {
        if (interface->getInheritedMethodCount() <= methodNumber)
        {
            methodNumber -= interface->getInheritedMethodCount();
            break;
        }
        else
        {
            std::string name = interface->getQualifiedSuperName();
            if (name == "")
            {
                return Any();
            }
            interface = getInterfaceData(name.c_str());
        }
    }
    if (!interface)
    {
        return Any();
    }

    Reflect::Method method = interface->getMethod(methodNumber);
    Reflect::Type type = method.getReturnType();

    printf("invoke %s::%s %p\n", interface->getName().c_str(), method.getName().c_str(), object.getNPObject());

    NPIdentifier id;
    NPVariant result;
    VOID_TO_NPVARIANT(result);

    if (method.isGetter())
    {
        id = NPN_GetStringIdentifier(method.getName().c_str());
        if (NPN_GetProperty(object.getNPP(), object.getNPObject(), id, &result))
        {
            return convertToAny(object.getNPP(), &result, buffer, length);
        }
    }
    else if (method.isSetter())
    {
        id = NPN_GetStringIdentifier(method.getName().c_str());
        NPVariant value;
        convertToVariant(object.getNPP(), paramArray[paramCount - 1], &value);
        NPN_SetProperty(object.getNPP(), object.getNPObject(), id, &value);
    }
    else if (method.isSpecialGetter())
    {
        id = getSpecialIdentifier(paramArray[paramCount - 1]);
        if (NPN_GetProperty(object.getNPP(), object.getNPObject(), id, &result))
        {
            return convertToAny(object.getNPP(), &result, buffer, length);
        }
    }
    else if (method.isSpecialSetter() || method.isSpecialCreator())
    {
        id = getSpecialIdentifier(paramArray[paramCount - 2]);
        NPVariant value;
        convertToVariant(object.getNPP(), paramArray[paramCount - 1], &value);
        NPN_SetProperty(object.getNPP(), object.getNPObject(), id, &value);
    }
    else if (method.isSpecialDeleter())
    {
        id = getSpecialIdentifier(paramArray[paramCount - 1]);
        NPN_RemoveProperty(object.getNPP(), object.getNPObject(), id);
    }
    else
    {
        unsigned variadicCount = 0;
        if (method.isVariadic())
        {
            variadicCount = static_cast<unsigned>(paramArray[paramCount - 1]);
            paramCount -= 2;
        }
        unsigned variantCount = paramCount - 1 + variadicCount;
        NPVariant variantArray[variantCount];
        for (int i = 1; i < paramCount; ++i)
        {
            convertToVariant(object.getNPP(), paramArray[i], &variantArray[i - 1]);
        }
        if (0 < variadicCount)
        {
            Reflect::Parameter param = method.listParameter();
            for (int i = 0; i < method.getParameterCount(); ++i)
            {
                param.next();
            }
            switch (param.getType().getType())
            {
            case Reflect::kBoolean:
            {
                const bool* variadic = reinterpret_cast<const bool*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kOctet:
            {
                const uint8_t* variadic = reinterpret_cast<const uint8_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kShort:
            {
                const int16_t* variadic = reinterpret_cast<const int16_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kUnsignedShort:
            {
                const uint16_t* variadic = reinterpret_cast<const uint16_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kLong:
            {
                const int32_t* variadic = reinterpret_cast<const int32_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kUnsignedLong:
            {
                const uint32_t* variadic = reinterpret_cast<const uint32_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kLongLong:
            {
                const int64_t* variadic = reinterpret_cast<const int64_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kUnsignedLongLong:
            {
                const uint64_t* variadic = reinterpret_cast<const uint64_t*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kFloat:
            {
                const float* variadic = reinterpret_cast<const float*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kDouble:
            {
                const double* variadic = reinterpret_cast<const double*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kString:
            {
                const std::string* variadic = reinterpret_cast<const std::string*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kAny:
            {
                const Any* variadic = reinterpret_cast<const Any*>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            case Reflect::kObject:
            {
                Object** variadic = reinterpret_cast<Object**>(static_cast<intptr_t>(paramArray[paramCount]));
                for (int i = paramCount - 1; i < variantCount; ++i)
                {
                    convertToVariant(object.getNPP(), *variadic++, &variantArray[i]);
                }
                break;
            }
            default:
                // TODO: Are we going to support non-simple types with variadic?
                break;
            }
        }
        id = NPN_GetStringIdentifier(method.getName().c_str());
        if (NPN_Invoke(object.getNPP(), object.getNPObject(), id, variantArray, variantCount, &result))
        {
            return convertToAny(object.getNPP(), &result, buffer, length);
        }
    }
    return Any();
}
