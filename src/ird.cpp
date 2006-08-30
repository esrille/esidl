/*
 * Copyright (c) 2006
 * Nintendo Co., Ltd.
 *  
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Nintendo makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#include <assert.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <map>
#include "esidl.h"
#include "binaryStream.h"

using namespace ReflectionFile;

namespace
{

class StringOffsetter : public Visitor
{
    u32 stringOffset;
    std::map<std::string, u32>& stringMap;

public:
    StringOffsetter(std::map<std::string, u32>& stringMap, u32 offset) :
        stringOffset(offset),
        stringMap(stringMap)
    {
    }

    void at(const Identifier* id)
    {
        const std::string& name = id->getName();

        if (name != "")
        {
            if (stringMap.find(name) == stringMap.end())
            {
                stringMap[name] = stringOffset;
                stringOffset += name.length() + 1;
            }
        }
    }

    void at(const NameSpace* ns)
    {
        Identifier* id;
        IdentifierList::Iterator iter = ns->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
        at(static_cast<const Identifier*>(ns));
    }

    void at(const Function* f)
    {
        Identifier* id;
        IdentifierList::Iterator iter = f->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
        at(static_cast<const Identifier*>(f));
    }

    void print()
    {
        std::map<std::string, u32>::iterator it = stringMap.begin();
        for ( ; it != stringMap.end(); ++it)
        {
            std::cout << it->first << ' ' << it->second << std::endl;
        }
        std::cout << std::endl;
    }
};

class TypeOffsetter : public Visitor
{
    u32 typeOffset;
    std::map<const Identifier*, u32>& typeMap;

public:
    TypeOffsetter(std::map<const Identifier*, u32>& typeMap) :
        typeOffset(0),
        typeMap(typeMap)
    {
    }

    void at(const Declarator* decl)
    {
        if (typeMap[decl])
        {
            return;
        }
        typeMap[decl] = typeOffset;

        const Array* a = dynamic_cast<const Array*>(decl);
        if (a)
        {
            typeOffset += sizeof(u32) * 4;
        }
        else
        {
            typeOffset += sizeof(u32) * 2;
        }
        if (decl->isTypename()) // typedef or XXX unnamed type?
        {
            std::cerr << "Typedef is not supported." << std::endl;
            return;
            const_cast<Identifier*>(decl->getType())->accept(*this);
        }
    }

    void at(const Function* f)
    {
        if (typeMap[f])
        {
            return;
        }
        typeMap[f] = typeOffset;
        typeOffset += sizeof(u32) * (f->getNumParams() + 4);
        Identifier* id;
        IdentifierList::Iterator iter = f->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);

            Declarator* dec = dynamic_cast<Declarator*>(id);
            if (dec)
            {
                AttributeList* attrList = const_cast<AttributeList*>(dec->getAttributes());
                if (attrList)
                {
                    Attribute* attr;
                    AttributeList::Iterator iter = attrList->begin();
                    while ((attr = iter.next()))
                    {
                        if (dynamic_cast<InAttribute*>(attr))
                        {
                            typeMap[id] |= IS_IN;
                        }
                        if (dynamic_cast<OutAttribute*>(attr))
                        {
                            typeMap[id] |= IS_OUT;
                        }

                        IidIsAttribute* iidis = dynamic_cast<IidIsAttribute*>(attr);
                        if (iidis)
                        {
                            typeMap[id] |= IS_IID;
                            typeOffset += 4;
                            dec->setIid(f->getParamNum(iidis->getName().c_str()));
                        }

                        SizeIsAttribute* sizeis = dynamic_cast<SizeIsAttribute*>(attr);
                        if (sizeis)
                        {
                            typeMap[id] |= IS_SIZE;
                            typeOffset += 4;
                            dec->setSize(f->getParamNum(sizeis->getName().c_str()));
                        }
                    }
                }
            }
        }
    }

    void at(const Constant* c)
    {
        if (typeMap[c])
        {
            return;
        }
        typeMap[c] = typeOffset;
        typeOffset += sizeof(u32) * 2;
        int size = 0;

        if (dynamic_cast<const Integer*>(c->getType()))
        {
            size = sizeof(u32);
        }
        else if (dynamic_cast<const Float*>(c->getType()))
        {
            size = sizeof(double);
        }
        else if (dynamic_cast<const Character*>(c->getType()))
        {
            if (c->getPointer() == 0)
            {
                size = sizeof(char);
            }
            else
            {
                const Expression* exp = c->getExpression();
                std::string name(static_cast<const char*>(*exp));
                size = name.size() + 1;
            }
        }
        else if (dynamic_cast<const Boolean*>(c->getType()))
        {
            size = sizeof(bool);
        }
        else
        {
            assert(false);
            // undefined constant type
        }

        typeOffset += size;
    }

    void at(const NameSpace* ns)
    {
        if (typeMap[ns])
        {
            return;
        }
        typeMap[ns] = typeOffset;
        typeOffset += sizeof(u32) * (ns->getNumMembers() + 1 + 1);

        Identifier* id;
        IdentifierList::Iterator iter = ns->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
    }

    void at(const Enum* e)
    {
        if (typeMap[e])
        {
            return;
        }
        typeMap[e] = typeOffset;
        typeOffset += sizeof(u32) * (2 * e->getNumMembers() + 3); // XXX
        Identifier* id;
        IdentifierList::Iterator iter = e->begin();
        while ((id = iter.next()))
        {
            // enumulate name-value pairs.
        }
    }

    void at(const Structure* s)
    {
        if (s->getNumMembers() == 0)
        {
            // omit forward declaration
            return;
        }

        if (typeMap[s])
        {
            return;
        }
        typeMap[s] = typeOffset;

        typeOffset += sizeof(u32) * (s->getNumMembers() + 3); // XXX

        Identifier* id;
        IdentifierList::Iterator iter = s->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
    }

    void at(const Interface* i)
    {
        if (typeMap[i])
        {
            return;
        }
        typeMap[i] = typeOffset;

        if (i->isImported())
        {
            typeOffset += sizeof(u32) * 2 + sizeof(Guid);
            typeMap[i] |= IS_IID;
        }
        else
        {
            typeOffset += sizeof(u32) * 2 + sizeof(Guid) +
                          sizeof(u32) * (3 + i->getNumMembers()) + sizeof(Guid);
            Identifier* id;
            IdentifierList::Iterator iter = i->begin();
            while ((id = iter.next()))
            {
                id->accept(*this);
            }
        }
    }

    unsigned getEnd(void)
    {
        return typeOffset;
    }

    void print()
    {
        std::map<const Identifier*, u32>::iterator it = typeMap.begin();
        for ( ; it != typeMap.end(); ++it)
        {
            if (it->second & IS_PRIMITIVE)
            {
                std::cout << std::hex;
                std::cout << it->first->getName() << ' ' << it->second << std::endl;
                std::cout << std::dec;
            }
            else
            {
                std::cout << it->first->getName() << ' ' << (it->second & OFFSET_MASK) << std::endl;
            }
        }
        std::cout << std::endl;
    }
};


class BinaryExpressionVisitor : public Visitor
{
    BinaryStream& out;
    const Constant* c;
    const Expression* exp;
    StringOffsetter& sv;
public:
    BinaryExpressionVisitor(BinaryStream& out, const Constant* c,
        StringOffsetter& sv) :
            out(out), c(c), exp(c->getExpression()), sv(sv)
    {

    }
    void at(const Integer* i)
    {
        if (i->isSigned())
        {
            out.write(static_cast<int>(*exp));
        }
        else
        {
            out.write(static_cast<unsigned>(static_cast<int>(*exp)));
        }
    }
    void at(const Float* f)
    {
        out.write(static_cast<double>(*exp));
    }
    void at(const Character* ch)
    {
        if (c->getPointer() == 0)
        {
            char data = static_cast<const char>(*exp);
            out.write(&data, sizeof(char));
        }
        else
        {
            std::string name(static_cast<const char*>(*exp));
            out.write(name.c_str(), name.size());
            const char end = 0;
            out.write(&end, sizeof(end));
        }
    }
    void at(const Boolean* b)
    {
        bool bit(static_cast<bool>(*exp));
        out.write((const char*) &bit, sizeof(bool));
    }
};


//
// EnumEmitter
//

class EnumEmitter : public Visitor
{
    BinaryStream& out;
    std::map<std::string, u32>& stringMap;
    std::map<const Identifier*, u32>& typeMap;
    int n;
    StringOffsetter& sv;

public:
    EnumEmitter(BinaryStream& out,
            std::map<std::string, u32>& stringMap,
            std::map<const Identifier*, u32>& typeMap,
            StringOffsetter& sv) :
                out(out), stringMap(stringMap), typeMap(typeMap), sv(sv), n(0) { }

    void at(const Constant* c)
    {

        Expression* exp = c->getExpression();
        if (!exp)
        {
            exp = new Literal<int>(n);
            const_cast<Constant*>(c)->setExpression(exp);
        }
        if (static_cast<int>(*exp) != n)
        {
            n = static_cast<int>(*exp);
        }

        out.write(stringMap[c->getName()]);
        out.write(n);
        ++n;
    }

    void at(const Enum* e)
    {
        out.write(stringMap[e->getName()]);

        u32 type = IS_ENUM | typeMap[e];
        out.write(type);
        out.write(e->getNumMembers());

        Identifier* id;
        IdentifierList::Iterator iter = e->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
    }
};

class Emitter : public Visitor
{
    BinaryStream& out;
    std::map<std::string, u32>& stringMap;
    std::map<const Identifier*, u32>& typeMap;
    StringOffsetter& sv;

    static const char Magic[4];
    static const GUID Version;

public:
    Emitter(BinaryStream& out,
            std::map<std::string, u32>& stringMap,
            std::map<const Identifier*, u32>& typeMap,
            StringOffsetter& sv) :
                out(out), stringMap(stringMap), typeMap(typeMap), sv(sv)
    {

    }

    bool isInherited(const Identifier* id) // should be refined. QQQ
    {
        unsigned offset = out.getOffset(); // offset from the beginning of IRD file.

        if ((OFFSET_MASK & typeMap[id]) != offset)
        {
            if (offset < typeMap[id])
            {
                std::cerr << "forward declaration is not supported." << std::endl;
            }

            return true;
        }
        return false;
    }

    void printFileHeader(u32 stringOffset)
    {
        out.write(Magic, sizeof(Magic));

        char dataEncoding = DATA2LSB;
        out.write(&dataEncoding, sizeof(char));

        char padding[3];
        memset(padding, 0 , sizeof(padding));
        out.write(padding, sizeof(padding));
        out.write(&Version);
        out.write(stringOffset);

        // byte offset from end of file header.
        out.resetOffset();
    }

    void at(const Declarator* decl)
    {
        assert((typeMap[decl] & OFFSET_MASK) == out.getOffset());

        int type = typeMap[decl->getType()];
        const Array* a = dynamic_cast<const Array*>(decl);

        if (decl->isTypename()) // typedef or XXX unnamed type?
        {
            std::cerr << "ERROR: Typedef is not supported." << std::endl;
            return;

            // type |= IS_TYPEDEF;
            // out.write(stringMap[decl->getName()]);
            // out.write(type);
        }
        if (a)
        {
            out.write(stringMap[decl->getName()]);
            out.write(IS_ARRAY | typeMap[decl]);
            switch (decl->getPointer())
            {
              case 1:
                type |= IS_STAR;
                break;
              case 2:
                type |= IS_STARSTAR;
                break;
            }
            if (dynamic_cast<const Ref*>(decl))
            {
                type |= IS_REFERENCE;
            }

            out.write(type);

            Attribute* attr;
            AttributeList::Iterator iter = a->begin();
            while ((attr = iter.next()))
            {
                BoundAttribute* bound;
                if ((bound = dynamic_cast<BoundAttribute*>(attr)))
                {
                    const Expression* exp = bound->getExpression();
                    u32 numAttributes =  static_cast<int>(*exp);
                    out.write(numAttributes);
                }
            }
        }
        else
        {
            out.write(stringMap[decl->getName()]);

            switch (decl->getPointer())
            {
              case 1:
                type |= IS_STAR;
                break;
              case 2:
                type |= IS_STARSTAR;
                break;
            }

            if (dynamic_cast<const Ref*>(decl))
            {
                type |= IS_REFERENCE;
            }

            out.write(type);

            if (0 <= decl->getIid())
            {
                out.write(decl->getIid());
            }

            if (0 <= decl->getSize())
            {
                out.write(decl->getSize());
            }
        }
    }

    void at(const Function* f)
    {
        assert((typeMap[f] & OFFSET_MASK) == out.getOffset());

        out.write(stringMap[f->getName()]);
        u32 type = IS_FUNCTION | typeMap[f];
        out.write(type);

        Identifier* returnType = const_cast<Identifier*>(f->getType());
        assert(typeMap.find(returnType) != typeMap.end());
        type = typeMap[returnType];
        if (f->getPointer())
        {
            type |= IS_STAR | IS_OUT;
        }
        out.write(type);

        out.write(f->getNumParams());

        Identifier* id;
        IdentifierList::Iterator param = f->begin();
        while ((id = param.next()))
        {
            out.write(typeMap[id]);
        }

        IdentifierList::Iterator iter = f->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
    }

    void at(const Constant* c)
    {
        assert((typeMap[c] & OFFSET_MASK) == out.getOffset());

        out.write(stringMap[c->getName()]);

        u32 type = IS_CONSTANT | typeMap[c->getType()];

        switch (c->getPointer())
        {
          case 1:
            type |= IS_STAR;
            break;
          case 2:
            type |= IS_STARSTAR;
            break;
        }

        if (dynamic_cast<const Ref*>(c))
        {
            type |= IS_REFERENCE;
        }

        out.write(type);

        BinaryExpressionVisitor bev(out, c, sv);
        Identifier* id = const_cast<Identifier*>(c->getType());
        id->accept(bev);

    }

    void at(const NameSpace* ns)
    {
        assert((typeMap[ns] & OFFSET_MASK) == out.getOffset());

        out.write(ns->getNumInterfaces());

        Identifier* id;
        IdentifierList::Iterator interface = ns->begin();
        while ((id = interface.next()))
        {
            if (dynamic_cast<Interface*>(id))
            {
                out.write(typeMap[id]);
            }
        }

        out.write(ns->getNumMembers() - ns->getNumInterfaces());
        IdentifierList::Iterator member = ns->begin();
        while ((id = member.next()))
        {
            if (dynamic_cast<Interface*>(id) == 0)
            {
                out.write(typeMap[id]);
            }
        }

        IdentifierList::Iterator iter = ns->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
    }

    void at(const Enum* e)
    {
        assert(typeMap[e] == out.getOffset());

        EnumEmitter ev(out, stringMap, typeMap, sv);
        const_cast<Enum*>(e)->accept(ev);
    }

    void at(const Structure* s)
    {
        assert((typeMap[s] & OFFSET_MASK) == out.getOffset());

        out.write(stringMap[s->getName()]);
        u32 type = IS_STRUCTURE | typeMap[s];
        out.write(type);
        out.write(s->getNumMembers());

        Identifier* id;
        IdentifierList::Iterator member = s->begin();
        while ((id = member.next()))
        {
            out.write(typeMap[id]);
        }

        IdentifierList::Iterator iter = s->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
    }

    bool writeIid(const Interface* i)
    {
        Attribute* attr;
        AttributeList::Iterator ai = i->beginAttributes();
        while ((attr = ai.next()))
        {
            UuidAttribute* uuid;
            if ((uuid = dynamic_cast<UuidAttribute*>(attr)))
            {
                const Guid* guid = uuid->getValue();
                out.write(guid);

                return true;
            }
        }
        return false;
    }

    void at(const Interface* i)
    {
        assert((typeMap[i] & OFFSET_MASK) == out.getOffset());

        out.write(stringMap[i->getName()]);

        u32 type = IS_INTERFACE | typeMap[i];
        out.write(type);
        writeIid(i);

        if (i->isImported())
        {
            return;
        }

        // Write method count including super class methods.
        u32 numMethods(0);
        const Interface* super(i);
        do
        {
            numMethods += super->getNumMethods();
        } while ((super = super->getParentInterface()));
        out.write(numMethods);

        out.write(i->getNumMembers() - i->getNumMethods());
        Identifier* id;
        IdentifierList::Iterator member = i->begin();
        u32 n = 0;
        while ((id = member.next()))
        {
            if (!dynamic_cast<Function*>(id))
            {
                ++n;
                out.write(typeMap[id]);
            }
        }
        assert(n == i->getNumMembers() - i->getNumMethods());

        const Interface* parent = i->getParentInterface();
        if (parent)
        {
            writeIid(parent);
        }
        else
        {
            out.write(&GUID_NULL, sizeof(Guid));
        }

        out.write(i->getNumMethods());
        IdentifierList::Iterator method = i->begin();
        while ((id = method.next()))
        {
            if (dynamic_cast<Function*>(id))
            {
                out.write(typeMap[id] | IS_FUNCTION);
            }
        }

        IdentifierList::Iterator iter = i->begin();
        while ((id = iter.next()))
        {
            if (isInherited(id))
            {
                continue;
            }
            id->accept(*this);
        }
    }

};

class StringEmitter : public Visitor
{
    std::map<std::string, u32>& stringMap;
    BinaryStream& out;

public:
    StringEmitter(BinaryStream& out, std::map<std::string, u32>& stringMap) :
        out(out),
        stringMap(stringMap)
    {
    }

    void at(const Identifier* id)
    {
        const std::string& name = id->getName();
        if (name != "" && stringMap[name] == out.getOffset())
        {
            out.write(name);
        }
    }

    void at(const NameSpace* ns)
    {
        Identifier* id;
        IdentifierList::Iterator iter = ns->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
        at(static_cast<const Identifier*>(ns));
    }

    void at(const Function* f)
    {
        Identifier* id;
        IdentifierList::Iterator iter = f->begin();
        while ((id = iter.next()))
        {
            id->accept(*this);
        }
        at(static_cast<const Identifier*>(f));
    }

};

const char Emitter::Magic[4] = {0x7f, 'I', 'R', 'D'};
const GUID Emitter::Version =
{
    0xad4883d3, 0x4efa, 0x4593, { 0x80, 0x11, 0x3e, 0x4e, 0x17, 0xc7, 0x49, 0x55 }
};

}   // unnamed namespace

void GenerateIrd(std::ostream& out)
{
    std::map<std::string, u32> stringMap;
    std::map<const Identifier*, u32> typeMap;

    //
    // Sets default offsets for types.
    //
    typeMap[NameSpace::newInteger(8, true)] = TAG_S8 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(16, true)] = TAG_S16 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(32, true)] = TAG_S32 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(64, true)] = TAG_S64 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(8, false)] = TAG_U8 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(16, false)] = TAG_U16 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(32, false)] = TAG_U32 | IS_PRIMITIVE;
    typeMap[NameSpace::newInteger(64, false)] = TAG_U64 | IS_PRIMITIVE;
    typeMap[NameSpace::newFloat(32)] = TAG_F32 | IS_PRIMITIVE;
    typeMap[NameSpace::newFloat(64)] = TAG_F64 | IS_PRIMITIVE;
    typeMap[NameSpace::newBoolean()] = TAG_BOOLEAN | IS_PRIMITIVE;
    typeMap[NameSpace::newCharacter()] = TAG_CHAR | IS_PRIMITIVE;
    typeMap[NameSpace::newVoid()] = TAG_VOID | IS_PRIMITIVE;
    typeMap[NameSpace::newUuid()] = TAG_UUID | IS_PRIMITIVE;

    //
    // Calculate offsets for types.
    //
    TypeOffsetter tv(typeMap);
    NameSpace::global().accept(tv);
    // std::cout << "type offset" << std::endl;
    // tv.print();

    //
    // Calculate offsets for character strings.
    //
    StringOffsetter sv(stringMap, tv.getEnd());
    NameSpace::global().accept(sv);
    // sv.print();

    //
    // Emit
    //
    BinaryStream binaryOut(out);
    Emitter ev(binaryOut, stringMap, typeMap, sv);
    ev.printFileHeader(tv.getEnd());
    NameSpace::global().accept(ev);

    StringEmitter se(binaryOut, stringMap);
    NameSpace::global().accept(se);
}
