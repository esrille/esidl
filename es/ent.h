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

#ifndef NINTENDO_ES_ENT_H_INCLUDED
#define NINTENDO_ES_ENT_H_INCLUDED

#include <assert.h>
#include <stdint.h>
#include <new>

// Amend namespace pollution
#ifdef minor
#undef minor
#endif
#ifdef major
#undef major
#endif

namespace Ent
{
    // Primitive types
    typedef uint32_t Spec;
    static const Spec SpecPrimitive = 0x80000000u;
    static const Spec SpecS8 = SpecPrimitive | 0u;
    static const Spec SpecS16 = SpecPrimitive | 1u;
    static const Spec SpecS32 = SpecPrimitive | 2u;
    static const Spec SpecS64 = SpecPrimitive | 3u;
    static const Spec SpecU8 = SpecPrimitive | 4u;
    static const Spec SpecU16 = SpecPrimitive | 5u;
    static const Spec SpecU32 = SpecPrimitive | 6u;
    static const Spec SpecU64 = SpecPrimitive | 7u;
    static const Spec SpecF32 = SpecPrimitive | 8u;
    static const Spec SpecF64 = SpecPrimitive | 9u;
    static const Spec SpecF128 = SpecPrimitive | 10u;
    static const Spec SpecBool = SpecPrimitive | 11u;
    static const Spec SpecChar = SpecPrimitive | 12u;
    static const Spec SpecWChar = SpecPrimitive | 13u;
    static const Spec SpecVoid = SpecPrimitive | 14u;
    static const Spec SpecUuid = SpecPrimitive | 15u;
    static const Spec SpecString = SpecPrimitive | 16u;
    static const Spec SpecWString = SpecPrimitive | 17u;
    static const Spec SpecAny = SpecPrimitive | 18u;
    static const Spec SpecObject = SpecPrimitive | 19u;
    static const Spec SpecFixed = SpecPrimitive | 20u;
    static const Spec SpecValue = SpecPrimitive | 21u;
    static const Spec SpecVariant = SpecPrimitive | 22u;
    static const int MaxSpec = 23;

    // Non-primitive types
    typedef uint32_t Type;
    static const Type TypeModule = 0u;
    static const Type TypeInterface = 1u;
    static const Type TypeStructure = 2u;
    static const Type TypeException = 3u;
    static const Type TypeEnum = 4u;
    static const Type TypeArray = 5u;
    static const Type TypeSequence = 6u;
    static const int MaxType = 7;

    // Attribute bits
    static const uint32_t AttrMask =                 0x00000003;
    static const uint32_t AttrOperation =            0x00000000;
    static const uint32_t AttrGetter =               0x00000001;
    static const uint32_t AttrSetter =               0x00000002;
    static const uint32_t AttrIn =                   0x00000000;
    static const uint32_t AttrOut =                  0x00000001;
    static const uint32_t AttrInOut =                0x00000002;

    // Extended attribute bits
    // [IndexCreator], [IndexDeleter], [IndexGetter] and [IndexSetter]
    static const uint32_t IndexMask =                0x0000003c;
    static const uint32_t IndexCreator =             0x00000004;
    static const uint32_t IndexDeleter =             0x00000008;
    static const uint32_t IndexGetter =              0x00000010;
    static const uint32_t IndexSetter =              0x00000020;
    // [NameCreator], [NameDeleter], [NameGetter] and [NameSetter]
    static const uint32_t NameMask =                 0x000003c0;
    static const uint32_t NameCreator =              0x00000040;
    static const uint32_t NameDeleter =              0x00000080;
    static const uint32_t NameGetter =               0x00000100;
    static const uint32_t NameSetter =               0x00000200;
    // [NoIndexingOperations]
    static const uint32_t NoIndexingOperations =     0x00000400;
    // [Callback]
    static const uint32_t CallbackMask =             0x00001800;
    static const uint32_t Callback =                 0x00001800;
    static const uint32_t CallbackIsFunctionOnly =   0x00000800;
    static const uint32_t CallbackIsPropertyOnly =   0x00001000;
    // [NoInterfaceObject]
    static const uint32_t NoInterfaceObject =        0x00002000;
    // [PrototypeRoot ]
    static const uint32_t PrototypeRoot =            0x00004000;
    // [Null]
    static const uint32_t NullIsEmpty =              0x00008000;
    static const uint32_t NullIsNull =               0x00010000;
    // [Undefined]
    static const uint32_t UndefinedIsEmpty =         0x00020000;
    static const uint32_t UndefinedIsNull =          0x00040000;
    // [Stringifies]
    static const uint32_t Stringifies =              0x00080000;
    // [Replaceable]
    static const uint32_t Replaceable =              0x00100000;
    // [Callable]
    static const uint32_t Callable =                 0x00200000;
    // [Optional]
    static const uint32_t Optional =                 0x00400000;
    // [Variadic]
    static const uint32_t Variadic =                 0x00800000;

    static bool isPrimitive(Spec spec)
    {
        return (spec & SpecPrimitive) ? true : false;
    }

    /** An interface reflection file begins with a Header.
     */
    struct Header
    {
        static const uint8_t Major = 0;
        static const uint8_t Minor = 1;
        static const uint8_t Patch = 0;

        char     magic[4];  // magic number (0x7f, 'E', 'N', 'T')
        uint8_t  major;
        uint8_t  minor;
        uint8_t  patch;
        uint8_t  reserved;
        uint32_t fileSize;
        // Module global;

        Header(size_t fileSize) :
            major(Major),
            minor(Minor),
            patch(Patch),
            reserved(0u),
            fileSize(fileSize)
        {
            magic[0] = 0x7f,
            magic[1] = 'E';
            magic[2] = 'N';
            magic[3] = 'T';
        }
    };

    struct Constant
    {
        Spec     spec;
        uint32_t name;
        uint32_t value;

        Constant(Spec spec, uint32_t name, uint32_t value) :
            spec(spec),
            name(name),
            value(value)
        {
            assert(isPrimitive(spec));
        }

        static size_t getSize()
        {
            return sizeof(Constant);
        }
    };

    /** The Module record for the global name space comes after the Header.
     */
    struct Module
    {
        Type     type;  // TypeModule
        uint32_t name;
        Spec     parent;
        uint32_t moduleCount;
        uint32_t interfaceCount;
        uint32_t constCount;
        // Spec modules[moduleCount];
        // Spec interfaces[interfaceCount];
        // Constant consts[constCount];

        Module(uint32_t name, Spec parent, uint32_t moduleCount, uint32_t interfaceCount,
               uint32_t constCount) :
            type(TypeModule),
            name(name),
            parent(parent),
            moduleCount(moduleCount),
            interfaceCount(interfaceCount),
            constCount(constCount)
        {
        }

        void addModule(Spec spec)
        {
            assert(spec);
            Spec* p = reinterpret_cast<Spec*>(&this[1]);
            uint32_t i;
            for (i = 0; i < moduleCount; ++i, ++p)
            {
                if (*p == 0)
                {
                    *p = spec;
                    break;
                }
            }
            assert(i < moduleCount);
        }

        Spec getModule(uint32_t index) const
        {
            assert(index < moduleCount);
            const Spec* p = reinterpret_cast<const Spec*>(&this[1]);
            return p[index];
        }

        void addInterface(Spec spec)
        {
            assert(spec);
            Spec* p = reinterpret_cast<Spec*>(&this[1]);
            p += moduleCount;
            uint32_t i;
            for (i = 0; i < interfaceCount; ++i, ++p)
            {
                if (*p == 0)
                {
                    *p = spec;
                    break;
                }
            }
            assert(i < interfaceCount);
        }

        Spec getInterface(uint32_t index) const
        {
            assert(index < interfaceCount);
            const Spec* p = reinterpret_cast<const Spec*>(&this[1]);
            return p[moduleCount + index];
        }

        void addConstant(Spec spec, uint32_t name, uint32_t value)
        {
            assert(spec);
            Spec* p = reinterpret_cast<Spec*>(&this[1]);
            p += moduleCount;
            p += interfaceCount;
            Constant* c = reinterpret_cast<Constant*>(p);
            uint32_t i;
            for (i = 0; i < constCount; ++i, ++c)
            {
                if (c->spec == 0)
                {
                    new(c) Constant(spec, name, value);
                    break;
                }
            }
            assert(i < constCount);
        }

        const Constant* getConstant(uint32_t index) const
        {
            assert(index < constCount);
            const Spec* p = reinterpret_cast<const Spec*>(&this[1]);
            p += moduleCount;
            p += interfaceCount;
            const Constant* c = reinterpret_cast<const Constant*>(p);
            return c + index;
        }

        static size_t getSize(uint32_t moduleCount, uint32_t interfaceCount, uint32_t constCount)
        {
            return sizeof(Module) +
                   sizeof(Spec) * (moduleCount + interfaceCount) +
                   sizeof(Constant) * constCount;
        }
    };

    struct Interface
    {
        Type     type;  // TypeInterface
        uint32_t name;
        uint32_t attr;
        uint32_t fullyQualifiedName;
        uint32_t fullyQualifiedBaseName;
        Spec     module;
        uint32_t methodCount;
        uint32_t constCount;
        uint32_t inheritedMethodCount;
        Spec     constructor;
        // Spec methods[methodCount];
        // Constant consts[constCount];

        Interface(uint32_t name,
                  uint32_t fullyQualifiedName,
                  uint32_t fullyQualifiedBaseName,
                  Spec module,
                  uint32_t methodCount, uint32_t constCount, uint32_t inheritedMethodCount,
                  Spec constructor) :
            type(TypeInterface),
            name(name),
            attr(0),
            fullyQualifiedName(fullyQualifiedName),
            fullyQualifiedBaseName(fullyQualifiedBaseName),
            module(module),
            methodCount(methodCount),
            constCount(constCount),
            inheritedMethodCount(inheritedMethodCount),
            constructor(constructor)
        {
        }

        void addMethod(Spec spec)
        {
            assert(spec);
            Spec* p = reinterpret_cast<Spec*>(&this[1]);
            uint32_t i;
            for (i = 0; i < methodCount; ++i, ++p)
            {
                if (*p == 0)
                {
                    *p = spec;
                    break;
                }
            }
            assert(i < methodCount);
        }

        Spec getMethod(uint32_t index) const
        {
            assert(index < methodCount);
            const Spec* p = reinterpret_cast<const Spec*>(&this[1]);
            return p[index];
        }

        void addConstant(Spec spec, uint32_t name, uint32_t value)
        {
            assert(spec);
            Spec* p = reinterpret_cast<Spec*>(&this[1]);
            p += methodCount;
            Constant* c = reinterpret_cast<Constant*>(p);
            uint32_t i;
            for (i = 0; i < constCount; ++i, ++c)
            {
                if (c->spec == 0)
                {
                    new(c) Constant(spec, name, value);
                    break;
                }
            }
            assert(i < constCount);
        }

        const Constant* getConstant(uint32_t index) const
        {
            assert(index < constCount);
            const Spec* p = reinterpret_cast<const Spec*>(&this[1]);
            p += methodCount;
            const Constant* c = reinterpret_cast<const Constant*>(p);
            return c + index;
        }

        static size_t getSize(uint32_t methodCount, uint32_t constCount)
        {
            return sizeof(Interface) +
                   sizeof(Spec) * methodCount +
                   sizeof(Constant) * constCount;
        }
    };

    struct Param
    {
        Spec     spec;
        uint32_t name;
        uint32_t attr;

        bool isInput() const
        {
            return ((attr & AttrMask) == AttrIn) ? true : false;
        }

        bool isOutput() const
        {
            return ((attr & AttrMask) == AttrOut) ? true : false;
        }

        bool isInOut() const
        {
            return ((attr & AttrMask) == AttrInOut) ? true : false;
        }

        bool isVariadic() const
        {
            return attr & Variadic;
        }
    };

    struct Method
    {
        Spec     spec;  // return type
        uint32_t name;
        uint32_t attr;
        uint32_t paramCount;
        uint32_t raiseCount;
        // Param params[paramCount];
        // Spec raises[raiseCount];

        Method(Spec spec, uint32_t name, uint32_t attr, uint32_t paramCount, uint32_t raiseCount) :
            spec(spec),
            name(name),
            attr(attr),
            paramCount(paramCount),
            raiseCount(raiseCount)
        {
        }

        void addParam(Spec spec, uint32_t name, uint32_t attr)
        {
            assert(spec);
            Param* p = reinterpret_cast<Param*>(&this[1]);
            uint32_t i;
            for (i = 0; i < paramCount; ++i, ++p)
            {
                if (p->spec == 0)
                {
                    p->spec = spec;
                    p->name = name;
                    p->attr = attr;
                    break;
                }
            }
            assert(i < paramCount);
        }

        const Param* getParam(uint32_t index) const
        {
            assert(index < paramCount);
            const Param* p = reinterpret_cast<const Param*>(&this[1]);
            return p + index;
        }

        void addRaise(Spec spec)
        {
            Param* params = reinterpret_cast<Param*>(&this[1]);
            Spec* raises = reinterpret_cast<Spec*>(&params[paramCount]);
            uint32_t i;
            for (i = 0; i < raiseCount; ++i, ++raises)
            {
                if (*raises == 0)
                {
                    *raises = spec;
                    break;
                }
            }
            assert(i < raiseCount);
        }

        Spec getRaise(uint32_t index) const
        {
            assert(index < raiseCount);
            const Param* params = reinterpret_cast<const Param*>(&this[1]);
            const Spec* raises = reinterpret_cast<const Spec*>(&params[paramCount]);
            return raises[index];
        }

        static size_t getSize(uint32_t paramCount, uint32_t raiseCount)
        {
            return sizeof(Method) +
                   sizeof(Param) * paramCount +
                   sizeof(Spec) * raiseCount;
        }

        bool isOperation() const
        {
            return ((attr & AttrMask) == AttrOperation) ? true : false;
        }

        bool isGetter() const
        {
            return ((attr & AttrMask) == AttrGetter) ? true : false;
        }

        bool isSetter() const
        {
            return ((attr & AttrMask) == AttrSetter) ? true : false;
        }

        bool isIndexGetter() const
        {
            return (attr & IndexGetter) ? true : false;
        }

        bool isIndexSetter() const
        {
            return (attr & IndexSetter) ? true : false;
        }

        bool isNameGetter() const
        {
            return (attr & NameGetter) ? true : false;
        }

        bool isNameSetter() const
        {
            return (attr & NameSetter) ? true : false;
        }
    };

    struct Member
    {
        Spec     spec;
        uint32_t name;
    };

    struct Structure
    {
        Type     type;  // TypeStructure
        uint32_t memberCount;
        // Member members[memberCount];

        Structure(uint32_t memberCount) :
            type(TypeStructure),
            memberCount(memberCount)
        {
        }

        void addMember(Spec spec, uint32_t name)
        {
            assert(spec);
            Member* p = reinterpret_cast<Member*>(&this[1]);
            uint32_t i;
            for (i = 0; i < memberCount; ++i, ++p)
            {
                if (p->spec == 0)
                {
                    p->spec = spec;
                    p->name = name;
                    break;
                }
            }
            assert(i < memberCount);
        }

        const Member* getMember(uint32_t index) const
        {
            assert(index < memberCount);
            const Member* p = reinterpret_cast<const Member*>(&this[1]);
            return p + index;
        }

        static size_t getSize(uint32_t memberCount)
        {
            return sizeof(Structure) + sizeof(Member) * memberCount;
        }
    };

    struct Exception
    {
        Type     type;  // TypeException
        uint32_t memberCount;
        // Member members[memberCount];

        Exception(uint32_t memberCount) :
            type(TypeException),
            memberCount(memberCount)
        {
        }

        void addMember(Spec spec, uint32_t name)
        {
            assert(spec);
            Member* p = reinterpret_cast<Member*>(&this[1]);
            uint32_t i;
            for (i = 0; i < memberCount; ++i, ++p)
            {
                if (p->spec == 0)
                {
                    p->spec = spec;
                    p->name = name;
                    break;
                }
            }
            assert(i < memberCount);
        }

        const Member* getMember(uint32_t index) const
        {
            const Member* p = reinterpret_cast<const Member*>(&this[1]);
            return p + index;
        }

        static size_t getSize(uint32_t memberCount)
        {
            return sizeof(Exception) + sizeof(Member) * memberCount;
        }
    };

    struct Enum
    {
        Type     type;  // TypeEnum
        uint32_t enumCount;
        // uint32_t enums[enumCount];

        Enum(uint32_t enumCount) :
            type(TypeEnum),
            enumCount(enumCount)
        {
        }

        void add(uint32_t name)
        {
            uint32_t* p = reinterpret_cast<uint32_t*>(&this[1]);
            uint32_t i;
            for (i = 0; i < enumCount; ++i, ++p)
            {
                if (*p == 0)
                {
                    *p = name;
                    break;
                }
            }
            assert(i < enumCount);
        }

        uint32_t get(uint32_t index) const
        {
            assert(index < enumCount);
            const uint32_t* p = reinterpret_cast<const uint32_t*>(&this[1]);
            return p[index];
        }

        static size_t getSize(uint32_t enumCount)
        {
            return sizeof(Enum) + sizeof(uint32_t) * enumCount;
        }
    };

    struct Array
    {
        Type     type;  // TypeArray
        Spec     spec;
        uint32_t dim;
        // uint32_t rank[dim];

        Array(Spec spec, uint32_t dim) :
            type(TypeArray),
            spec(spec),
            dim(dim)
        {
            assert(0 < dim);
        }

        void setRank(uint32_t rank)
        {
            uint32_t* p = reinterpret_cast<uint32_t*>(&this[1]);
            uint32_t i;
            for (i = 0; i < dim; ++i, ++p)
            {
                if (*p == 0)
                {
                    *p = rank;
                    break;
                }
            }
            assert(i < dim);
        }

        uint32_t getRank(uint32_t index) const
        {
            assert(index < dim);
            const uint32_t* p = reinterpret_cast<const uint32_t*>(&this[1]);
            return p[index];
        }

        static size_t getSize(uint32_t dim)
        {
            return sizeof(Array) + sizeof(uint32_t) * dim;
        }
    };

    struct Sequence
    {
        Type     type;  // TypeSequence
        Spec     spec;
        uint64_t max;

        Sequence(Spec spec, uint64_t max = 0) :
            type(TypeSequence),
            spec(spec),
            max(max)
        {
        }

        static size_t getSize()
        {
            return sizeof(Sequence);
        }
    };
}

#endif  // #ifndef NINTENDO_ES_ENT_H_INCLUDED
