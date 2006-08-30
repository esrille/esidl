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

#include "esidl.h"

class TypeNameVisitor : public Visitor
{
protected:
    std::ostream& out;
public:
    TypeNameVisitor(std::ostream& out) : out(out) { }
    void at(const Identifier* element);
    void at(const Integer* element);
    void at(const Float* element);
    void at(const Character* element);
    void at(const Boolean* element);
    void at(const Void* element);
    void at(const UuidType* element);
};

class TypeSpecVisitor : public TypeNameVisitor
{
public:
    TypeSpecVisitor(std::ostream& out) : TypeNameVisitor(out) { }
    void at(const Structure* element);
    void at(const Enum* element);
};

class NameSpaceVisitor : public TypeSpecVisitor
{
    // Namespace may contain the following nodes:
    //  - interface
    //  - const_declaration
    //  - type_declaration
    //  - tagged_declarator
public:
    NameSpaceVisitor(std::ostream& out) : TypeSpecVisitor(out) { }
    void at(const Declarator* element);
    void at(const Constant* element);
    void at(const Enum* element);
    void at(const Structure* element);
    void at(const Interface* element);
};

class InterfaceVisitor : public NameSpaceVisitor
{
    // Interface may contain the following nodes:
    //  - const_declaration
    //  - type_declaration
    //  - op_declaration
    //  - tagged_declarator
public:
    InterfaceVisitor(std::ostream& out) : NameSpaceVisitor(out) { }
    void at(const Interface* i) { /* error */ }
    void at(const Function* f);
};

class EnumVisitor : public Visitor
{
    // Structure may contain the following nodes:
    //  - enum_identifier
    std::ostream& out;
    int n;
public:
    EnumVisitor(std::ostream& out) : out(out), n(0) { }
    void at(const Constant* element);
};

class StructureVisitor : public Visitor
{
    // Structure may contain the following nodes:
    //  - field_declarator
    std::ostream& out;
public:
    StructureVisitor(std::ostream& out) : out(out) { }
    void at(const Declarator* element);
};

class ExpressionVisitor : public Visitor
{
    std::ostream& out;
    const Constant* c;
    const Expression* exp;
public:
    ExpressionVisitor(std::ostream& out, const Constant* c) :
        out(out), c(c), exp(c->getExpression())
    {

    }
    void at(const Integer* i)
    {
        if (i->isSigned())
        {
            out << static_cast<int>(*exp);
        }
        else
        {
            out << static_cast<unsigned int>(static_cast<int>(*exp)) << 'u';
        }
    }
    void at(const Float* f)
    {
        if (f->getBits() == 32)
        {
            out << static_cast<double>(*exp) << 'f';
        }
        else
        {
            out << static_cast<double>(*exp);
        }
    }
    void at(const Character* ch)
    {
        if (c->getPointer() == 0)
        {
            out << '\'' << static_cast<char>(*exp) << '\'';
        }
        else
        {
            out << '"' << static_cast<const char*>(*exp) << '"';
        }
    }
    void at(const Boolean* b)
    {
        if (static_cast<bool>(*exp))
        {
            out << "true";
        }
        else
        {
            out << "false";
        }
    }
};
