/*
 * Copyright 2008-2010 Google Inc.
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

#ifndef ES_ANY_H_INCLUDED
#define ES_ANY_H_INCLUDED

#include <inttypes.h>
#include <limits.h>

#include <new>
#include <string>

#include <nullable.h>
#include <sequence.h>

class Object;

struct AnyBase
{
    union
    {
        bool        boolValue;
        int8_t      byteValue;
        uint8_t     octetValue;
        int16_t     shortValue;
        uint16_t    unsignedShortValue;
        int32_t     longValue;
        uint32_t    unsignedLongValue;
        int64_t     longLongValue;
        uint64_t    unsignedLongLongValue;
        float       floatValue;
        double      doubleValue;
        Object*     objectValue;
        char        stringValue[sizeof(std::string)];
        char        sequenceValue[sizeof(std::string)];
    };
    unsigned        type;
};

// The any type for Web IDL
class Any : private AnyBase
{
    Any& assign(const Any& value)
    {
        type = value.type;
        if (type & FlagSequence)
        {
            new (sequenceValue) Sequence<std::string>(reinterpret_cast<const Sequence<std::string>*>(value.sequenceValue));
        }
        else if ((type & PrimitiveMask) == TypeString)
        {
            new (stringValue) std::string(*reinterpret_cast<const std::string*>(value.stringValue));
        }
        else
        {
            unsignedLongLongValue = value.unsignedLongLongValue;
        }
        return *this;
    }

    Any& assign(bool value)
    {
        boolValue = value;
        type = TypeBool;
        return *this;
    }

    Any& assign(signed char value)
    {
        byteValue = value;
        type = TypeByte;
        return *this;
    }

    Any& assign(unsigned char value)
    {
        octetValue = value;
        type = TypeOctet;
        return *this;
    }

    Any& assign(short value)
    {
        shortValue = value;
        type = TypeShort;
        return *this;
    }

    Any& assign(unsigned short value)
    {
        unsignedShortValue = value;
        type = TypeUnsignedShort;
        return *this;
    }

    // Since int32_t can be either long or int, use int so as not to conflict with constructor for long
    Any& assign(int value)
    {
        longValue = value;
        type = TypeLong;
        return *this;
    }

    // Since uint32_t can be either long or int, use int so as not to conflict with constructor for unsigned long
    Any& assign(unsigned int value)
    {
        unsignedLongValue = value;
        type = TypeUnsignedLong;
        return *this;
    }

    // To support constants declared with intptr_t, constructor for long is necessary.
    Any& assign(long value)
    {
#if 2147483647L < LONG_MAX
        longLongValue = value;
        type = TypeLongLong;
#else
        longValue = value;
        type = TypeLong;
#endif
        return *this;
    }

    // To support constants declared with uintptr_t, constructor for long is necessary.
    Any& assign(unsigned long value)
    {
#if 4294967295UL < ULONG_MAX
        unsignedLongValue = value;
        type = TypeUnsignedLong;
#else
        unsignedLongValue = value;
        type = TypeUnsignedLong;
#endif
        return *this;
    }

    // To support constants declared with LL, long long variant cannot be int64_t.
    Any& assign(long long value)
    {
        longLongValue = value;
        type = TypeLongLong;
        return *this;
    }

    // To support constants declared with LLu, long long variant cannot be uint64_t.
    Any& assign(unsigned long long value)
    {
        unsignedLongLongValue = value;
        type = TypeUnsignedLongLong;
        return *this;
    }

    Any& assign(float value)
    {
        floatValue = value;
        type = TypeFloat;
        return *this;
    }

    Any& assign(double value)
    {
        doubleValue = value;
        type = TypeDouble;
        return *this;
    }

    Any& assign(const std::string& value)
    {
        new (stringValue) std::string(value);
        type = TypeString;
        return *this;
    }

    template <typename T>
    Any& assign(const Sequence<T>& sequence)
    {
        new (sequenceValue) Sequence<T>(sequence);
        // TODO: set type for T
        type = FlagSequence;
        return *this;
    }

    Any& assign(const char* value)
    {
        new (stringValue) std::string(value);
        type = TypeString;
        return *this;
    }

    Any& assign(Object* value)
    {
        objectValue = value;
        type = TypeObject;
        return *this;
    }

public:
    enum
    {
        TypeVoid,
        TypeBool,
        TypeByte,
        TypeOctet,
        TypeShort,
        TypeUnsignedShort,
        TypeLong,
        TypeUnsignedLong,
        TypeLongLong,
        TypeUnsignedLongLong,
        TypeFloat,
        TypeDouble,
        TypeString,
        TypeObject,
        PrimitiveMask = 0x0fffffff,
        FlagSequence = 0x20000000,
        FlagAny = 0x40000000,
        FlagNullable = 0x80000000
    };

    Any()
    {
        longLongValue = 0;
        type = TypeVoid;
    }

    template <typename T>
    Any(T value)
    {
        assign(value);
    }

    template <typename T>
    Any(const Nullable<T> nullable)
    {
        if (!nullable.hasValue())
        {
            longLongValue = 0;
            type = TypeVoid;
        }
        else
        {
            assign(nullable.value());
        }
        type |= FlagNullable;
    }

    // Copy constructor
    Any(const Any& value)
    {
        assign(value);
    }

    ~Any()
    {
        if (getType() == TypeString)
        {
            using std::string;

            const string* p = reinterpret_cast<const string*>(stringValue);
            p->~string();
        }
    }

    template <typename T>
    Any& operator=(T value)
    {
        assign(value);
        return *this;
    }

    Any& operator=(const Any& value)
    {
        assign(value);
        return *this;
    }

    operator signed char() const
    {
        return byteValue;
    }

    operator unsigned char() const
    {
        return octetValue;
    }

    operator short() const
    {
        return shortValue;
    }

    operator unsigned short() const
    {
        return unsignedShortValue;
    }

    operator int() const
    {
        return longValue;
    }

    operator unsigned int() const
    {
        return unsignedLongValue;
    }

    operator long() const
    {
#if 2147483647L < LONG_MAX
        return longLongValue;
#else
        return longValue;
#endif
    }

    operator unsigned long() const
    {
#if 4294967295UL < ULONG_MAX
        return unsignedLongLongValue;
#else
        return unsignedLongValue;
#endif
    }

    operator long long() const
    {
        return longLongValue;
    }

    operator unsigned long long() const
    {
        return unsignedLongLongValue;
    }

    operator bool() const
    {
        return boolValue;
    }

    operator float() const
    {
        return floatValue;
    }

    operator double() const
    {
        return doubleValue;
    }

    operator const std::string() const
    {
        return *reinterpret_cast<const std::string*>(stringValue);
    }

    operator Object*() const
    {
        return objectValue;
    }

    int getType() const
    {
        return (type & PrimitiveMask);
    }

    void makeAny()
    {
        type |= FlagAny;
    }

    bool isNullable() const
    {
        return (type & FlagNullable);
    }

    bool isAny() const
    {
        return (type & FlagAny);
    }

    bool isString() const
    {
        return getType() == TypeString;
    }

    bool isObject() const
    {
        return getType() == TypeObject;
    }

    bool isNull() const
    {
        return getType() == TypeVoid;
    }

    bool isSequence() const
    {
        return (type & FlagSequence);
    }

    bool hasValue() const
    {
        return getType() != TypeVoid;
    }

    template <typename T>
    const Sequence<T>* getSequence() const
    {
        return reinterpret_cast<const Sequence<T>*>(&sequenceValue);
    };
};

template <typename T>
Nullable<T>::Nullable(const Any& any)
{
    hasValue_ = any.hasValue();
    if (hasValue_)
    {
        value_ = static_cast<T>(any);
    }
}

template <typename T>
Sequence<T>::Sequence(const Any& any)
{
    if (!any.isSequence())
    {
        // TODO: Raise an exception
    }
    const Sequence<T>* value = any.getSequence<T>();
    ++value->rep->count;
    rep = value->rep;
}

#endif // ESNPAPI_ANY_H_INCLUDED
