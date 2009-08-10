/*
 * Copyright 2009 Google Inc.
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

#include <stdint.h>

template<class O>
class Proxy_Base
{
protected:
    O object;
public:
    Proxy_Base(O object) :
        object(object)
    {
    }
    virtual ~Proxy_Base()
    {
        object.release();
    }
    O* getObject()
    {
        return &object;
    }
};

template<class O, class I>
class Proxy_Impl1 : public I, public Proxy_Base<O>
{
public:
    Proxy_Impl1(O object) :
        Proxy_Base<O>(object)
    {
    }
    virtual ~Proxy_Impl1()
    {
    }
    virtual std::string getInterfaceName()
    {
        return this->object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return this->object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = this->object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J>
class Proxy_Impl2 : public I, public J, public Proxy_Base<O>
{
protected:
    O object;
public:
    Proxy_Impl2(O object) :
        Proxy_Base<O>(object)
    {
    }
    virtual ~Proxy_Impl2()
    {
    }
    virtual std::string getInterfaceName()
    {
        return this->object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return this->object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = this->object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J, class K>
class Proxy_Impl3 : public I, public J, public K, public Proxy_Base<O>
{
protected:
    O object;
public:
    Proxy_Impl3(O object) :
        Proxy_Base<O>(object)
    {
    }
    virtual ~Proxy_Impl3()
    {
    }
    virtual std::string getInterfaceName()
    {
        return this->object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return this->object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = this->object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J, class K, class L>
class Proxy_Impl4 : public I, public J, public K, public L, public Proxy_Base<O>
{
protected:
    O object;
public:
    Proxy_Impl4(O object) :
        Proxy_Base<O>(object)
    {
    }
    virtual ~Proxy_Impl4()
    {
    }
    virtual std::string getInterfaceName()
    {
        return this->object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return this->object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = this->object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J = int, class K = int, class L = int>
class Proxy_Impl : public Proxy_Impl4<O, I, J, K, L>
{
public:
    Proxy_Impl(O object) :
        Proxy_Impl4<O, I, J, K, L>(object)
    {
    }
};

template<class O, class I, class J, class K>
class Proxy_Impl<O, I, J, K, int> : public Proxy_Impl3<O, I, J, K>
{
public:
    Proxy_Impl(O object) :
        Proxy_Impl3<O, I, J, K>(object)
    {
    }
};

template<class O, class I, class J>
class Proxy_Impl<O, I, J, int, int> : public Proxy_Impl2<O, I, J>
{
public:
    Proxy_Impl(O object) :
        Proxy_Impl2<O, I, J>(object)
    {
    }
};

template<class O, class I>
class Proxy_Impl<O, I, int, int, int> : public Proxy_Impl1<O, I>
{
public:
    Proxy_Impl(O object) :
        Proxy_Impl1<O, I>(object)
    {
    }
};

#endif // ESNPAPI_PROXY_IMPL_H_INCLUDED
