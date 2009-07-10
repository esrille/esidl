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

#ifndef ESIDL_PROXY_IMPL_H_INCLUDED
#define ESIDL_PROXY_IMPL_H_INCLUDED

#include <stdint.h>

template<class O, class I>
class Proxy_Impl1 : public I
{
protected:
    O object;
public:
    Proxy_Impl1(O object) :
        object(object)
    {
    }
    virtual ~Proxy_Impl1()
    {
        object.release();
    }
    virtual std::string getInterfaceName()
    {
        return object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J>
class Proxy_Impl2 : public I, public J
{
protected:
    O object;
public:
    Proxy_Impl2(O object) :
        object(object)
    {
    }
    virtual ~Proxy_Impl2()
    {
        object.release();
    }
    virtual std::string getInterfaceName()
    {
        return object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J, class K>
class Proxy_Impl3 : public I, public J, public K
{
protected:
    O object;
public:
    Proxy_Impl3(O object) :
        object(object)
    {
    }
    virtual ~Proxy_Impl3()
    {
        object.release();
    }
    virtual std::string getInterfaceName()
    {
        return object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = object.release();
        if (count == 0)
        {
            delete this;
        }
        return count;
    }
};

template<class O, class I, class J, class K, class L>
class Proxy_Impl4 : public I, public J, public K, public L
{
protected:
    O object;
public:
    Proxy_Impl4(O object) :
        object(object)
    {
    }
    virtual ~Proxy_Impl4()
    {
        object.release();
    }
    virtual std::string getInterfaceName()
    {
        return object.getInterfaceName();
    }
    virtual unsigned int retain()
    {
        return object.retain();
    }
    virtual unsigned int release()
    {
        unsigned int count = object.release();
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

#endif // ESIDL_PROXY_IMPL_H_INCLUDED
