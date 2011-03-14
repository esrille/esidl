/*
 * Copyright 2011 Esrille Inc.
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

// a public domain hash function - one_at_a_time
// cf. http://burtleburtle.net/bob/hash/doobs.html
inline uint32_t Hash(const char* key)
{
    uint32_t hash, i;
    for (hash = 0, i = 0; key[i]; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

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

}   // namespace

// Note deallocate() can be invoked after the plugin instance is destroyed.
void StubObject::deallocate()
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    printf("%s()\n", __func__);
    if (currentPluginInstance)
    {
        StubControl* stubControl = currentPluginInstance->getStubControl();
        if (stubControl)
        {
            stubControl->remove(&object);
        }
    }
    delete this;
}

void StubObject::invalidate()
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    printf("%s()\n", __func__);
}

bool StubObject::hasMethod(NPIdentifier name)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }
    printf("%s(%s)\n", __func__, identifier);

    Any property = object.self()->message_(Hash(identifier), identifier, Object::HAS_OPERATION_, 0);
    bool result = property.toBoolean();
    NPN_MemFree(identifier);
    return result;
}

bool StubObject::invoke(NPIdentifier name, const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    printf("%s(%s, %p, %u, %p)\n", __func__, identifier, args, arg_count, result);

    Any arguments[arg_count];
    for (unsigned i = 0; i < arg_count; ++i)
    {
        arguments[i] = convertToAny(npp, &args[i]);
    }
    Any value = object.self()->message_(Hash(identifier), identifier, arg_count, arguments);
    convertToVariant(npp, value, result, true);
    NPN_MemFree(identifier);
    return true;  // XXX check undefined
}

bool StubObject::invokeDefault(const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    printf("%s()\n", __func__);
    return false;
}

bool StubObject::hasProperty(NPIdentifier name)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }
    printf("%s(%s)\n", __func__, identifier);

    Any property = object.self()->message_(Hash(identifier), identifier, Object::HAS_PROPERTY_, 0);
    bool result = property.toBoolean();
    NPN_MemFree(identifier);
    return result;
}

bool StubObject::getProperty(NPIdentifier name, NPVariant* result)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }
    printf("%s(%s)\n", __func__, identifier);

    Any property = object.self()->message_(Hash(identifier), identifier, Object::GETTER_, 0);
    convertToVariant(npp, property, result, true);
    NPN_MemFree(identifier);
    return true;  // XXX check undefined
}

bool StubObject::setProperty(NPIdentifier name, const NPVariant* value)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    NPUTF8* identifier = NPN_UTF8FromIdentifier(name);
    if (!identifier)
    {
        return false;
    }

    printf("%s(%s)\n", __func__, identifier);

    Any argument;
    argument = convertToAny(npp, value);
    object.self()->message_(0x957e2539, "setLength", Object::SETTER_, &argument);
    NPN_MemFree(identifier);
    return true;  // XXX check undefined
}

bool StubObject::removeProperty(NPIdentifier name)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    return false;
}

bool StubObject::enumeration(NPIdentifier** value, uint32_t* count)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    return false;
}

bool StubObject::construct(const NPVariant* args, uint32_t arg_count, NPVariant* result)
{
    currentPluginInstance = static_cast<PluginInstance*>(npp->pdata);
    printf("%s(%p, %u, %p)\n", __func__, args, arg_count, result);

    Any arguments[arg_count];
    for (unsigned i = 0; i < arg_count; ++i)
    {
        arguments[i] = convertToAny(npp, &args[i]);
    }
    Any value = object.self()->message_(0, "", arg_count, arguments);
    convertToVariant(npp, value, result, true);
    return true;  // XXX check undefined
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