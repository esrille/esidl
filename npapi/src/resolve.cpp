/*
 * Copyright 2008-2010 Google Inc.
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

namespace
{

NPObject* stubAllocate(NPP npp, NPClass* aClass)
{
    return new StubObject(npp);
}

void stubDeallocate(NPObject* object)
{
    delete static_cast<StubObject*>(object);
}

void stubInvalidate(NPObject* object)
{
    return static_cast<StubObject*>(object)->invalidate();
}

bool stubHasMethod(NPObject* object, NPIdentifier name)
{
    return static_cast<StubObject*>(object)->hasMethod(name);
}

bool stubInvoke(NPObject* object, NPIdentifier name, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
    return static_cast<StubObject*>(object)->invoke(name, args, argCount, result);
}

bool stubInvokeDefault(NPObject* object, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
    return static_cast<StubObject*>(object)->invokeDefault(args, argCount, result);
}

bool stubHasProperty(NPObject* object, NPIdentifier name)
{
    return static_cast<StubObject*>(object)->hasProperty(name);
}

bool stubGetProperty(NPObject* object, NPIdentifier name, NPVariant* result)
{
    return static_cast<StubObject*>(object)->getProperty(name, result);
}

bool stubSetProperty(NPObject* object, NPIdentifier name, const NPVariant* value)
{
    return static_cast<StubObject*>(object)->setProperty(name, value);
}

bool stubRemoveProperty(NPObject* object, NPIdentifier name)
{
    return static_cast<StubObject*>(object)->removeProperty(name);
}

bool stubEnumeration(NPObject* object, NPIdentifier** value, uint32_t* count)
{
    return static_cast<StubObject*>(object)->enumeration(value, count);
}

bool stubConstruct(NPObject* object, const NPVariant* args, uint32_t argCount, NPVariant* result)
{
    return static_cast<StubObject*>(object)->construct(args, argCount, result);
}

unsigned lookupSymbolTalbe(Object* object, const char* identifier, unsigned& interfaceNumber, unsigned& symbolNumber)
{
    const Reflect::SymbolData* symbolTable;
    while (symbolTable = object->getSymbolTable(interfaceNumber))
    {
        unsigned offset = 0;
        for (symbolTable += symbolNumber; symbolTable->symbol; ++symbolNumber, ++symbolTable)
        {
            if (!std::strcmp(symbolTable->symbol, identifier))
            {
                return symbolTable->offset;
            }
        }
        ++interfaceNumber;
    }
    return 0;
}

}   // namespace

void StubObject::invalidate()
{
}

bool StubObject::hasMethod(NPIdentifier name)
{
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        unsigned offset = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!offset)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += offset;
        if (*metaData == Reflect::kOperation)
        {
            found = true;
            break;
        }
    }
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::invoke(NPIdentifier name, const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    return false;
}

bool StubObject::invokeDefault(const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    return false;
}

bool StubObject::hasProperty(NPIdentifier name)
{
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        unsigned offset = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!offset)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += offset;
        if (*metaData == Reflect::kConstant || *metaData == Reflect::kGetter || *metaData == Reflect::kSetter)
        {
            found = true;
            break;
        }
    }
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::getProperty(NPIdentifier name, NPVariant* result)
{
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        unsigned offset = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!offset)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += offset;
        if (*metaData == Reflect::kConstant)
        {
            // TODO: eval the constant value
            found = true;
            break;
        }
        if (*metaData == Reflect::kGetter)
        {
            Any property = object->call(interfaceNumber, symbolNumber, 0, 0);
            convertToVariant(npp, property, result);
            found = true;
            break;
        }
    }
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::setProperty(NPIdentifier name, const NPVariant* value)
{
    return false;
}

bool StubObject::removeProperty(NPIdentifier name)
{
    return false;
}

bool StubObject::enumeration(NPIdentifier** value, uint32_t* count)
{
    return false;
}

bool StubObject::construct(const NPVariant* args, uint32_t argCount, NPVariant* result)
{
    return false;
}

NPClass StubObject::npclass =
{
    NP_CLASS_STRUCT_VERSION_CTOR,
    stubAllocate,
    stubDeallocate,
    stubInvalidate,
    stubHasMethod,
    stubInvoke,
    stubInvokeDefault,
    stubHasProperty,
    stubGetProperty,
    stubSetProperty,
    stubRemoveProperty,
    stubEnumeration,
    stubConstruct
};
