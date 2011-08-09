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

#include "Object.h"
#include "Any.h"

#include <org/w3c/dom/html/Window.h>

class ObjectImp : public Object
{
    unsigned int count;
protected:
    virtual unsigned int retain_() {
        return ++count;
    };
    virtual unsigned int release_() {
        if (0 < count)
            --count;
        if (count == 0) {
            delete this;
            return 0;
        }
        return count;
    };
public:
    ObjectImp() :
        Object(this),
        count(0) {
    }
};

class ProxyObject : public Object
{
    friend class ProxyControl;

    NPObject*    object;
    NPP          npp;
    unsigned int count;

    ProxyObject(const ProxyObject&);
    ProxyObject(NPObject* object, NPP npp);

protected:
    virtual unsigned int retain_();
    virtual unsigned int release_();

public:
    virtual ~ProxyObject();

    // void invalidate();

    NPObject* getNPObject()
    {
        return object;
    }
    NPP getNPP()
    {
        return npp;
    }

    // Object
    virtual Any message_(uint32_t selector, const char* name, int argc, Any* argv);
};

class ProxyControl
{
    NPP npp;
    std::map<NPObject*, ProxyObject*> proxyMap;

public:
    explicit ProxyControl(NPP npp);
    ~ProxyControl();

    ProxyObject* findProxy(NPObject* object);
    ProxyObject* createProxy(NPObject* object);
    void remove(NPObject* object);
};

class StubObject : public NPObject
{
    NPP npp;
    Object object;

public:
    StubObject(NPP npp) :
        npp(npp),
        object(0)
    {
    }

    Object* getObject()
    {
        return &object;
    }
    void setObject(Object* object)
    {
        this->object = object;
    }

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

    org::w3c::dom::html::Window window;
};

extern PluginInstance* currentPluginInstance;

std::string getInterfaceName(NPP npp, NPObject* object);

void convertToVariant(NPP npp, const Any& any, NPVariant* variant, bool result);
Any convertToAny(NPP npp, const NPVariant* variant);

extern "C" NPObject* NPP_GetScriptableInstance(NPP instance);

#endif  // ESIDL_ESNPAPI_H_INCLUDED
