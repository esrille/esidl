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

#include <esjsapi.h>

#include <org/w3c/dom/test/EventTarget.h>
#include <org/w3c/dom/test/Event.h>

#include "DOMStringMapImp.h"
#include "HTMLCollectionImp.h"

#include <iostream>

class EventTargetImp : public ObjectMixin<EventTargetImp>
{
    org::w3c::dom::test::EventListener listener;
    DOMStringMapImp stringMap;
    HTMLCollectionImp collection;
public:
    // EventTarget
    void addEventListener(std::u16string type, org::w3c::dom::test::EventListener listener) {
        this->listener = listener;
    }
    void removeEventListener(std::u16string type, org::w3c::dom::test::EventListener listener) {
        listener = 0;
    }
    bool dispatchEvent(org::w3c::dom::test::Event event) {
        if (listener) {
            listener(event);
            return true;
        }
        return false;
    }
    org::w3c::dom::test::DOMStringMap getDataset()
    {
        return &stringMap;
    }
    org::w3c::dom::test::HTMLCollection getCollection()
    {
        return &collection;
    }
    std::u16string getName()
    {
        return u"stringifier";
    }
    void overloaded(org::w3c::dom::test::DOMStringMap map)
    {
        std::cout << __func__ << ' ' << "DOMStringMap\n";
    }
    void overloaded(org::w3c::dom::test::HTMLCollection collection)
    {
        std::cout << __func__ << ' ' << "HTMLCollection\n";
    }

    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::test::EventTarget::dispatch(this, selector, id, argc, argv);
    }

    EventTargetImp() :
        listener(0)
    {
    }
};
