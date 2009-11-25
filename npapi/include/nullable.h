/*
 * Copyright 2008, 2009 Google Inc.
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

#ifndef ESNPAPI_NULLABLE_H_INCLUDED
#define ESNPAPI_NULLABLE_H_INCLUDED

#include <exception.h>

template <typename T>
class Nullable
{
    bool hasValue_;
    T    value_;

public:
    bool hasValue() const
    {
        return hasValue_;
    }

    T value() const
    {
        if (hasValue_)
        {
            return value_;
        }
        throw std::unexpected();
    }

    Nullable()
    {
        hasValue_ = false;
    }

    Nullable(const T& value) :
        value_(value)
    {
        hasValue_ = true;
    }

    Nullable(const Nullable<T>& nullable) :
        hasValue_(nullable.hasValue_),
        value_(nullable.value_)
    {
    }
};

#endif  // ESNPAPI_NULLABLE_H_INCLUDED
