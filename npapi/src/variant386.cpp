/*
 * Copyright 2008, 2009 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "any.h"

// The apply functions are implemented in C++ so that these can support C++
// exceptions.

// This version supports the binding DOMString for std::string.

namespace
{

Any apply(int argc, Any* argv, void (*function)())
{
    // Do not invoke C++ functions inside apply(). Otherwise, the stack will
    // be polluted while the stack frame for function is being built.
    Any* result;
    unsigned type;
    __asm__ __volatile__ (
        "movl   8(%%ebp), %0\n"
        :"=r"(result)
    );
    for (argv += argc - 1; 0 < argc; --argc, --argv)
    {
        __asm__ __volatile__ (
            "movl   8(%1), %0\n"
            :"=r"(type) :"r"(argv)
        );
        if (type & Any::FlagAny)
        {
            __asm__ __volatile__ (
                "pushl  %0\n"
                "pushl  4(%1)\n"
                "pushl  (%1)\n"
                ::"r"(type), "r"(argv)
            );
            continue;
        }
        switch (type)
        {
        case Any::TypeBool:
        case Any::TypeOctet:
            __asm__ __volatile__ (
                "movzbl (%0), %%edx\n"
                "pushl   %%edx\n"
                ::"r"(argv) : "%edx"
            );
            break;
        case Any::TypeShort:
        case Any::TypeUnsignedShort:
            __asm__ __volatile__ (
                "movzwl (%0), %%edx\n"
                "pushl   %%edx\n"
                ::"r"(argv) : "%edx"
            );
            break;
        case Any::TypeLong:
        case Any::TypeUnsignedLong:
        case Any::TypeFloat:
        case Any::TypeObject:
            __asm__ __volatile__ (
                "pushl  (%0)\n"
                ::"r"(argv)
            );
            break;
        case Any::TypeLongLong:
        case Any::TypeUnsignedLongLong:
        case Any::TypeDouble:
            __asm__ __volatile__ (
                "pushl  4(%0)\n"
                "pushl  (%0)\n"
                ::"r"(argv)
            );
            break;
        case Any::TypeString:
        default:
            __asm__ __volatile__ (
                "pushl  %0\n"
                ::"r"(argv)
            );
            break;
        }
    }
    __asm__ __volatile__ (
        "movl   8(%1), %0\n"
        :"=r"(type) :"r"(result)
    );
    if ((type & Any::FlagAny)  || type == Any::TypeString)
    {
        __asm__ __volatile__ (
            "pushl  %0\n"
            ::"r"(result)
        );
        function();
    }
    else
    {
        switch (type)
        {
        case Any::TypeFloat:
            function();
            __asm__ __volatile__ (
                "fstps  0(%0)\n"
                ::"r"(result)
            );
            break;
        case Any::TypeDouble:
            function();
            __asm__ __volatile__ (
                "fstpl  0(%0)\n"
                ::"r"(result)
            );
            break;
        default:
            function();
            __asm__ __volatile__ (
                "movl   %%eax, 0(%0)\n"
                "movl   %%edx, 4(%0)\n"
                ::"r"(result) : "%eax", "%edx"
            );
            break;
        }
    }
}

}  // namespace

Any apply(int argc, Any* argv, bool (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $1, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, uint8_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $2, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, int16_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $3, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, uint16_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $4, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, int32_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $5, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, uint32_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $6, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, int64_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $7, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, uint64_t (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $8, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, float (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $9, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, double (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $10, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, std::string (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $11, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, Object* (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $12, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}

Any apply(int argc, Any* argv, Any (*function)())
{
    __asm__ __volatile__ (
        "movl   8(%ebp), %eax\n"
        "movl   $0x80000000, 8(%eax)\n"
    );
    return apply(argc, argv, reinterpret_cast<void (*)()>(function));
}
