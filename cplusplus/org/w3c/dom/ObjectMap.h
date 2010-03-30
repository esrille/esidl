/*
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

#ifndef ORG_W3C_DOM_OBJECTMAP_INCLUDED
#define ORG_W3C_DOM_OBJECTMAP_INCLUDED

#include <string>

namespace org
{
namespace w3c
{
namespace dom
{

template <typename E>
class ObjectMap : public Object
{
public:
    virtual E getElement(std::string name) = 0;
    virtual void setElement(std::string name, const E value) = 0;
    virtual void createElement(std::string name, const E value) = 0;
    virtual void deleteElement(std::string name) = 0;
};

}
}
}

#endif  // ORG_W3C_DOM_OBJECTMAP_INCLUDED
