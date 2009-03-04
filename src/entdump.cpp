/*
 * Copyright 2008, 2009 Google Inc.
 * Copyright 2007 Nintendo Co., Ltd.
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
#include <unistd.h>
#include <set>
#include <string>
#include <es/ent.h>
#include "utf.h"

using namespace Ent;

std::set<Spec> specs;

void addSpec(const Header* header, Spec spec)
{
    if (isPrimitive(spec))
    {
        return;
    }

    std::pair<std::set<Spec>::iterator, bool> ret = specs.insert(spec);
    if (ret.second == false)
    {
        return;
    }

    // Added a new spec
    Type type = *reinterpret_cast<const Type*>(reinterpret_cast<const char*>(header) + spec);
    switch (type)
    {
    case TypeModule:
    case TypeInterface:
        break;
    case TypeStructure:
        {
            const Structure* st = reinterpret_cast<const Structure*>(reinterpret_cast<const char*>(header) + spec);
            for (uint32_t i = 0; i < st->memberCount; ++i)
            {
                const Member* m = st->getMember(i);
                addSpec(header, m->spec);
            }
        }
        break;
    case TypeException:
        {
            const Exception* st = reinterpret_cast<const Exception*>(reinterpret_cast<const char*>(header) + spec);
            for (uint32_t i = 0; i < st->memberCount; ++i)
            {
                const Member* m = st->getMember(i);
                addSpec(header, m->spec);
            }
        }
        break;
    case TypeArray:
        {
            const Array* a = reinterpret_cast<const Array*>(reinterpret_cast<const char*>(header) + spec);
            addSpec(header, a->spec);
        }
        break;
    case TypeSequence:
        {
            const Sequence* seq = reinterpret_cast<const Sequence*>(reinterpret_cast<const char*>(header) + spec);
            addSpec(header, seq->spec);
        }
        break;
    default:
        fprintf(stderr, "Inv. type: %#x.\n", type);
        exit(EXIT_FAILURE);
        break;
    }
}

const char* getString(const Header* header, uint32_t offset)
{
    assert(offset < header->fileSize);
    if (offset == 0)
    {
        return "";
    }
    else
    {
        return reinterpret_cast<const char*>(header) + offset;
    }
}

void dumpConstant(const Header* header, const Constant* constant)
{
    char utf8[9];

    printf("    Constant(%#x): %#x '%s' %#x: ",
           reinterpret_cast<const uint8_t*>(constant) - reinterpret_cast<const uint8_t*>(header),
           constant->spec,
           getString(header, constant->name),
           constant->value);

    switch (constant->spec)
    {
    case Ent::SpecS8:
    case Ent::SpecS16:
    case Ent::SpecS32:
        printf("(%d)", constant->value);
        break;
    case Ent::SpecS64:
        printf("(%lld)", *reinterpret_cast<const int64_t*>(reinterpret_cast<const uint8_t*>(header) + constant->value));
        break;
    case Ent::SpecU8:
    case Ent::SpecU16:
    case Ent::SpecU32:
        printf("(%u)", constant->value);
        break;
    case Ent::SpecU64:
        printf("(%llu)", *reinterpret_cast<const uint64_t*>(reinterpret_cast<const uint8_t*>(header) + constant->value));
        break;
    case Ent::SpecBool:
        printf("(%s)", constant->value ? "TRUE" : "FALSE");
        break;
    case Ent::SpecF32:
        printf("(%g)", *reinterpret_cast<const float*>(&constant->value));
        break;
    case Ent::SpecF64:
        printf("(%g)", *reinterpret_cast<const double*>(reinterpret_cast<const uint8_t*>(header) + constant->value));
        break;
    case Ent::SpecF128:
        printf("(%Lg)", *reinterpret_cast<const long double*>(reinterpret_cast<const uint8_t*>(header) + constant->value));
        break;
    case Ent::SpecChar:
        printf("(%c)", constant->value);
        break;
    case Ent::SpecWChar:
        if (char* end = utf32to8(constant->value, utf8))
        {
            *end = '\0';
            printf("(%s)", utf8);
        }
        break;
    case Ent::SpecString:
        printf("(%s)", reinterpret_cast<const char*>(header) + constant->value);
        break;
    case Ent::SpecWString:
        printf("(%s)", reinterpret_cast<const char*>(header) + constant->value);
        break;
    default:
        fprintf(stderr, "Inv. const type.\n");
        exit(EXIT_FAILURE);
        break;
    }

    printf("\n");
}

void dumpParam(const Header* header, const Param* param)
{
    printf("        Param: %#x '%s' : %#x\n",
           param->spec,
           getString(header, param->name),
           param->attr);

    addSpec(header, param->spec);
}

void dumpMethod(const Header* header, const Method* method)
{
    printf("    Method(%#x): %#x '%s' : %#x (%u, %u)\n",
           reinterpret_cast<const uint8_t*>(method) - reinterpret_cast<const uint8_t*>(header),
           method->spec,
           getString(header, method->name),
           method->attr,
           method->paramCount,
           method->raiseCount);

    addSpec(header, method->spec);

    for (uint32_t index = 0; index < method->paramCount; ++index)
    {
        const Param* p = method->getParam(index);
        assert(p->spec);
        dumpParam(header, p);
    }

    for (uint32_t index = 0; index < method->raiseCount; ++index)
    {
        Spec spec = method->getRaise(index);
        assert(spec);
        printf("        Raise: %#x\n", spec);
        addSpec(header, spec);
    }
}

void dumpInterface(const Header* header, const Interface* interface)
{
    printf("Interface(%#x): %#x '%s' : %#x (%u, %u, %u)\n",
           reinterpret_cast<const uint8_t*>(interface) - reinterpret_cast<const uint8_t*>(header),
           interface->type,
           getString(header, interface->name),
           interface->module,
           interface->methodCount,
           interface->constCount,
           interface->inheritedMethodCount);
    printf("    fullyQualifiedName: %s\n", getString(header, interface->fullyQualifiedName));
    printf("    fullyQualifiedBaseName: %s\n", getString(header, interface->fullyQualifiedBaseName));

    for (uint32_t index = 0; index < interface->constCount; ++index)
    {
        const Constant* c = interface->getConstant(index);
        assert(c->spec);
        dumpConstant(header, c);
    }

    for (uint32_t index = 0; index < interface->methodCount; ++index)
    {
        Spec spec = interface->getMethod(index);
        assert(spec);
        dumpMethod(header,
                   reinterpret_cast<const Method*>(reinterpret_cast<const char*>(header) + spec));
    }
}

void dumpModule(const Header* header, const Module* module)
{
    printf("Module(%#x): %#x '%s' : %#x (%u, %u, %u)\n",
           reinterpret_cast<const uint8_t*>(module) - reinterpret_cast<const uint8_t*>(header),
           module->type,
           getString(header, module->name),
           module->parent,
           module->moduleCount,
           module->interfaceCount,
           module->constCount);

    for (uint32_t index = 0; index < module->constCount; ++index)
    {
        const Constant* c = module->getConstant(index);
        assert(c->spec);
        dumpConstant(header, c);
    }

    for (uint32_t index = 0; index < module->interfaceCount; ++index)
    {
        Spec spec = module->getInterface(index);
        assert(spec);
        dumpInterface(header,
                      reinterpret_cast<const Interface*>(reinterpret_cast<const char*>(header) + spec));
    }

    for (uint32_t index = 0; index < module->moduleCount; ++index)
    {
        Spec spec = module->getModule(index);
        assert(spec);
        dumpModule(header,
                   reinterpret_cast<const Module*>(reinterpret_cast<const char*>(header) + spec));
    }
}

void dumpHeader(const Header* header)
{
    printf("Header: %#04x %c%c%c %u.%u.%u %u bytes\n",
           header->magic[0], header->magic[1], header->magic[2], header->magic[3],
           header->major, header->minor, header->patch,
           header->fileSize);

    dumpModule(header, reinterpret_cast<const Module*>(&header[1]));
}

void dumpSpec(const Header* header, Spec spec)
{
    Type type = *reinterpret_cast<const Type*>(reinterpret_cast<const char*>(header) + spec);
    switch (type)
    {
    case TypeModule:
    case TypeInterface:
        break;
    case TypeStructure:
        {
            const Structure* st = reinterpret_cast<const Structure*>(reinterpret_cast<const char*>(header) + spec);
            printf("Structure(%#x): %#x : (%u)\n",
                   reinterpret_cast<const uint8_t*>(st) - reinterpret_cast<const uint8_t*>(header),
                   st->type,
                   st->memberCount);
            for (uint32_t i = 0; i < st->memberCount; ++i)
            {
                const Member* m = st->getMember(i);
                printf("    Member: %#x '%s'\n",
                       m->spec,
                       getString(header, m->name));
            }
        }
        break;
    case TypeException:
        {
            const Exception* st = reinterpret_cast<const Exception*>(reinterpret_cast<const char*>(header) + spec);
            printf("Exception(%#x): %#x : (%u)\n",
                   reinterpret_cast<const uint8_t*>(st) - reinterpret_cast<const uint8_t*>(header),
                   st->type,
                   st->memberCount);
            for (uint32_t i = 0; i < st->memberCount; ++i)
            {
                const Member* m = st->getMember(i);
                printf("    Member: %#x '%s'\n",
                       m->spec,
                       getString(header, m->name));
            }
        }
        break;
    case TypeArray:
        {
            const Array* a = reinterpret_cast<const Array*>(reinterpret_cast<const char*>(header) + spec);
            printf("Array(%#x): %#x : %#x (%u) ",
                   reinterpret_cast<const uint8_t*>(a) - reinterpret_cast<const uint8_t*>(header),
                   a->type,
                   a->spec,
                   a->dim);
            for (uint32_t i = 0; i < a->dim; ++i)
            {
                printf("[%u]", a->getRank(i));
            }
            printf("\n");
        }
        break;
    case TypeSequence:
        {
            const Sequence* seq = reinterpret_cast<const Sequence*>(reinterpret_cast<const char*>(header) + spec);
            printf("Sequence(%#x): %#x : %#x (%u)\n",
                   reinterpret_cast<const uint8_t*>(seq) - reinterpret_cast<const uint8_t*>(header),
                   seq->type,
                   seq->spec,
                   seq->max);
        }
        break;
    default:
        fprintf(stderr, "Inv. type: %#x.\n", type);
        exit(EXIT_FAILURE);
        break;
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

        printf("%s\n", argv[i]);

        struct stat s;
        if (fstat(fd, &s) < 0)
        {
            continue;
        }
        void* buf = malloc(s.st_size);
        if (!buf)
        {
            continue;
        }
        read(fd, buf, s.st_size);
        close(fd);

        specs.clear();
        dumpHeader(static_cast<Header*>(buf));
        for (std::set<Spec>::iterator it = specs.begin();
             it != specs.end();
             ++it)
        {
            dumpSpec(static_cast<Header*>(buf), *it);
        }

        free(buf);
    }
}
