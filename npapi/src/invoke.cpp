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

#include <stdio.h>

#include "esnpapi.h"

#include <any.h>
#include <reflect.h>

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

template <typename T>
void expandSequence(NPP npp, const Sequence<T> sequence, NPVariant* variantArray)
{
    for (unsigned i = 0; i < sequence.getLength(); ++i)
    {
        convertToVariant(npp, sequence[i], variantArray + i);
    }
}

}  // namespace

Any invoke(Object* object, unsigned interfaceNumber, unsigned methodNumber,
           const char* meta, unsigned offset,
           unsigned argumentCount, Any* arguments)
{
    ProxyObject* proxy = interface_cast<ProxyObject*>(object);  // XXX
    if (!proxy)
    {
        return Any();
    }
    Reflect::Interface interface = Reflect::Interface(meta);
    Reflect::Method method = Reflect::Method(meta + offset);
    Reflect::Type type = method.getReturnType();

    printf("invoke %s::%s %p\n", interface.getName().c_str(), method.getName().c_str(), proxy->getNPObject());

    NPIdentifier id;
    NPVariant result;
    VOID_TO_NPVARIANT(result);

    if (method.isGetter())
    {
        id = NPN_GetStringIdentifier(method.getName().c_str());
        if (NPN_GetProperty(proxy->getNPP(), proxy->getNPObject(), id, &result))
        {
            return convertToAny(proxy->getNPP(), &result);
        }
    }
    else if (method.isSetter())
    {
        id = NPN_GetStringIdentifier(method.getName().c_str());
        NPVariant value;
        convertToVariant(proxy->getNPP(), arguments[argumentCount - 1], &value);
        NPN_SetProperty(proxy->getNPP(), proxy->getNPObject(), id, &value);
    }
    else if (method.isSpecialGetter())
    {
        id = getSpecialIdentifier(arguments[argumentCount - 1]);
        if (NPN_GetProperty(proxy->getNPP(), proxy->getNPObject(), id, &result))
        {
            return convertToAny(proxy->getNPP(), &result);
        }
    }
    else if (method.isSpecialSetter() || method.isSpecialCreator())
    {
        id = getSpecialIdentifier(arguments[argumentCount - 2]);
        NPVariant value;
        convertToVariant(proxy->getNPP(), arguments[argumentCount - 1], &value);
        NPN_SetProperty(proxy->getNPP(), proxy->getNPObject(), id, &value);
    }
    else if (method.isSpecialDeleter())
    {
        id = getSpecialIdentifier(arguments[argumentCount - 1]);
        NPN_RemoveProperty(proxy->getNPP(), proxy->getNPObject(), id);
    }
    else
    {
        unsigned variadicCount = 0;
        if (method.isVariadic())
        {
            --argumentCount;
            Sequence<int> sequence(arguments[argumentCount]);  // TODO: make sure this works ignoring T
            variadicCount = sequence.getLength();
        }

        unsigned variantCount = argumentCount + variadicCount;
        NPVariant variantArray[variantCount];
        for (int i = 0; i < argumentCount; ++i)
        {
            convertToVariant(proxy->getNPP(), arguments[i], &variantArray[i]);
        }

        if (0 < variadicCount)
        {
            Reflect::Parameter param = method.listParameter();
            for (int i = 0; i < method.getParameterCount(); ++i)
            {
                param.next();
            }
            // TODO: Check Nullable, too.
            switch (param.getType().getType())
            {
            case Reflect::kBoolean:
                expandSequence(proxy->getNPP(), Sequence<bool>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kOctet:
                expandSequence(proxy->getNPP(), Sequence<uint8_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kShort:
                expandSequence(proxy->getNPP(), Sequence<int16_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kUnsignedShort:
                expandSequence(proxy->getNPP(), Sequence<uint16_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kLong:
                expandSequence(proxy->getNPP(), Sequence<int32_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kUnsignedLong:
                expandSequence(proxy->getNPP(), Sequence<uint32_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kLongLong:
                expandSequence(proxy->getNPP(), Sequence<int64_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kUnsignedLongLong:
                expandSequence(proxy->getNPP(), Sequence<uint64_t>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kFloat:
                expandSequence(proxy->getNPP(), Sequence<float>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kDouble:
                expandSequence(proxy->getNPP(), Sequence<double>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kString:
                expandSequence(proxy->getNPP(), Sequence<std::string>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            case Reflect::kAny:
                expandSequence(proxy->getNPP(), Sequence<Any>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            // TODO: kSequence???
            case Reflect::kObject:
            default:
                expandSequence(proxy->getNPP(), Sequence<Object*>(arguments[argumentCount]), &variantArray[argumentCount]);
                break;
            }
        }

        id = NPN_GetStringIdentifier(method.getName().c_str());
        if (method.isOperation())
        {
            if (NPN_Invoke(proxy->getNPP(), proxy->getNPObject(), id, variantArray, variantCount, &result))
            {
                return convertToAny(proxy->getNPP(), &result);
            }
        }
        else if (method.isConstructor())
        {
            // Chrome uses NPN_Construct()
            if (NPN_Construct(proxy->getNPP(), proxy->getNPObject(), variantArray, variantCount, &result))
            {
                return convertToAny(proxy->getNPP(), &result);
            }
            // Firefox uses NPN_InvokeDefault()
            if (NPN_InvokeDefault(proxy->getNPP(), proxy->getNPObject(), variantArray, variantCount, &result))
            {
                return convertToAny(proxy->getNPP(), &result);
            }
        }
    }
    return Any();
}
