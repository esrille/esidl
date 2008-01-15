/*
 * Copyright 2008 Google Inc.
 * Copyright 2006, 2007 Nintendo Co., Ltd.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <es/ent.h>
#include <es/reflect.h>
#include "utf.h"

void printGuid(const Guid& guid)
{
    printf("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
           guid.Data1, guid.Data2, guid.Data3,
           guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
           guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}

void processType(Reflect::Type type)
{
    switch (type.getType())
    {
    case Ent::SpecS8:
        printf("s8");
        break;
    case Ent::SpecS16:
        printf("s16");
        break;
    case Ent::SpecS32:
        printf("s32");
        break;
    case Ent::SpecS64:
        printf("s64");
        break;
    case Ent::SpecU8:
        printf("u8");
        break;
    case Ent::SpecU16:
        printf("u16");
        break;
    case Ent::SpecU32:
        printf("u32");
        break;
    case Ent::SpecU64:
        printf("u64");
        break;
    case Ent::SpecF32:
        printf("f32");
        break;
    case Ent::SpecF64:
        printf("f64");
        break;
    case Ent::SpecF128:
        printf("f128");
        break;
    case Ent::SpecBool:
        printf("bool");
        break;
    case Ent::SpecChar:
        printf("char");
        break;
    case Ent::SpecWChar:
        printf("wchar");
        break;
    case Ent::SpecVoid:
        printf("void");
        break;
    case Ent::SpecUuid:
        printf("uuid");
        break;
    case Ent::SpecString:
        printf("string");
        break;
    case Ent::SpecWString:
        printf("wstring");
        break;
    case Ent::SpecAny:
        printf("any");
        break;
    case Ent::SpecObject:
        printf("object");
        break;
    case Ent::SpecFixed:
        printf("fixed");
        break;
    case Ent::SpecValue:
        printf("value");
        break;
    case Ent::TypeModule:
        printf("module");
        break;
    case Ent::TypeInterface:
        printf("interface");
        break;
    case Ent::TypeStructure:
        printf("structure");
        break;
    case Ent::TypeException:
        printf("exception");
        break;
    case Ent::TypeEnum:
        printf("enum");
        break;
    case Ent::TypeArray:
        printf("array");
        break;
    case Ent::TypeSequence:
        printf("sequence");
        break;
    default:
        fprintf(stderr, "Unknown type %x\n", type.getType());
        exit(EXIT_FAILURE);
        break;
    }
}

void processConstant(Reflect::Constant& c)
{
    char utf8[9];

    processType(c.getType());
    printf(" %s = ", c.getName());
    switch (c.getType().getType())
    {
    case Ent::SpecU8:
        printf("%u", c.getValue<u8>());
        break;
    case Ent::SpecU16:
        printf("%u", c.getValue<u16>());
        break;
    case Ent::SpecU32:
        printf("%u", c.getValue<u32>());
        break;
    case Ent::SpecU64:
        printf("%llu", c.getValue<u64>());
        break;
    case Ent::SpecS8:
        printf("%d", c.getValue<s8>());
        break;
    case Ent::SpecS16:
        printf("%d", c.getValue<s16>());
        break;
    case Ent::SpecS32:
        printf("%d", c.getValue<s32>());
        break;
    case Ent::SpecS64:
        printf("%lld", c.getValue<s64>());
        break;
    case Ent::SpecF32:
        printf("%g", c.getValue<f32>());
        break;
    case Ent::SpecF64:
        printf("%g", c.getValue<f64>());
        break;
    case Ent::SpecF128:
        printf("%Lg", c.getValue<f128>());
        break;
    case Ent::SpecBool:
        printf("%s", c.getValue<bool>() ? "true" : "false");
        break;
    case Ent::SpecChar:
        printf("'%c'", c.getValue<char>());
        break;
    case Ent::SpecWChar:
        if (char* end = utf32to8(c.getValue<wchar_t>(), utf8))
        {
            *end = '\0';
            printf("L'%s'", utf8);
        }
        break;
    case Ent::SpecString:
        printf("\"%s\"", c.getString());
        break;
    case Ent::SpecWString:
        printf("L\"%s\"", c.getString());
        break;
    }
    printf(";\n");
}

void processMethod(Reflect::Method& method)
{
    processType(method.getReturnType());
    if (method.isGetter())
    {
        printf(" get_%s(", method.getName());
    }
    else if (method.isSetter())
    {
        printf(" set_%s(", method.getName());
    }
    else
    {
        printf(" %s(", method.getName());
    }
    for (int i = 0; i < method.getParameterCount(); ++i)
    {
        Reflect::Parameter param(method.getParameter(i));
        processType(param.getType());
        printf(" %s", param.getName());
        if (i + 1 < method.getParameterCount())
        {
            printf(", ");
        }
    }
    printf(")\n");
}

void processInterface(Reflect::Interface& interface)
{
    printf("interface %s\n", interface.getName());
    printf("iid: ");
    printGuid(interface.getIid());
    printf("\n");

    if (interface.getSuperIid() != GUID_NULL)
    {
        printf("piid: ");
        printGuid(interface.getSuperIid());
        printf("\n");
    }

    for (int i = 0; i < interface.getMethodCount(); ++i)
    {
        Reflect::Method method(interface.getMethod(i));
        processMethod(method);
    }

    for (int i = 0; i < interface.getConstantCount(); ++i)
    {
        Reflect::Constant c(interface.getConstant(i));
        processConstant(c);
    }
}

void processModule(Reflect::Module& module)
{
    for (int i = 0; i < module.getInterfaceCount(); ++i)
    {
        Reflect::Interface interface(module.getInterface(i));
        processInterface(interface);
    }

    for (int i = 0; i < module.getConstantCount(); ++i)
    {
        Reflect::Constant c(module.getConstant(i));
        processConstant(c);
    }

    for (int i = 0; i < module.getModuleCount(); ++i)
    {
        Reflect::Module m(module.getModule(i));
        processModule(m);
    }
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0)
        {
            continue;
        }

        struct stat s;
        if (fstat(fd, &s) < 0)
        {
            continue;
        }
        void* ent = malloc(s.st_size);
        if (!ent)
        {
            continue;
        }
        read(fd, ent, s.st_size);
        close(fd);

        printf("# %s\n", argv[i]);
        Reflect r(ent);
        Reflect::Module global(r.getGlobalModule());
        processModule(global);

        free(ent);
    }

    return EXIT_SUCCESS;
}
