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

#ifndef ESNPAPI_H_INCLUDED
#define ESNPAPI_H_INCLUDED

#include <cstring>
#include <string>

using std::strlen;  // for STRINGZ_TO_NPVARIANT

#include "npapi/npapi.h"
#include "npapi/npruntime.h"

#include "object.h"
#include "any.h"
#include "reflect.h"

class ProxyObject
{
    NPObject*         object;
    NPP               npp;
    const std::string interfaceName;
    unsigned int      count;
public:
    ProxyObject(NPObject* object, NPP npp, const std::string& interfaceName) :
        object(object),
        npp(npp),
        interfaceName(interfaceName),
        count(1)
    {
    }
    ProxyObject(const ProxyObject& original) :
        object(original.object),
        npp(original.npp),
        interfaceName(original.interfaceName),
        count(1)
    {
    }
    ~ProxyObject()
    {
    }
    NPObject* getNPObject()
    {
        return object;
    }
    NPP getNPP()
    {
        return npp;
    }
    std::string getInterfaceName()
    {
        return interfaceName;
    }
    unsigned int retain()
    {
        NPN_RetainObject(object);
        return ++count;
    }
    unsigned int release()
    {
        NPN_ReleaseObject(object);
        return --count;
    }
};

class StubObject : public NPObject
{
public:
    virtual Object* getObject() const = 0;
    virtual void invalidate() = 0;
    virtual bool hasMethod(NPIdentifier name) = 0;
    virtual bool invoke(NPIdentifier name, const NPVariant* args, uint32_t arg_count, NPVariant* result) = 0;
    virtual bool invokeDefault(const NPVariant* args, uint32_t arg_count, NPVariant* result) = 0;
    virtual bool hasProperty(NPIdentifier name) = 0;
    virtual bool getProperty(NPIdentifier name, NPVariant* result) = 0;
    virtual bool setProperty(NPIdentifier name, const NPVariant* value) = 0;
    virtual bool removeProperty(NPIdentifier name) = 0;

    static void Deallocate(NPObject* object)
    {
        delete static_cast<StubObject*>(object);
    }
    static void Invalidate(NPObject* object)
    {
        static_cast<StubObject*>(object)->invalidate();
    }
    static bool HasMethod(NPObject* object, NPIdentifier name)
    {
        return static_cast<StubObject*>(object)->hasMethod(name);
    }
    static bool Invoke(NPObject* object, NPIdentifier name, const NPVariant* args, uint32_t arg_count, NPVariant* result)
    {
        return static_cast<StubObject*>(object)->invoke(name, args, arg_count, result);
    }
    static bool InvokeDefault(NPObject* object, const NPVariant* args, uint32_t arg_count, NPVariant* result)
    {
        return static_cast<StubObject*>(object)->invokeDefault(args, arg_count, result);
    }
    static bool HasProperty(NPObject* object, NPIdentifier name)
    {
        return static_cast<StubObject*>(object)->hasProperty(name);
    }
    static bool GetProperty(NPObject* object, NPIdentifier name, NPVariant* result)
    {
        return static_cast<StubObject*>(object)->getProperty(name, result);
    }
    static bool SetProperty(NPObject* object, NPIdentifier name, const NPVariant* value)
    {
        return static_cast<StubObject*>(object)->setProperty(name, value);
    }
    static bool RemoveProperty(NPObject* object, NPIdentifier name)
    {
        return static_cast<StubObject*>(object)->removeProperty(name);
    }
};

std::string getInterfaceName(NPP npp, NPObject* object);

void convertToVariant(NPP npp, bool value, NPVariant* variant);
void convertToVariant(NPP npp, uint8_t value, NPVariant* variant);
void convertToVariant(NPP npp, int16_t value, NPVariant* variant);
void convertToVariant(NPP npp, uint16_t value, NPVariant* variant);
void convertToVariant(NPP npp, int32_t value, NPVariant* variant);
void convertToVariant(NPP npp, uint32_t value, NPVariant* variant);
void convertToVariant(NPP npp, int value, NPVariant* variant);
void convertToVariant(NPP npp, unsigned int value, NPVariant* variant);
void convertToVariant(NPP npp, int64_t value, NPVariant* variant);
void convertToVariant(NPP npp, uint64_t value, NPVariant* variant);
void convertToVariant(NPP npp, double value, NPVariant* variant);
void convertToVariant(NPP npp, const std::string& value, NPVariant* variant);
void convertToVariant(NPP npp, Object* value, NPVariant* variant);
void convertToVariant(NPP npp, const Any& any, NPVariant* variant);

bool convertToBool(const NPVariant* variant);
uint8_t convertToOctet(const NPVariant* variant);
int16_t convertToShort(const NPVariant* variant);
uint16_t convertToUnsignedShort(const NPVariant* variant);
int32_t convertToLong(const NPVariant* variant);
uint32_t convertToUnsignedLong(const NPVariant* variant);
int64_t convertToLongLong(const NPVariant* variant);
uint64_t convertToUnsignedLongLong(const NPVariant* variant);
float convertToFloat(const NPVariant* variant);
double convertToDouble(const NPVariant* variant);
std::string convertToString(NPP npp, const NPVariant* variant, unsigned attribute = 0);
Object* convertToObject(NPP npp, const NPVariant* variant, char* buffer, size_t length);
Any convertToAny(NPP npp, const NPVariant* variant, void* buffer, size_t length);
Any convertToAny(NPP npp, const NPVariant* variant, int type, void* buffer, size_t length);

void addProxyConstructor(const std::string interfaceName, Object* (*createProxy)(ProxyObject object));
void addStubConstructor(const std::string interfaceName, NPObject* (*createStub)(NPP npp, Object* object));

NPObject* createStub(NPP npp, const char* interfaceName, Object* object);
Object* createProxy(NPP npp, NPObject* object);

Any invoke(Object* object, const char* iid, unsigned baseNumber, unsigned methodNumber, unsigned paramCount, Any* paramArray);

void addInterfaceData(const char* iid, const char* info);
Reflect::Interface* getInterfaceData(const std::string interfaceName);
Reflect::Interface* getInterfaceData(const char* iid);

#endif  // ESIDL_ESNPAPI_H_INCLUDED
