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

#ifndef SAMPLE_PAINT_H_
#define SAMPLE_PAINT_H_

#include <esnpapi.h>
#include <org/w3c/dom/events/Event.h>
#include <org/w3c/dom/events/EventListener.h>

class PaintInstance;

class EventHandler : public ObjectImp
{
    PaintInstance* instance;
    void (PaintInstance::*handler)(org::w3c::dom::events::Event evt);
public:
    EventHandler(PaintInstance* instance,
                 void (PaintInstance::*handler)(org::w3c::dom::events::Event)) :
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

class PaintInstance : public PluginInstance
{
public:
    PaintInstance(NPP npp, NPObject* window) :
        PluginInstance(npp, window),
        downHandler(0),
        moveHandler(0),
        upHandler(0),
        selectHandler(0) {
        initialize();
    }
    ~PaintInstance();

private:
    org::w3c::dom::events::EventListener downHandler;
    org::w3c::dom::events::EventListener moveHandler;
    org::w3c::dom::events::EventListener upHandler;
    org::w3c::dom::events::EventListener selectHandler;

    void initialize();
    void down(org::w3c::dom::events::Event evt);
    void move(org::w3c::dom::events::Event evt);
    void up(org::w3c::dom::events::Event evt);
    void select(org::w3c::dom::events::Event evt);
};

#endif  // SAMPLE_PAINT_H_
