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

#ifndef ESIDL_ANY_H_INCLUDED
#define ESIDL_ANY_H_INCLUDED

#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

#include <new>
#include <string>

class Object;

struct AnyBase
{
    union
    {
        bool        boolValue;
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
    };
    unsigned     type;
};

// The any type for Web IDL
class Any : private AnyBase
{
    Any& assign(const Any& value)
    {
        type = value.type;
        if (type == TypeString)
        {
            new (stringValue) std::string(static_cast<const std::string>(value));
        }
        else
        {
            unsignedLongLongValue = value.unsignedLongLongValue;
        }
    }

public:
    enum
    {
        TypeVoid,
        TypeBool,
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
        FlagAny = 0x80000000
    };

    Any()
    {
        longLongValue = 0;
        type = TypeVoid;
    }

    Any(const AnyBase& v) :
        AnyBase(v)
    {
        type |= FlagAny;
    }

    Any(bool value)
    {
        boolValue = value;
        type = TypeBool;
    }

    Any(uint8_t value)
    {
        octetValue = value;
        type = TypeOctet;
    }

    Any(int16_t value)
    {
        shortValue = value;
        type = TypeShort;
    }

    Any(uint16_t value)
    {
        unsignedShortValue = value;
        type = TypeUnsignedShort;
    }

    // Since int32_t can be either long or int, use int so as not to conflict with constructor for long
    Any(int value)
    {
        longValue = value;
        type = TypeLong;
    }

    // Since uint32_t can be either long or int, use int so as not to conflict with constructor for unsigned long
    Any(unsigned int value)
    {
        unsignedLongValue = value;
        type = TypeUnsignedLong;
    }

    // To support constants declared with intptr_t, constructor for long is necessary.
    Any(long value)
    {
#if 2147483647L < LONG_MAX
        longLongValue = value;
        type = TypeLongLong;
#else
        longValue = value;
        type = TypeLong;
#endif
    }

    // To support constants declared with uintptr_t, constructor for long is necessary.
    Any(unsigned long value)
    {
#if 4294967295UL < ULONG_MAX
        unsignedLongValue = value;
        type = TypeUnsignedLong;
#else
        unsignedLongValue = value;
        type = TypeUnsignedLong;
#endif
    }

    // To support constants declared with LL, long long variant cannot be int64_t.
    Any(long long value)
    {
        longLongValue = value;
        type = TypeLongLong;
    }

    // To support constants declared with LLu, long long variant cannot be uint64_t.
    Any(unsigned long long value)
    {
        unsignedLongLongValue = value;
        type = TypeUnsignedLongLong;
    }

    Any(float value)
    {
        floatValue = value;
        type = TypeFloat;
    }

    Any(double value)
    {
        doubleValue = value;
        type = TypeDouble;
    }

    Any(const std::string value)
    {
        new (stringValue) std::string(value);
        type = TypeString;
    }

    Any(const char* value)
    {
        new (stringValue) std::string(value);
        type = TypeString;
    }

    Any(Object* value)
    {
        objectValue = value;
        type = TypeObject;
    }

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

    Any& operator=(const Any& value)
    {
        assign(value);
    }

    operator uint8_t() const
    {
        return octetValue;
    }

    operator int16_t() const
    {
        return shortValue;
    }

    operator uint16_t() const
    {
        return unsignedShortValue;
    }

    operator int32_t() const
    {
        return longValue;
    }

    operator uint32_t() const
    {
        return unsignedLongValue;
    }

    operator int64_t() const
    {
        return longLongValue;
    }

    operator uint64_t() const
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
        return *reinterpret_cast<const std::string*>(&stringValue);
    }

    operator Object*() const
    {
        return objectValue;
    }

    int getType() const
    {
        return (type & ~FlagAny);
    }

    void makeVariant()
    {
        type |= FlagAny;
    }

    bool isVariant() const
    {
        return (type & FlagAny) ? true : false;
    }

    bool isString() const
    {
        return (getType() == TypeString) ? true : false;
    }
};

Any apply(int argc, Any* argv, Any (*function)());
Any apply(int argc, Any* argv, bool (*function)());
Any apply(int argc, Any* argv, uint8_t (*function)());
Any apply(int argc, Any* argv, int16_t (*function)());
Any apply(int argc, Any* argv, uint16_t (*function)());
Any apply(int argc, Any* argv, int32_t (*function)());
Any apply(int argc, Any* argv, uint32_t (*function)());
Any apply(int argc, Any* argv, int64_t (*function)());
Any apply(int argc, Any* argv, uint64_t (*function)());
Any apply(int argc, Any* argv, float (*function)());
Any apply(int argc, Any* argv, double (*function)());
Any apply(int argc, Any* argv, std::string (*function)());
Any apply(int argc, Any* argv, Object* (*function)());

#endif // ESIDL_ANY_H_INCLUDED
