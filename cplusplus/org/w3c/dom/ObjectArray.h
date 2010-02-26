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

#ifndef ORG_W3C_DOM_OBJECTARRAY_INCLUDED
#define ORG_W3C_DOM_OBJECTARRAY_INCLUDED

namespace org
{
namespace w3c
{
namespace dom
{

template <typename E>
class ObjectArray : public Object
{
public:
    virtual unsigned int getLength() = 0;
    virtual void setLength(unsigned int length) = 0;
    virtual E getElement(unsigned int index) = 0;
    virtual void setElement(unsigned int index, const E value) = 0;
};

}
}
}

#endif  // ORG_W3C_DOM_OBJECTARRAY_INCLUDED
