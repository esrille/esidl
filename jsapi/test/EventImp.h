/*
 * Copyright 2011 Esrille Inc.
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

#include <org/w3c/dom/test/Event.h>

#include <iostream>

class EventImp : public ObjectMixin<EventImp>
{
    std::u16string type;
public:
    // Event
    std::u16string getType() {
        return type;
    }
    void setType(std::u16string type) {
        this->type = type;
    }

    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::test::Event::dispatch(this, selector, id, argc, argv);
    }

    EventImp(std::u16string type) :
        type(type)
    {
        std::cout << __func__ << '(' << (char) (type[0]) << ')' << '\n';
    }
};
