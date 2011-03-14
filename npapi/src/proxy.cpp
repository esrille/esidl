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

#include <esnpapi.h>

#include <assert.h>
#include <stdarg.h>

PluginInstance* currentPluginInstance;

ProxyControl::ProxyControl(NPP npp) :
    npp(npp)
{
}

ProxyControl::~ProxyControl()
{
    assert(proxyMap.size() == 0);
}

ProxyObject* ProxyControl::findProxy(NPObject* object)
{
    std::map<NPObject*, ProxyObject*>::iterator i;
    i = proxyMap.find(object);
    if (i != proxyMap.end())
    {
        return i->second;
    }
    return 0;
}

ProxyObject* ProxyControl::createProxy(NPObject* object)
{
    ProxyObject* proxy = findProxy(object);
    if (proxy)
    {
        return proxy;
    }
    proxy = new(std::nothrow) ProxyObject(object, npp);
    if (!proxy)
    {
        return 0;
    }
    proxyMap.insert(std::pair<NPObject*, ProxyObject*>(object, proxy));
    return proxy;
}

void ProxyControl::remove(NPObject* object)
{
    proxyMap.erase(object);
}

ProxyObject::ProxyObject(NPObject* object, NPP npp) :
    Object(this),
    object(object),
    npp(npp),
    count(0)
{
    assert(object);
}

ProxyObject::~ProxyObject()
{
    PluginInstance* instance = static_cast<PluginInstance*>(npp->pdata);
    if (!instance)
    {
        return;
    }
    ProxyControl* proxyControl = instance->getProxyControl();
    assert(proxyControl);
    proxyControl->remove(object);
}

unsigned int ProxyObject::retain_()
{
    if (count == 0)
        NPN_RetainObject(object);
    return ++count;
}

unsigned int ProxyObject::release_()
{
    if (0 < count)
    {
        if (count == 1)
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

PluginInstance::PluginInstance(NPP npp, NPObject* window) :
    proxyControl(npp),
    stubControl(npp),
    window(proxyControl.createProxy(window))
{
    npp->pdata = this;
    currentPluginInstance = this;
}

PluginInstance::~PluginInstance()
{
    currentPluginInstance = 0;
}
