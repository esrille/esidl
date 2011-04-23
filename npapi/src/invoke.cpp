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

#include <stdio.h>

#include "esnpapi.h"

namespace
{

NPIdentifier getSpecialIdentifier(const Any& any)
{
    NPIdentifier id;

    if (any.isString())
    {
        id = NPN_GetStringIdentifier(any.toString().c_str());
    }
    else
    {
        id = NPN_GetIntIdentifier(static_cast<uint32_t>(any));
    }
    return id;
}

Any processResult(NPP npp, NPVariant* variant)
{
    Any result = convertToAny(npp, variant);
    if (NPVARIANT_IS_OBJECT(*variant))
    {
        if (!result.isObject())
        {
            NPN_ReleaseObject(NPVARIANT_TO_OBJECT(*variant));
        }
    }
    else
    {
        NPN_ReleaseVariantValue(variant);
    }
    return result;
}

}  // namespace

Any ProxyObject::message_(uint32_t selector, const char* name, int argumentCount, Any* arguments)
{
    printf("message_(%s, %d, ...)\n", name, argumentCount);

    NPIdentifier id;
    NPVariant result;
    VOID_TO_NPVARIANT(result);

    if (argumentCount == IS_KIND_OF_)
    {
        std::string interfaceName = getInterfaceName(getNPP(), getNPObject());
        if (const char* id = strrchr(name, ':'))
            name = id + 1;
        return interfaceName.compare(name) == 0;
    }
    else if (argumentCount == GETTER_)
    {
        id = NPN_GetStringIdentifier(name);
        if (NPN_GetProperty(getNPP(), getNPObject(), id, &result))
        {
            return processResult(getNPP(), &result);
        }
    }
    else if (argumentCount == SETTER_)
    {
        id = NPN_GetStringIdentifier(name);
        NPVariant value;
        convertToVariant(getNPP(), arguments[0], &value, false);
        NPN_SetProperty(getNPP(), getNPObject(), id, &value);
    }
    else if (argumentCount == SPECIAL_GETTER_)
    {
        id = getSpecialIdentifier(arguments[0]);
        if (NPN_GetProperty(getNPP(), getNPObject(), id, &result))
        {
            return processResult(getNPP(), &result);
        }
    }
    else if (argumentCount == SPECIAL_SETTER_ || argumentCount == SPECIAL_CREATOR_ || argumentCount == SPECIAL_SETTER_CREATOR_)
    {
        id = getSpecialIdentifier(arguments[0]);
        NPVariant value;
        convertToVariant(getNPP(), arguments[1], &value, false);
        NPN_SetProperty(getNPP(), getNPObject(), id, &value);
    }
    else if (argumentCount == SPECIAL_DELETER_)
    {
        id = getSpecialIdentifier(arguments[0]);
        NPN_RemoveProperty(getNPP(), getNPObject(), id);
    }
    else
    {
        NPVariant variantArray[argumentCount];
        for (int i = 0; i < argumentCount; ++i)
        {
            convertToVariant(getNPP(), arguments[i], &variantArray[i], false);
        }
        id = NPN_GetStringIdentifier(name);
        if (selector)
        {
            if (NPN_Invoke(getNPP(), getNPObject(), id, variantArray, argumentCount, &result))
            {
                return processResult(getNPP(), &result);
            }
        }
        else // [Constructor]
        {
            // Chrome uses NPN_Construct()
            if (NPN_Construct(getNPP(), getNPObject(), variantArray, argumentCount, &result))
            {
                return processResult(getNPP(), &result);
            }
            // Firefox uses NPN_InvokeDefault()
            if (NPN_InvokeDefault(getNPP(), getNPObject(), variantArray, argumentCount, &result))
            {
                return processResult(getNPP(), &result);
            }
        }
    }
    return Any();
}
