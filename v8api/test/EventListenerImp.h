/*
 * Copyright 2011, 2012 Esrille Inc.
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

#include <org/w3c/dom/test/EventListener.h>
#include <org/w3c/dom/test/Event.h>

class EventListenerImp : public ObjectMixin<EventListenerImp>
{
public:
    // EventListener
    void operator() (org::w3c::dom::test::Event event) {
    }

    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::test::EventListener::dispatch(this, selector, id, argc, argv);
    }

    EventListenerImp()
    {
    }
};
