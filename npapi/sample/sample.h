/*
 * Copyright 2009-2010 Google Inc.
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

#ifndef ESNPAPI_SAMPLE_H_INCLUDED
#define ESNPAPI_SAMPLE_H_INCLUDED

#include "esnpapi.h"

#include <any.h>
#include <reflect.h>
#include <org/w3c/dom.h>

class EventHandler : public org::w3c::dom::events::EventListener
{
    void (*handler)(org::w3c::dom::events::Event* evt);

public:
    EventHandler(void (*handler)(org::w3c::dom::events::Event* evt)) :
        handler(handler)
    {
    }
    virtual void handleEvent(org::w3c::dom::events::Event* evt)
    {
        handler(evt);
    }

    unsigned int retain() {};
    unsigned int release() {};
};

class PluginInstance
{
    org::w3c::dom::html::Window* window;

    EventHandler* downHandler;

    void initialize();
    void drawCharts(org::w3c::dom::Document* document);

public:
    PluginInstance(org::w3c::dom::html::Window* window) :
        window(window),
        downHandler(0)
    {
        initialize();
    }

    ~PluginInstance()
    {
        if (downHandler)
        {
            delete downHandler;  // XXX
        }
        if (window)
        {
            window->release();
        }
    }

    NPObject* getScriptableInstance()
    {
        return 0;
    }
};

#endif // ESNPAPI_SAMPLE_H_INCLUDED
