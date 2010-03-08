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

#ifndef ES_SEQUENCE_INCLUDED
#define ES_SEQUENCE_INCLUDED

#include <org/w3c/dom/ObjectArray.h>
#include <new>

class Any;

// The sequence type for Web IDL
// TODO: This implementation is not multi-thread safe.
template <typename T>
class Sequence
{
    class Rep : public org::w3c::dom::ObjectArray<T>
    {
        friend class Sequence;
        // stub
        T* sequence;
        unsigned int length;
        // proxy
        org::w3c::dom::ObjectArray<T>* array;
        // reference count
        unsigned count;

        void init(unsigned int size)
        {
            array = 0;
            length = size;
            sequence = (0 < length) ? new T[length] : 0;
            count = 1;
        }

        void copy(const T* array, unsigned int size)
        {
            for (unsigned int i = 0; i < length; ++i)
            {
                sequence[i] = array[i];
            }
        }

        // Prevent copying
        Rep(const Rep& value);
        Rep& operator=(const Rep& value);

    public:
        Rep()
        {
            init(0);
        }

        Rep(const T* sequence, unsigned int length)
        {
            init(length);
            copy(sequence, length);
        }

        Rep(unsigned int size)
        {
            init(size);
        }

        Rep(org::w3c::dom::ObjectArray<T>* array)
        {
            init(0);
            this->array = array;
        }

        ~Rep()
        {
            if (sequence)
            {
                delete[] sequence;
            }
            delete array;
        }

        virtual T getElement(unsigned int index)
        {
            if (array)
            {
                return array->getElement(index);
            }
            return sequence[index];
        }

        virtual void setElement(unsigned int index, const T value)
        {
            if (array)
            {
                array->setElement(index, value);
                return;
            }
            sequence[index] = value;
        }

        virtual unsigned int getLength()
        {
            if (array)
            {
                return array->getLength();
            }
            return length;
        }

        virtual void setLength(unsigned int length)
        {
            if (array)
            {
                array->setLength(length);
                return;
            }
            // TODO: Raise an exception
        }
    };

    Rep* rep;

public:
    class Ref
    {
        friend class Sequence;
        Sequence& sequence;
        unsigned int index;

        Ref(Sequence& sequence, unsigned int index) :
            sequence(sequence),
            index(index)
        {
        }
    public:
        operator T()
        {
            return sequence.getElement(index);
        }
        void operator=(T value)
        {
            sequence.setElement(index, value);
        }
    };

    Sequence()
    {
        rep = new Rep();
    }

    Sequence(const T* array, unsigned int size)
    {
        rep = new Rep(array, size);
    }

    Sequence(unsigned int size)
    {
        rep = new Rep(size);
    }

    Sequence(org::w3c::dom::ObjectArray<T>* array)
    {
        rep = new Rep(array);
    }

    // Copy-constructor
    Sequence(const Sequence& value)
    {
        ++value.rep->count;
        rep = value.rep;
    }

    ~Sequence()
    {
        if (--rep->count == 0)
        {
            delete rep;
        }
    }

    Sequence& operator=(const Sequence& value)
    {
        ++value.rep->count;
        if (--rep->count == 0)
        {
            delete rep;
        }
        rep = value.rep;
        return *this;
    }

    T getElement(unsigned int index)
    {
        return rep->getElement(index);
    }

    void setElement(unsigned int index, const T value)
    {
        rep->setElement(index, value);
    }

    T operator[](int index) const
    {
        return rep->getElement(index);
    }

    Ref operator[](int index)
    {
        return Ref(*this, index);
    }

    T at(unsigned int index) const
    {
        if (getLength() <= index)
        {
            // TODO: raise an exception;
        }
        return rep->getElement(index);
    }

    Ref at(unsigned int index)
    {
        if (getLength() <= index)
        {
            // TODO: raise an exception;
        }
        return Ref(*this, index);
    }

    unsigned int getLength() const
    {
        return rep->getLength();
    }

    operator org::w3c::dom::ObjectArray<T>*()
    {
        return rep;
    }

    Sequence(const Any& any);
};

#endif  // ES_SEQUENCE_INCLUDED
