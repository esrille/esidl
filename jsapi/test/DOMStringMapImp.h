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

#ifndef DOMSTRING_MAP_IMP_H
#define DOMSTRING_MAP_IMP_H

#include <esjsapi.h>

#include <org/w3c/dom/test/DOMStringMap.h>

#include <map>
#include <string>

#include <iostream>

class DOMStringMapImp : public ObjectMixin<DOMStringMapImp>
{
    std::map<std::u16string, std::u16string> map;

public:
    // DOMStringMap
    std::u16string getElement(std::u16string name)
    {
        auto found = map.find(name);
        if (found == map.end())
            return u"";
        return found->second;
    }

    void setElement(std::u16string name, std::u16string value)
    {
        map[name] = value;
    }

    void createElement(std::u16string name, std::u16string value)
    {
        map[name] = value;
    }

    void deleteElement(std::u16string name)
    {
        map.erase(name);
    }

    // Object
    virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {
        return org::w3c::dom::test::DOMStringMap::dispatch(this, selector, id, argc, argv);
    }

    DOMStringMapImp()
    {
    }
};

#endif  // DOMSTRING_MAP_IMP_H
