/*
 * Copyright 2011 Esrille Inc.
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

#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <Object.h>

namespace {

std::string toString(const char* format, ...)
{
    va_list ap;
    char buffer[24];

    va_start(ap, format);
    vsnprintf(buffer, sizeof buffer, format, ap);
    va_end(ap);
    return std::string(buffer);
}

std::string toString(double value)
{
    if (isnan(value))
        return "NaN";
    if (isinf(value))
        return (isinf(value) == 1) ? "Infinity" : "-Infinity";
    char buffer[24];
    std::sprintf(buffer, "%g", value);
    return std::string(buffer);
}

std::string toString(Object* object)
{
    if (!object)
        return "null";
    Any result = object->message_(0, 0, Object::STRINGIFY_, 0);
    if (result.isString())
        return result.toString();
    return "";
}

char* skipSpace(const char* str)
{
    static const char* rgsp[] =
    {
        // NBSP
        "\xc2\xa0",
        // LS
        "\xe2\x80\xa8",
        // PS
        "\xe2\x80\xa9",
        // USP
        "\xe1\x9a\x80",
        "\xe1\xa0\x8e",
        "\xe2\x80\x80",
        "\xe2\x80\x81",
        "\xe2\x80\x82",
        "\xe2\x80\x83",
        "\xe2\x80\x84",
        "\xe2\x80\x85",
        "\xe2\x80\x86",
        "\xe2\x80\x87",
        "\xe2\x80\x88",
        "\xe2\x80\x89",
        "\xe2\x80\x8a",
        "\xe2\x80\xaf",
        "\xe2\x81\x9f",
        "\xe3\x80\x80",
        // BOM
        "\xef\xbb\xbf"
    };

    while (char c = *str)
    {
        if (strchr(" \t\v\f\n\r", c))
        {
            ++str;
            continue;
        }
        int i;
        for (i = 0; i < sizeof(rgsp) / sizeof(rgsp[0]); ++i)
        {
            const char* sp = rgsp[i];
            size_t len = strlen(sp);
            if (strncmp(str, sp, len) == 0)
            {
                str += len;
                break;
            }
        }
        if (sizeof(rgsp) / sizeof(rgsp[0]) <= i)
        {
            break;
        }
    }
    return const_cast<char*>(str);
}

double toNumber(const std::string value)
{
    const char* ptr = value.c_str();
    ptr = skipSpace(ptr);
    const char* uptr = ptr;
    bool negative = (*ptr == '-');
    if (negative || *ptr == '+')
    {
        ++uptr;
    }
    char* end;
    double number;
    if (strncmp(uptr, "Infinity", 8) == 0)
    {
        end = const_cast<char*>(uptr) + 8;
        number = negative ? -INFINITY : INFINITY;
    }
    else
    {
        number = strtod(ptr, &end);
        if (end == ptr)
        {
            return NAN;
        }
    }
    end = skipSpace(end);
    if (*end != 0)
    {
        return NAN;
    }
    return number;
};

}  // namespace

bool Any::isString() const
{
    return (type == Dynamic) && vtable->getType() == typeid(std::string);
}

std::string Any::toString() const
{
    switch (type) {
    case Bool:
        return heap.i32 ? "true" : "false";
    case Int32:
        return ::toString("%d", heap.i32);
    case Uint32:
        return ::toString("%u", heap.u32);
    case Int64:
        return ::toString("%lld", heap.i64);
    case Uint64:
        return ::toString("%llu", heap.u64);
    case Float32:
        return ::toString(heap.f32);
    case Float64:
        return ::toString(heap.f64);
    case Dynamic:
        if (vtable->getType() == typeid(std::string))
            return *reinterpret_cast<const std::string*>(&heap);
        if (vtable->getType() == typeid(Object))
            return ::toString(const_cast<Object*>(reinterpret_cast<const Object*>(&heap)->self()));
        break;
    default:
        break;
    }
    return std::string();
}

// toNumber
// for Object, call valueOf and then apply toNumber
// null returns 0, undefined returns NAN
double Any::toNumber() const
{
    switch (type) {
    case Bool:
    case Int32:
        return heap.i32;
    case Uint32:
        return heap.u32;
    case Int64:
        return heap.i64;
    case Uint64:
        return heap.u64;
    case Float32:
        return heap.f32;
    case Float64:
        return heap.f64;
    case Dynamic:
        if (vtable->getType() == typeid(std::string))
            return ::toNumber(*reinterpret_cast<const std::string*>(&heap));
        break;
    default:
        break;
    }
    return 0.0;
}

// toBoolean
// Object returns true. Otherwise, false.
// NAN returns false.
// empty string returns false. Otherwise, true.
bool Any::toBoolean() const
{
    switch (type) {
    case Bool:
    case Int32:
        return heap.i32;
    case Uint32:
        return heap.u32;
    case Int64:
        return heap.i64;
    case Uint64:
        return heap.u64;
    case Float32:
        return heap.f32 && !isnan(heap.f32);
    case Float64:
        return heap.f64 && !isnan(heap.f64);
    case Dynamic:
        if (vtable->getType() == typeid(std::string))
            return (*reinterpret_cast<const std::string*>(&heap)).length();
        return true;
        break;
    default:
        break;
    }
    return false;
}

bool Any::isObject() const
{
    return (type == Dynamic) && vtable->getType() == typeid(Object);
}

Object* Any::toObject() const
{
    if (isObject())
        return reinterpret_cast<const Object*>(&heap)->self();
    return 0;
}