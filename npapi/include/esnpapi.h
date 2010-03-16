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

#ifndef ESNPAPI_H_INCLUDED
#define ESNPAPI_H_INCLUDED

#include <cstring>
#include <string>

using std::strlen;  // for STRINGZ_TO_NPVARIANT

#include "npapi/npapi.h"
#include "npapi/npruntime.h"

extern "C" NPObject* NPP_GetScriptableInstance(NPP instance);

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
    NPP npp;
    Object* object;

public:
    StubObject(NPP npp) :
        npp(npp),
        object(0)
    {
    }

    Object* getObject() const
    {
        return object;
    }
    void setObject(Object* object)
    {
        this->object = object;
    }

    void invalidate();
    bool hasMethod(NPIdentifier name);
    bool invoke(NPIdentifier name, const NPVariant* args, uint32_t arg_count, NPVariant* result);
    bool invokeDefault(const NPVariant* args, uint32_t arg_count, NPVariant* result);
    bool hasProperty(NPIdentifier name);
    bool getProperty(NPIdentifier name, NPVariant* result);
    bool setProperty(NPIdentifier name, const NPVariant* value);
    bool removeProperty(NPIdentifier name);
    bool enumeration(NPIdentifier** value, uint32_t* count);
    bool construct(const NPVariant* args, uint32_t argCount, NPVariant* result);

    static NPClass npclass;
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
Object* convertToObject(NPP npp, const NPVariant* variant);
Any convertToAny(NPP npp, const NPVariant* variant);
Any convertToAny(NPP npp, const NPVariant* variant, int type);

void addProxyConstructor(const std::string interfaceName, Object* (*createProxy)(ProxyObject object));

NPObject* createStub(NPP npp, Object* object);
Object* createProxy(NPP npp, NPObject* object);

Any invoke(Object* object, unsigned interfaceNumber, unsigned methodNumber,
           const char* meta, unsigned offset,
           unsigned argumentCount, Any* arguments);

void addInterfaceData(const char* mataData, const char* name = 0);
Reflect::Interface* getInterfaceData(const std::string interfaceName);
Reflect::Interface* getInterfaceData(const char* iid);

void registerMetaData(const char* meta,
                      Object* (*createProxy)(ProxyObject object),
                      const char* alias = 0);

#endif  // ESIDL_ESNPAPI_H_INCLUDED
