/*
 * Copyright 2009, 2010 Google Inc.
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

#ifndef ESNPAPI_PROXY_IMPL_H_INCLUDED
#define ESNPAPI_PROXY_IMPL_H_INCLUDED

class Object;

template<class O, class I>
class Proxy_Impl : public O, public I
{
public:
    Proxy_Impl(O object) :
        O(object)
    {
    }
    virtual ~Proxy_Impl()
    {
    }
    virtual std::string getInterfaceName()
    {
        return O::getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return O::retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = O::release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
    static Object* createInstance(O object)
    {
        return static_cast<Object*>(new Proxy_Impl(object));
    }    
    virtual void* queryInterface(const char* qualifiedName)
    {
        if (qualifiedName == O::getQualifiedName())
        {
            return static_cast<O*>(this);
        }
        if (void* object = I::queryInterface(qualifiedName))
        {
            return object;
        }
        return 0;
    }
};

#endif // ESNPAPI_PROXY_IMPL_H_INCLUDED
