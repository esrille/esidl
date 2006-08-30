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

#include <typeinfo>
#include <iostream>
#include <iomanip>
#include "header.h"

namespace
{

//
// Indentation
//

class Indentation
{
    int level;
    static int last;
public:
    Indentation(const NameSpace* n)
    {
        level = 0;
        while ((n = n->parent()))
        {
            ++level;
        }
        last = level;
    }
    Indentation() : level(last) { }

    friend std::ostream& operator<<(std::ostream& s, const Indentation t);
};

int Indentation::last;

std::ostream& operator<<(std::ostream& s, const Indentation t)
{
    s << '\n';
    int level = t.level;
    while (0 <= --level)
    {
        s << "    ";
    }
    return s;
}

inline Indentation Indent(const NameSpace* n)
{
    return Indentation(n);
}

inline Indentation Indent()
{
    return Indentation();
}

}   // unnamed namespace

//
// Common Handlers
//

void ImportHandler(std::ostream& out, const NameSpace* n)
{
    bool cr(false);
    Import* inc;
    ImportList::Iterator i = n->beginImports();
    while ((inc = i.next()))
    {
        if (inc->isImported())
        {
            continue;
        }

        out << "#include " << inc->getHeaderName();
        if (i.hasNext())
        {
            out << '\n';
        }
        cr = true;
    }
    if (cr)
    {
        out << '\n';
    }
}

void ParamListHandler(std::ostream& out, IdentifierList::Iterator& iter);

void DeclaratorHandler(std::ostream& out, const Declarator* d, bool interface = false)
{
    const Array* a = dynamic_cast<const Array*>(d);
    const Function* f = dynamic_cast<const Function*>(d);
    bool paren = f && !interface;
    int constant = 0;

    AttributeList* attrList = const_cast<AttributeList*>(d->getAttributes());
    if (attrList)
    {
        Attribute* attr;
        AttributeList::Iterator iter = attrList->begin();
        while ((attr = iter.next()))
        {
            if (dynamic_cast<OutAttribute*>(attr))
            {
                --constant;
            }
            if (dynamic_cast<InAttribute*>(attr))
            {
                ++constant;
            }
        }
    }

    if (0 < constant && (0 < d->getPointer() || dynamic_cast<const Ref*>(d)))
    {
        out << "const ";
    }

    if (d->isTypename())
    {
        out << "typedef ";
        TypeSpecVisitor v(out);
        Identifier* id = const_cast<Identifier*>(d->getType());
        id->accept(v);
    }
    else
    {
        TypeNameVisitor v(out);
        Identifier* id = const_cast<Identifier*>(d->getType());
        id->accept(v);
    }

    int pointer = d->getPointer();
    while (0 <= --pointer)
    {
        out << '*';
    }

    if (dynamic_cast<const Ref*>(d))
    {
        out << '&';
    }

    if (paren)
    {
        out << " (*" << d->getName() << ')';
    }
    else
    {
        out << ' ' << d->getName();
    }

    if (f)
    {
        IdentifierList::Iterator iter = f->begin();
        ParamListHandler(out, iter);
    }

    if (a)
    {
        Attribute* attr;
        AttributeList::Iterator iter = a->begin();
        while ((attr = iter.next()))
        {
            BoundAttribute* bound;
            if ((bound = dynamic_cast<BoundAttribute*>(attr)))
            {
                const Expression* exp = bound->getExpression();
                out << '[';
                if (exp)
                {
                    out << static_cast<int>(*exp);
                }
                out << ']';
            }
        }
    }
}

void ParamListHandler(std::ostream& out, IdentifierList::Iterator& iter)
{
    out << "(";
    Identifier* param;
    while ((param = iter.next()))
    {
        DeclaratorHandler(out, dynamic_cast<Declarator*>(param));
        if (iter.hasNext())
        {
            out << ", ";
        }
    }
    out << ")";
}

//
// TypeNameVisitor
//

void TypeNameVisitor::at(const Identifier* id)
{
    out << id->getName();
}

void TypeNameVisitor::at(const Integer* i)
{
    if (!i->isSigned())
    {
        out << "unsigned ";
    }
    else if (i->getBits() == 8)
    {
        out << "signed ";
    }
    switch (i->getBits())
    {
      case 8:
        out << "char";
        break;
      case 16:
        out << "short";
        break;
      case 32:
        out << "int";
        break;
      case 64:
        out << "long long";
        break;
    }
}

void TypeNameVisitor::at(const Float* f)
{
    switch (f->getBits())
    {
      case 32:
        out << "float";
        break;
      case 64:
        out << "double";
        break;
    }
}

void TypeNameVisitor::at(const Character* c)
{
    out << "char";
}

void TypeNameVisitor::at(const Boolean* b)
{
    out << "bool";
}

void TypeNameVisitor::at(const Void* v)
{
    out << "void";
}

void TypeNameVisitor::at(const UuidType* v)
{
    out << "Guid";
}

//
// Structure
//

void StructureVisitor::at(const Declarator* d)
{
    DeclaratorHandler(out, d);
}

//
// EnumVisitor
//

void EnumVisitor::at(const Constant* c)
{
    out << c->getName();
    Expression* exp = c->getExpression();

    if (!exp)
    {
        exp = new Literal<int>(n);
        const_cast<Constant*>(c)->setExpression(exp);
    }
    if (static_cast<int>(*exp) != n)
    {
        n = static_cast<int>(*exp);
        out << " = " << std::setbase(exp->getBase()) << std::showbase << n << std::noshowbase << std::setbase(10);
    }
    ++n;
}

//
// TypeSpecVisitor
//

void TypeSpecVisitor::at(const Structure* structure)
{
    // struct
    out << "struct";
    if (structure->getName() != "")
    {
        out << ' ' << structure->getName();
    }
    out << Indent() << "{";

    StructureVisitor v(out);
    Identifier* id;
    IdentifierList::Iterator iter = structure->begin();
    while ((id = iter.next()))
    {
        out << Indent(structure);

        // Structure may contain the following nodes:
        //  - field_declarator
        id->accept(v);
        out << ';';
    }

    out << Indent(structure->parent()) << "}";
}

void TypeSpecVisitor::at(const Enum* enumeration)
{
    // enum
    out << "enum";
    if (enumeration->getName() != "")
    {
        out << ' ' << enumeration->getName();
    }
    out << Indent() << "{";

    EnumVisitor v(out);
    Identifier* id;
    IdentifierList::Iterator iter = enumeration->begin();
    while ((id = iter.next()))
    {
        if (id->getComment())
        {
            out << Indent(enumeration) << id->getComment();
        }
        out << Indent(enumeration);

        // Enumeration may contain the following nodes:
        //  - enum_identifier
        id->accept(v);
        if (iter.hasNext())
        {
            out << ',';
        }
    }

    out << Indent(enumeration->parent()) << "}";
}

//
// NameSpaceVisitor
//

void NameSpaceVisitor::at(const Constant* c)
{
    out << "static const ";

    TypeNameVisitor tnv(out);
    Identifier* id = const_cast<Identifier*>(c->getType());
    id->accept(tnv);

    int pointer = c->getPointer();
    while (0 <= --pointer)
    {
        out << '*';
    }
    out << " " << c->getName();
    if (c->getExpression())
    {
        out << " = ";
        ExpressionVisitor v(out, c);
        id->accept(v);
    }
    out << ';';
}

void NameSpaceVisitor::at(const Declarator* d)
{
    DeclaratorHandler(out, d);
    out << ';';
}

void NameSpaceVisitor::at(const Structure* s)
{
    TypeSpecVisitor::at(s);
    out << ';';
}

void NameSpaceVisitor::at(const Enum* e)
{
    TypeSpecVisitor::at(e);
    out << ';';
}

void NameSpaceVisitor::at(const Interface* interface)
{
    // #include
    ImportHandler(out, interface);

    // Guid
    out << "const Guid IID_" << interface->getName() << " =\n{\n";
    Attribute* attr;
    AttributeList::Iterator ai = interface->beginAttributes();
    while ((attr = ai.next()))
    {
        UuidAttribute* uuid;
        if ((uuid = dynamic_cast<UuidAttribute*>(attr)))
        {
            const Guid* guid = uuid->getValue();
            out << "   " << std::setbase(16) << std::setfill('0') <<
                "0x" << std::setw(8) << guid->Data1 << ", " <<
                "0x" << std::setw(4) << guid->Data2 << ", " <<
                "0x" << std::setw(4) << guid->Data3 << ", { ";
            for (int i = 0; i < 7; ++i)
            {
                out << "0x" << std::setw(2) << (unsigned) guid->Data4[i] << ", ";
            }
            out << "0x" << std::setw(2) << (unsigned) guid->Data4[7] << " }\n";
            out << std::setbase(10) << std::setfill(' ');
            break;
        }
    }
    out << "};\n";

    // Class
    if (interface->getComment())
    {
        out << Indent() << interface->getComment();
    }
    out << Indent() << "class " << interface->getName();
    if (interface->getExtends() != "")
    {
        out << " : public " << interface->getExtends();
    }
    out << Indent() << '{' << Indent() << "public:";

    InterfaceVisitor v(out);
    Identifier* id;
    IdentifierList::Iterator iter = interface->begin();
    while ((id = iter.next()))
    {
        // Interface may contain the following nodes:
        //  - const_declaration
        //  - type_declaration
        //  - op_declaration
        //  - tagged_declarator
        if (id->getComment())
        {
            out << Indent(interface) << id->getComment() << Indent(interface);
            id->accept(v);
            out << '\n';
        }
        else
        {
            out << Indent(interface);
            id->accept(v);
        }
    }

    // Class::interfaceID()
    out << Indent() << "static const Guid& interfaceID()" <<
           Indent() << "{" <<
           Indent() << "    return IID_" << interface->getName() << ';' <<
           Indent() << "}";

    out << Indent(interface->parent()) << "};\n";
}

//
// InterfaceVisitor
//

void InterfaceVisitor::at(const Function* f)
{
    out << "virtual ";
    DeclaratorHandler(out, f, true);
    out << " = 0;";
}

void GenerateHeader(std::ostream& out)
{
    //
    // Process global Namespace
    //

    // #include
    ImportHandler(out, &NameSpace::global());

    NameSpaceVisitor v(out);
    Identifier* id;
    IdentifierList::Iterator iter = NameSpace::global().begin();
    while ((id = iter.next()))
    {
        if (id->isImported())
        {
            continue;
        }

        out << Indent();

        // Namespace may contain the following nodes:
        //  - interface
        //  - const_declaration
        //  - type_declaration
        //  - tagged_declarator
        id->accept(v);
    }
    out << '\n';
}
