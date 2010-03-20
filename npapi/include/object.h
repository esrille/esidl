/*
 * Copyright 2008-2010 Google Inc.
 * Copyright 2006, 2007 Nintendo Co., Ltd.
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

#ifndef ESNPAPI_OBJECT_H_INCLUDED
#define ESNPAPI_OBJECT_H_INCLUDED

#include <reflect.h>

class Any;

class Object
{
public:
    static const char* const getQualifiedName()
    {
        return "::Object";
    }
    virtual void* queryInterface(const char* qualifiedName)
    {
        if (qualifiedName == getQualifiedName())
        {
            return this;
        }
        return 0;
    }
    virtual unsigned int retain() = 0;
    virtual unsigned int release() = 0;
    virtual const char* getMetaData(unsigned interfaceNumber) const = 0;
    virtual const Reflect::SymbolData* const getSymbolTable(unsigned interfaceNumber) const = 0;
    virtual Any call(unsigned interfaceNumber, unsigned methodNumber, unsigned argumentCount, Any* arguments) = 0;
};

template <typename X>
X interface_cast(Object* object)
{
    if (object)
    {
        return static_cast<X>(object->queryInterface(static_cast<X>(0)->getQualifiedName()));
    }
    return 0;
}

#endif  // ESNPAPI_OBJECT_H_INCLUDED
