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
using std::strlen;  // for STRINGZ_TO_NPVARIANT

#include <list>
#include <map>
#include <string>

#ifdef __native_client__
#include "nacl/npapi.h"
#include "nacl/npruntime.h"
#define utf8characters UTF8Characters
#define utf8length     UTF8Length
#else
#include "npapi/npapi.h"
#include "npapi/npruntime.h"
#endif

#include "object.h"
#include "any.h"
#include "reflect.h"

namespace org {
namespace w3c {
namespace dom {
namespace html {
class Window;
}
}
}
}

class ProxyObject
{
    friend class ProxyControl;
    
    NPObject*    object;
    NPP          npp;
    unsigned int count;
    unsigned int age;

    // age constants
    static const unsigned int CREATED = 0;
    static const unsigned int NEW = 1;
    static const unsigned int OLD = 2;

public:
    ProxyObject(NPObject* object, NPP npp);
    ProxyObject(const ProxyObject& original);
    virtual ~ProxyObject();

    unsigned int retain();
    unsigned int release();
    unsigned int mark();
    void invalidate();

    NPObject* getNPObject()
    {
        return object;
    }
    NPP getNPP()
    {
        return npp;
    }

    static const char* const getQualifiedName()
    {
        static const char* qualifiedName = "ProxyObject";
        return qualifiedName;
    }
};

class ProxyControl
{
    NPP npp;
    long nestingCount;  // more than zero if the control is in plugin module

    std::list<ProxyObject*> oldList;
    std::list<ProxyObject*> newList;

    // Map from interfaceName to constructors.
    static std::map<const std::string, Object* (*)(ProxyObject object)> proxyConstructorMap;

public:
    explicit ProxyControl(NPP npp);
    ~ProxyControl();

    Object* createProxy(NPObject* object, const Reflect::Type type);

    long enter();
    long leave();
    void track(ProxyObject* proxy);
    void untrack(ProxyObject* proxy);

    static void registerMetaData(const char* meta, Object* (*createProxy)(ProxyObject object), const char* alias = 0);
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

    long enter();
    long leave();

    void deallocate();
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

    static bool isStub(const NPObject* object)
    {
        return object->_class == &StubObject::npclass;
    }

    static NPClass npclass;
};

class StubControl
{
    NPP npp;
    std::map<Object*, StubObject*> stubMap;

public:
    explicit StubControl(NPP npp);
    ~StubControl();

    NPObject* findStub(Object* object);
    NPObject* createStub(Object* object);
    void remove(Object* object);
};

class PluginInstance
{
    ProxyControl proxyControl;
    StubControl stubControl;

protected:
    org::w3c::dom::html::Window* window;

public:
    PluginInstance(NPP npp, NPObject* window);
    virtual ~PluginInstance();

    virtual NPObject* getScriptableInstance()
    {
        return 0;
    }

    ProxyControl* getProxyControl()
    {
        return &proxyControl;
    }

    StubControl* getStubControl()
    {
        return &stubControl;
    }

    unsigned int retain(Object* stub);
    unsigned int release(Object* stub);
};

std::string getInterfaceName(NPP npp, NPObject* object);

void convertToVariant(NPP npp, bool value, NPVariant* variant);
void convertToVariant(NPP npp, int8_t value, NPVariant* variant);
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
int8_t convertToByte(const NPVariant* variant);
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
Object* convertToObject(NPP npp, const NPVariant* variant, const Reflect::Type type = Reflect::Type("v"));
Any convertToAny(NPP npp, const NPVariant* variant);
Any convertToAny(NPP npp, const NPVariant* variant, const Reflect::Type type);

Any invoke(Object* object, unsigned interfaceNumber, unsigned methodNumber,
           const char* meta, unsigned offset,
           unsigned argumentCount, Any* arguments);

// The following four functions are called from initializeHtmlMetaData();
void initializeHtmlMetaDataA_G();
void initializeHtmlMetaDataH_N();
void initializeHtmlMetaDataO_U();
void initializeHtmlMetaDataV_Z();

// The following five functions are called from initializeSvgMetaData();
void initializeSvgMetaDataA_E();
void initializeSvgMetaDataF_G();
void initializeSvgMetaDataH_N();
void initializeSvgMetaDataO_U();
void initializeSvgMetaDataV_Z();

void initializeMetaData();
void initializeFileMetaData();
void initializeGeolocationMetaData();
void initializeHtmlMetaData();
void initializeIndexedDBMetaData();
void initializeSvgMetaData();
void initializeWebDatabaseMetaData();
void initializeWebGLMetaData();
void initializeWorkersMetaData();

extern "C" NPObject* NPP_GetScriptableInstance(NPP instance);

#endif  // ESIDL_ESNPAPI_H_INCLUDED
