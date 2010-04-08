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

#include <esnpapi.h>

#include <map>

namespace
{
    // Map from interfaceName to constructors.
    std::map<const std::string, Object* (*)(ProxyObject object)> proxyConstructorMap;
};

void registerMetaData(const char* meta,
                      Object* (*createProxy)(ProxyObject object),
                      const char* alias)
{
    Reflect::Interface interface(meta);
    std::string name = interface.getName();
    if (alias)
    {
        name = alias;
    }
    proxyConstructorMap[name] = createProxy;
    printf("%s\n", name.c_str());
}

Object* createProxy(NPP npp, NPObject* object)
{
    if (!object)
    {
        return 0;
    }

    std::string className = getInterfaceName(npp, object);
    if (className == "Function" || className == "Object")
    {
        // TODO: We should define 'Object' interface
        return 0;
    }

    std::map<const std::string, Object* (*)(ProxyObject object)>::iterator it;
    it = proxyConstructorMap.find(className);
    if (it != proxyConstructorMap.end())
    {
        ProxyObject browserObject(object, npp);
        return (*it).second(browserObject);
    }
    return 0;
}

ProxyObject::ProxyObject(NPObject* object, NPP npp) :
    object(object),
    npp(npp),
    count(1)
{
}

ProxyObject::ProxyObject(const ProxyObject& original) :
    object(original.object),
    npp(original.npp),
    count(1)
{
}

ProxyObject::~ProxyObject()
{
}

unsigned int ProxyObject::retain()
{
    NPN_RetainObject(object);
    return ++count;
}

unsigned int ProxyObject::release()
{
    NPN_ReleaseObject(object);
    return --count;
}
