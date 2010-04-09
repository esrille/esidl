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
#include <org/w3c/dom.h>

#include <assert.h>

std::map<const std::string, Object* (*)(ProxyObject object)> ProxyControl::proxyConstructorMap;

ProxyControl::ProxyControl(NPP npp) :
    npp(npp),
    nestingCount(1)
{
}

ProxyControl::~ProxyControl()
{
    // TODO: Release objects in newList and oldList
}

Object* ProxyControl::createProxy(NPObject* object)
{
    if (!object)
    {
        return 0;
    }

    std::string className = getInterfaceName(npp, object);
    if (className == "Object")
    {
        // TODO: We should define 'Object' interface
        return 0;
    }

    std::map<const std::string, Object* (*)(ProxyObject object)>::iterator it;
    it = proxyConstructorMap.find(className);
    if (it != proxyConstructorMap.end())
    {
        ProxyObject browserObject(object, npp);
        if (Object* object = (*it).second(browserObject))
        {
            return track(object);
        }
    }
    return 0;
}

long ProxyControl::enter()
{
    return ++nestingCount;
}

long ProxyControl::leave()
{
    --nestingCount;
    assert(0 <= nestingCount);
    if (nestingCount == 0)
    {
        while (!newList.empty())
        {
            Object* object = newList.front();
            newList.pop_front();
            if (0 < object->release())
            {
                oldList.push_back(object);
            }
        }
    }
    return nestingCount;
}

void ProxyControl::registerMetaData(const char* meta, Object* (*createProxy)(ProxyObject object), const char* alias)
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

ProxyObject::ProxyObject(NPObject* object, NPP npp) :
    object(object),
    npp(npp),
    count(0)
{
}

ProxyObject::ProxyObject(const ProxyObject& original) :
    object(original.object),
    npp(original.npp),
    count(original.count)
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
    if (0 < count)
    {
        NPN_ReleaseObject(object);
        --count;
    }
    if (count == 0)
    {
        delete this;
        return 0;
    }
    return count;
}

unsigned int ProxyObject::mark()
{
    return ++count;
}

PluginInstance::PluginInstance(NPP npp, NPObject* window) :
    proxyControl(npp),
    window(0)
{
    npp->pdata = this;
    this->window = interface_cast<org::w3c::dom::html::Window*>(proxyControl.createProxy(window));
    if (this->window)
    {
        ProxyObject* proxy = interface_cast<ProxyObject*>(this->window);
        proxy->mark();
        proxy->retain();
    }
}

PluginInstance::~PluginInstance()
{
    if (window)
    {
        window->release();
    }
}

