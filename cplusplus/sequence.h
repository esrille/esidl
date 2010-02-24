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

#include <new>

// The sequence type for Web IDL
// TODO: How could we improve the implementation for providing better performance?
template <typename T>
class Sequence
{
    T* sequence;
    size_t length;

    void init(size_t size)
    {
        length = size;
        sequence = (0 < length) ? new T[length] : 0;
    }

    void clone(const T* array, size_t size)
    {
        init(size);
        for (size_t i = 0; i < length; ++i)
        {
            sequence[i] = array[i];
        }
    }

public:
    Sequence() :
        sequence(0),
        length(0)
    {
    }

    Sequence(const T* array, size_t size)
    {
        clone(array, size);
    }

    Sequence(size_t size)
    {
        init(size);
    }

    /** Copy-constructor.
     */
    Sequence(const Sequence& value)
    {
        clone(value.sequence, value.length);
    }

    ~Sequence()
    {
        if (sequence)
        {
            delete[] sequence;
        }
    }

    Sequence& operator=(const Sequence& value)
    {
        clone(value.sequence, value.size);
        return *this;
    }

    T operator[](int index) const
    {
        return sequence[index];
    }

    T& operator[](int index)
    {
        return sequence[index];
    }

    T at(size_t index) const
    {
        if (length <= index)
        {
            // TODO: raise an exception;
        }
        return sequence[index];
    }

    T& at(size_t index)
    {
        if (length <= index)
        {
            // TODO: raise an exception;
        }
        return sequence[index];
    }

    size_t getLength() const
    {
        return length;
    }
};

#endif  // ES_SEQUENCE_INCLUDED
