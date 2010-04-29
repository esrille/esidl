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

#include <stdio.h>
#include <string.h>
#include "esnpapi.h"

namespace
{

NPObject* stubAllocate(NPP npp, NPClass* aClass)
{
    return new StubObject(npp);
}

void stubDeallocate(NPObject* object)
{
    static_cast<StubObject*>(object)->deallocate();
}

void stubInvalidate(NPObject* object)
{
    static_cast<StubObject*>(object)->invalidate();
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

const Reflect::SymbolData* lookupSymbolTalbe(Object* object, const char* identifier, unsigned& interfaceNumber, unsigned& symbolNumber)
{
    const Reflect::SymbolData* symbolTable;
    while (symbolTable = object->getSymbolTable(interfaceNumber))
    {
        unsigned offset = 0;
        for (symbolTable += symbolNumber; symbolTable->symbol; ++symbolNumber, ++symbolTable)
        {
            if (!std::strcmp(symbolTable->symbol, identifier))
            {
                return symbolTable;
            }
        }
        ++interfaceNumber;
    }
    return 0;
}

template <typename T>
Any convertToSequence(NPP npp, const NPVariant* args, unsigned count, const Reflect::Type type)
{
    Sequence<T> sequence(count);
    for (unsigned i = 0; i < count; ++i)
    {
        Any value = convertToAny(npp, args++, type);
        sequence.setElement(i, static_cast<T>(value));
    }
    return sequence;
}

void processResult(NPP npp, const Any& any, NPVariant* variant)
{
    convertToVariant(npp, any, variant, true);
}

}   // namespace

long StubObject::enter()
{
    PluginInstance* instance = static_cast<PluginInstance*>(npp->pdata);
    if (instance)
    {
        ProxyControl* proxyControl = instance->getProxyControl();
        if (proxyControl)
        {
            return proxyControl->enter();
        }
    }
    return 0;
}

long StubObject::leave()
{
    PluginInstance* instance = static_cast<PluginInstance*>(npp->pdata);
    if (instance)
    {
        ProxyControl* proxyControl = instance->getProxyControl();
        if (proxyControl)
        {
            return proxyControl->leave();
        }
    }
    return 0;
}

// Note deallocate() can be invoked after the plugin instance is destroyed.
void StubObject::deallocate()
{
    printf("%s()\n", __func__);
    PluginInstance* instance = static_cast<PluginInstance*>(npp->pdata);
    if (instance)
    {
        StubControl* stubControl = instance->getStubControl();
        if (stubControl)
        {
            stubControl->remove(object);
        }
    }
    delete object;
    delete this;
}

void StubObject::invalidate()
{
    printf("%s()\n", __func__);
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
        const Reflect::SymbolData* data = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!data)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += data->offset;
        if (*metaData == Reflect::kOperation)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        if (!std::strcmp(identifier, "toString"))
        {
            found = true;
        }
    }
    printf("%s(%s) : %d\n", __func__, identifier, found);
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::call(unsigned interfaceNumber, const Reflect::SymbolData* data, const char* metaData,
                      const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    Reflect::Method method(metaData);
    unsigned argumentCount = method.getParameterCount();
    if (argumentCount != arg_count && (!method.isVariadic() || arg_count + 1 < argumentCount))
    {
        return false;
    }

    enter();
    Any arguments[argumentCount];
    if (method.isVariadic())
    {
        --argumentCount;
    }
    Reflect::Parameter parameter = method.listParameter();
    for (unsigned i = 0; i < argumentCount; ++i)
    {
        parameter.next();
        arguments[i] = convertToAny(npp, &args[i], parameter.getType());
    }
    if (method.isVariadic())
    {
        Reflect::Parameter param = method.listParameter();
        for (int i = 0; i < method.getParameterCount(); ++i)
        {
            param.next();
        }
        arg_count -= argumentCount;
        Reflect::Type type = param.getType();
        if (type.isNullable())
        {
            switch (type.getType())
            {
            case Reflect::kBoolean:
                arguments[argumentCount] = convertToSequence<Nullable<bool> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kByte:
                arguments[argumentCount] = convertToSequence<Nullable<int8_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kOctet:
                arguments[argumentCount] = convertToSequence<Nullable<uint8_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kShort:
                arguments[argumentCount] = convertToSequence<Nullable<int16_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kUnsignedShort:
                arguments[argumentCount] = convertToSequence<Nullable<uint16_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kLong:
                arguments[argumentCount] = convertToSequence<Nullable<int32_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kUnsignedLong:
                arguments[argumentCount] = convertToSequence<Nullable<uint32_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kLongLong:
                arguments[argumentCount] = convertToSequence<Nullable<int64_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kUnsignedLongLong:
                arguments[argumentCount] = convertToSequence<Nullable<uint64_t> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kFloat:
                arguments[argumentCount] = convertToSequence<Nullable<float> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kDouble:
                arguments[argumentCount] = convertToSequence<Nullable<double> >(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kString:
                arguments[argumentCount] = convertToSequence<Nullable<std::string> >(npp, &args[argumentCount], arg_count, type);
                break;
            }
        }
        else
        {
            switch (type.getType())
            {
            case Reflect::kBoolean:
                arguments[argumentCount] = convertToSequence<bool>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kByte:
                arguments[argumentCount] = convertToSequence<int8_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kOctet:
                arguments[argumentCount] = convertToSequence<uint8_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kShort:
                arguments[argumentCount] = convertToSequence<int16_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kUnsignedShort:
                arguments[argumentCount] = convertToSequence<uint16_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kLong:
                arguments[argumentCount] = convertToSequence<int32_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kUnsignedLong:
                arguments[argumentCount] = convertToSequence<uint32_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kLongLong:
                arguments[argumentCount] = convertToSequence<int64_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kUnsignedLongLong:
                arguments[argumentCount] = convertToSequence<uint64_t>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kFloat:
                arguments[argumentCount] = convertToSequence<float>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kDouble:
                arguments[argumentCount] = convertToSequence<double>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kString:
                arguments[argumentCount] = convertToSequence<std::string>(npp, &args[argumentCount], arg_count, type);
                break;
            case Reflect::kAny:
                arguments[argumentCount] = convertToSequence<Any>(npp, &args[argumentCount], arg_count, type);
                break;
            // TODO: kSequence???
            case Reflect::kObject:
            default:
                arguments[argumentCount] = convertToSequence<Object*>(npp, &args[argumentCount], arg_count, type);
                break;
            }
        }
        ++argumentCount;
    }
    Any value = object->call(interfaceNumber, data->number, argumentCount, arguments);
    processResult(npp, value, result);
    leave();
    return true;
}

bool StubObject::invoke(NPIdentifier name, const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    printf("%s(%s, %p, %u, %p)\n", __func__, identifier, args, arg_count, result);
    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        const Reflect::SymbolData* data = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!data)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += data->offset;
        if (*metaData == Reflect::kOperation &&
            (found = call(interfaceNumber, data, metaData, args, arg_count, result)))
        {
            break;
        }
    }
    if (!found)
    {
        if (!std::strcmp(identifier, "toString"))
        {
            const char* metaData = object->getMetaData(0);
            unsigned length;
            const char* qualifiedName = Reflect::skipDigits(metaData + 1, &length);
            std::string name(qualifiedName, length);
            size_t pos = name.rfind(':');
            name = name.substr(pos + 1);
            if (12 < name.length() && !name.compare(name.length() - 12, 12, "_Constructor"))
            {
                name = name.substr(0, name.length() - 12);
                name = "function " + name + "() { [native code] }";
            }
            else
            {
                name = "[object " + name + "]";
            }
            Any value(name);
            processResult(npp, value, result);
            found = true;
        }
    }
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::invokeDefault(const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    printf("%s()\n", __func__);
    return false;
}

bool StubObject::hasProperty(NPIdentifier name)
{
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    printf("%s(%s)\n", __func__, identifier);

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        const Reflect::SymbolData* data = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!data)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += data->offset;
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

    printf("%s(%s)\n", __func__, identifier);

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        const Reflect::SymbolData* data = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!data)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += data->offset;
        if (*metaData == Reflect::kConstant)
        {
            Reflect::Constant constant(metaData);
            convertToVariant(npp, constant.getValue(), result, true);
            found = true;
            break;
        }
        if (*metaData == Reflect::kGetter)
        {
            enter();
            Any property = object->call(interfaceNumber, data->number, 0, 0);
            processResult(npp, property, result);
            found = true;
            leave();
            break;
        }
    }
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::setProperty(NPIdentifier name, const NPVariant* value)
{
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    printf("%s(%s)\n", __func__, identifier);

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        const Reflect::SymbolData* data = lookupSymbolTalbe(object, identifier, interfaceNumber, symbolNumber);
        if (!data)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += data->offset;
        if (*metaData == Reflect::kSetter)
        {
            Reflect::Method method(metaData);
            Reflect::Parameter parameter = method.listParameter();
            parameter.next();
            enter();
            Any argument = convertToAny(npp, value, parameter.getType());
            object->call(interfaceNumber, data->number, 1, &argument);
            found = true;
            leave();
            break;
        }
    }
    NPN_MemFree(identifier);
    return found;
}

bool StubObject::removeProperty(NPIdentifier name)
{
    return false;
}

bool StubObject::enumeration(NPIdentifier** value, uint32_t* count)
{
    return false;
}

bool StubObject::construct(const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    printf("%s(%p, %u, %p)\n", __func__, args, arg_count, result);

    bool found = false;
    unsigned interfaceNumber = 0;
    unsigned symbolNumber = 0;
    for (;; ++symbolNumber)
    {
        const Reflect::SymbolData* data = lookupSymbolTalbe(object, "createInstance", interfaceNumber, symbolNumber);
        if (!data)
        {
            break;
        }
        const char* metaData = object->getMetaData(interfaceNumber);
        metaData += data->offset;
        if (*metaData == Reflect::kConstructor &&
            (found = call(interfaceNumber, data, metaData, args, arg_count, result)))
        {
            break;
        }
    }
    return found;
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

StubControl::StubControl(NPP npp) :
    npp(npp)
{
}

StubControl::~StubControl()
{
}

NPObject* StubControl::findStub(Object* object)
{
    std::map<Object*, StubObject*>::iterator i;
    i = stubMap.find(object);
    if (i != stubMap.end())
    {
        return i->second;
    }
    return 0;
}

NPObject* StubControl::createStub(Object* object)
{
    NPObject* npobject = findStub(object);
    if (npobject)
    {
        return npobject;
    }
    npobject = NPN_CreateObject(npp, &StubObject::npclass);
    if (!npobject)
    {
        return 0;
    }
    StubObject* stub = static_cast<StubObject*>(npobject);
    stub->setObject(object);
    stubMap.insert(std::pair<Object*, StubObject*>(object, stub));
    return stub;
}

void StubControl::remove(Object* object)
{
    stubMap.erase(object);
}

unsigned int PluginInstance::retain(Object* stub)
{
    NPObject* npobject = stubControl.findStub(stub);
    if (!npobject)
    {
        return 1;
    }
    NPN_RetainObject(npobject);
    return npobject->referenceCount;
}

unsigned int PluginInstance::release(Object* stub)
{
    NPObject* npobject = stubControl.findStub(stub);
    if (!npobject)
    {
        delete stub;
        return 0;
    }
    unsigned int count = npobject->referenceCount;
    NPN_ReleaseObject(npobject);
    return --count;
}
