/*
 * Copyright 2011 Esrille Inc.
 * Copyright 2010 Google Inc.
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

#ifndef SAMPLE_XHR_H_
#define SAMPLE_XHR_H_

#include <esnpapi.h>
#include <org/w3c/dom/events/Event.h>
#include <org/w3c/dom/events/EventListener.h>

class XHRInstance;

class EventHandler : public ObjectImp
{
    XHRInstance* instance;
    void (XHRInstance::*handler)(org::w3c::dom::events::Event evt);
public:
    EventHandler(XHRInstance* instance,
                 void (XHRInstance::*handler)(org::w3c::dom::events::Event)) :
        instance(instance),
        handler(handler) {
    }
    virtual void handleEvent(org::w3c::dom::events::Event evt) {
        (instance->*handler)(evt);
    }
    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::events::EventListener::dispatch(this, selector, id, argc, argv);
    }
};

class XHRInstance : public PluginInstance {
    org::w3c::dom::events::EventListener loadHandler;
    org::w3c::dom::events::EventListener displayHandler;

    void initialize();
    void load(org::w3c::dom::events::Event evt);
    void display(org::w3c::dom::events::Event evt);
public:
    XHRInstance(NPP npp, NPObject* window) :
        PluginInstance(npp, window),
        loadHandler(0),
        displayHandler(0) {
        initialize();
    }
    ~XHRInstance();
};

#endif  // SAMPLE_XHR_H_
