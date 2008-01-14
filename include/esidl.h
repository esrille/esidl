/*
 * Copyright (c) 2007
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

#ifndef NINTENDO_ESIDL_H_INCLUDED
#define NINTENDO_ESIDL_H_INCLUDED

#include <assert.h>
#include <stdio.h>
#include <list>
#include <string>
#include <es/uuid.h>
#include "guid.h"

class Node;
    class Include;
    class ScopedName;
    class Module;
    class EnumType;
    class StructType;
    class ExceptDcl;
    class Interface;
    class Type;
    class SequenceType;
    class BinaryExpr;
    class UnaryExpr;
    class GroupingExpression;
    class Literal;
    class PragmaID;
    class Member;
        class ArrayDcl;
        class Attribute;
        class ConstDcl;
        class OpDcl;
        class ParamDcl;
class Visitor;

typedef std::list<Node*> NodeList;

Node* getSpecification();
Node* setSpecification(Node* node);
Node* getCurrent();
Node* setCurrent(const Node* node);

void setFilename(const char* name);
char* getFilename();
std::string& getJavadoc();
void setJavadoc(const char* doc);

class Node
{
protected:
    Node*               parent;
    NodeList*           children;
    std::string         name;
    mutable std::string separator;
    std::string         id;
    mutable size_t      offset;
    int                 rank;
    std::string         javadoc;

    static int          level;      // current include level

public:
    Node() :
        parent(0),
        children(0),
        separator(", "),
        offset(0),
        rank(level)
    {
    }

    Node(std::string name) :
        parent(0),
        name(name),
        children(0),
        separator(", "),
        offset(0),
        rank(level)
    {
    }

    Node(NodeList* children) :
        parent(0),
        separator(", "),
        offset(0),
        rank(level)
    {
        setChildren(children);
    }

    Node(std::string name, NodeList* children) :
        parent(0),
        name(name),
        separator(", "),
        offset(0),
        rank(level)
    {
        setChildren(children);
    }

    ~Node()
    {
        if (parent)
        {
            parent->remove(this);
        }
        if (children)
        {
            while (0 < children->size())
            {
                Node* child = children->front();
                delete child;
            }
        }
    }

    Node* getParent() const
    {
        return parent;
    }

    void setParent(Node* node)
    {
        parent = node;
    }

    bool isLeaf() const
    {
        return (children == 0) ? true : false;
    }

    int getSize() const
    {
        if (isLeaf())
        {
            return 0;
        }

        return children->size();
    }

    virtual void add(Node* node)
    {
        if (children == 0)
        {
            children = new NodeList;
        }
        children->push_back(node);
        node->setParent(this);
    }

    void remove(Node* node)
    {
        assert(children);
        node->setParent(0);
        children->remove(node);
    }

    void setChildren(NodeList* list)
    {
        assert(children == 0);
        children = list;
        for (NodeList::iterator i = begin(); i != end(); ++i)
        {
            (*i)->setParent(this);
        }
    }

    NodeList::iterator begin() const
    {
        assert(children);
        return children->begin();
    }

    NodeList::iterator end() const
    {
        assert(children);
        return children->end();
    }

    std::string& getName()
    {
        return name;
    }

    const std::string& getName() const
    {
        return name;
    }

    void setSeparator(const char* s) const
    {
        separator = s;
    }

    const std::string& getSeparator() const
    {
        return separator;
    }

    virtual Node* search(const std::string& elem, size_t pos = 0) const
    {
        if (elem.size() <= pos)
        {
            return const_cast<Node*>(this);
        }

        if (!children)
        {
            return 0;
        }

        size_t n = elem.find("::", pos);
        assert(n != 0);
        if (n == std::string::npos)
        {
            n = elem.size();
        }
        for (NodeList::reverse_iterator i = children->rbegin();
             i != children->rend();
             ++i)
        {
            if ((*i)->getName().compare(0, (*i)->getName().size(), elem, pos, n - pos) == 0)
            {
                return (*i)->search(elem, n + 2);
            }
        }
        return 0;
    }

    virtual bool compare(const char* str, const Node* scope) const
    {
        return name.compare(str);
    }

    virtual bool isAny(const Node* scope) const
    {
        return compare("any", scope) == 0;
    }

    virtual bool isOctet(const Node* scope) const
    {
        return compare("octet", scope) == 0;
    }

    virtual bool isBoolean(const Node* scope) const
    {
        return compare("boolean", scope) == 0;
    }

    virtual bool isString(const Node* scope) const
    {
        return compare("string", scope) == 0;
    }

    virtual bool isWString(const Node* scope) const
    {
        return compare("wstring", scope) == 0;
    }

    virtual bool isGuid(const Node* scope) const
    {
        return compare("uuid", scope) == 0;
    }

    virtual const Interface* isInterface(const Node* scope) const
    {
        return 0;
    }

    virtual const SequenceType* isSequence(const Node* scope) const
    {
        return 0;
    }

    virtual const StructType* isStruct(const Node* scope) const
    {
        return 0;
    }

    virtual const ArrayDcl* isArray(const Node* scope) const
    {
        return 0;
    }

    std::string& getID()
    {
        return id;
    }

    const std::string& getID() const
    {
        return id;
    }

    size_t getOffset() const
    {
        return offset;
    }

    void setOffset(size_t offset) const
    {
        assert(offset == 0 || this->offset == 0);
        this->offset = offset;
    }

    virtual void accept(Visitor* visitor);

    const std::string& getJavadoc() const
    {
        return javadoc;
    }

    void setJavadoc(const char* doc)
    {
        javadoc = doc;
    }

    int getRank() const
    {
        return rank;
    }

    static int incLevel()
    {
        ++level;
    }

    static int decLevel()
    {
        --level;
    }
};

class Include : public Node
{
public:
    Include(std::string identifier) :
        Node(identifier)
    {
        separator = "\n";

        if (name[0] == '"')
        {
            name = name.substr(1, name.size() - 2);
        }
    }

    virtual void accept(Visitor* visitor);
};

class ScopedName : public Node
{
public:
    ScopedName(std::string identifier) :
        Node(identifier)
    {
    }

    Node* search(const Node* scope) const
    {
        if (name.compare(0, 2, "::") == 0)
        {
            return getSpecification()->search(name, 2);
        }

        for (const Node* node = scope; node; node = node->getParent())
        {
            if (Node* find = node->search(name))
            {
                return find;
            }
        }
        return 0;
    }

    virtual bool compare(const char* str, const Node* scope) const
    {
        const Node* node = search(scope);
        return node->compare(str, node->getParent());
    }

    virtual const Interface* isInterface(const Node* scope) const
    {
        const Node* node = search(scope);
        return node->isInterface(node->getParent());
    }

    virtual const SequenceType* isSequence(const Node* scope) const
    {
        const Node* node = search(scope);
        return node->isSequence(node->getParent());
    }

    virtual const StructType* isStruct(const Node* scope) const
    {
        const Node* node = search(scope);
        return node->isStruct(node->getParent());
    }

    virtual const ArrayDcl* isArray(const Node* scope) const
    {
        const Node* node = search(scope);
        return node->isArray(node->getParent());
    }

    virtual void accept(Visitor* visitor);
};

class Module : public Node
{
    int interfaceCount; // of rank 1
    int constCount;     // of rank 1
    int moduleCount;    // of rank 1

public:
    Module(std::string identifier) :
        Node(identifier),
        interfaceCount(0),
        constCount(0),
        moduleCount(0)
    {
        separator = "\n";
        rank = 1;   // 'cause namespace is open.
        javadoc = ::getJavadoc();
    }

    virtual void add(Node* node);

    int getInterfaceCount() const
    {
        return interfaceCount;
    }

    int getConstCount() const
    {
        return constCount;
    }

    int getModuleCount() const
    {
        return moduleCount;
    }

    virtual void accept(Visitor* visitor);
};

class EnumType : public Node
{
public:
    EnumType(std::string identifier) :
        Node(identifier)
    {
        separator = ";\n";
    }

    int getMemberCount() const
    {
        assert(children);
        return children->size();
    }

    virtual void accept(Visitor* visitor);
};

class StructType : public Node
{
    int memberCount;

public:
    StructType(std::string identifier) :
        Node(identifier)
    {
        separator = ";\n";
        javadoc = ::getJavadoc();
    }

    virtual void add(Node* node);

    int getMemberCount() const
    {
        return memberCount;
    }

    virtual const StructType* isStruct(const Node* scope) const
    {
        return this;
    }

    virtual void accept(Visitor* visitor);
};

class ExceptDcl : public StructType
{
public:
    ExceptDcl(std::string identifier) :
        StructType(identifier)
    {
        javadoc = ::getJavadoc();
    }

    virtual void accept(Visitor* visitor);
};

class Interface : public Node
{
    Node* extends;
    int constCount;
    int methodCount;

public:
    Interface(std::string identifier, Node* extends = 0, bool forward = false) :
        Node(identifier),
        extends(extends),
        constCount(0),
        methodCount(0)
    {
        separator = ";\n";
        if (!forward)
        {
            children = new NodeList;
        }
        javadoc = ::getJavadoc();
    }

    ~Interface()
    {
        if (extends)
        {
            delete extends;
        }
    }

    virtual void add(Node* node);

    virtual Node* search(const std::string& elem, size_t pos = 0) const
    {
        if (Node* node = Node::search(elem, pos))
        {
            return node;
        }

        if (!extends)
        {
            return 0;
        }

        for (NodeList::iterator i = extends->begin(); i != extends->end(); ++i)
        {
            Node* base = static_cast<ScopedName*>(*i)->search(parent);
            if (Node* node = base->search(elem, pos))
            {
                return node;
            }
        }

        return 0;
    }

    Node* getExtends() const
    {
        return extends;
    }

    virtual const Interface* isInterface(const Node* scope) const
    {
        return this;
    }

    int getConstCount() const
    {
        return constCount;
    }

    int getMethodCount() const
    {
        return methodCount;
    }

    bool getIID(Guid& iid) const
    {
        if (getID().compare(0, 5, "\"DCE:") == 0 &&
            parseGuid(getID().c_str() + 5, &iid))
        {
            return true;
        }
        else
        {
            memset(&iid, 0, sizeof(Guid));
            return false;
        }
    }

    virtual void accept(Visitor* visitor);
};

class Type : public Node
{
public:
    Type(std::string identifier) :
        Node(identifier)
    {
    }

    virtual void accept(Visitor* visitor);
};

class SequenceType : public Node
{
    Node* spec;
    Node* max;

public:
    SequenceType(Node* spec, Node* max = 0) :
        spec(spec),
        max(max)
    {
    }

    ~SequenceType()
    {
        delete spec;
        if (max)
        {
            delete max;
        }
    }

    Node* getSpec() const
    {
        return spec;
    }

    Node* getMax() const
    {
        return max;
    }

    virtual const SequenceType* isSequence(const Node* scope) const
    {
        return this;
    }

    virtual void accept(Visitor* visitor);
};

class BinaryExpr : public Node
{
public:
    BinaryExpr(const char* op, Node* left, Node* right) :
        Node(op)
    {
        add(left);
        add(right);
    }

    Node* getLeft() const
    {
        return children->front();
    }

    Node* getRight() const
    {
        return children->back();
    }

    virtual void accept(Visitor* visitor);
};

class UnaryExpr : public Node
{
public:
    UnaryExpr(const char* op, Node* exp) :
        Node(op)
    {
        add(exp);
    }

    virtual void accept(Visitor* visitor);
};

class GroupingExpression : public Node
{
public:
    GroupingExpression(Node* exp)
    {
        add(exp);
    }

    virtual void accept(Visitor* visitor);
};

class Literal : public Node
{
public:
    Literal(const char* literal) :
        Node(literal)
    {
    }

    operator bool() const;
    operator char() const;
    operator wchar_t() const;
    operator short() const;
    operator int() const;
    operator long long() const;
    operator unsigned char() const;
    operator unsigned short() const;
    operator unsigned int() const;
    operator unsigned long long() const;
    operator float() const;
    operator double() const;
    operator long double() const;
    operator std::string() const;

    virtual void accept(Visitor* visitor);
};

class PragmaID : public Node
{
    Node*       scopedName;
    std::string id;

public:
    PragmaID(Node* scopedName, std::string id) :
        scopedName(scopedName),
        id(id)
    {
        separator = "\n";

        Node* node = static_cast<ScopedName*>(scopedName)->search(getCurrent());
        assert(node);
        node->getID() = id;
    }

    ~PragmaID()
    {
        delete scopedName;
    }

    Node* getScopedName() const
    {
        return scopedName;
    }

    const std::string& getID() const
    {
        return id;
    }

    virtual void accept(Visitor* visitor);
};

class Member : public Node
{
    Node*           spec;
    bool            type;

public:
    Member(std::string identifier, Node* spec = 0) :
        Node(identifier),
        spec(spec),
        type(false)
    {
        separator = ";\n";
        javadoc = ::getJavadoc();
    }

    ~Member()
    {
        delete spec;
    }

    Node* getSpec() const
    {
        return spec;
    }

    void setSpec(Node* node)
    {
        spec = node;
    }

    bool isTypedef() const
    {
        return type;
    }

    void setTypedef(bool type)
    {
        this->type = type;
    }

    virtual bool compare(const char* str, const Node* scope) const
    {
        if (!type)
        {
            return 0;
        }
        return spec->compare(str, scope);
    }

    virtual const Interface* isInterface(const Node* scope) const
    {
        if (!type)
        {
            return 0;
        }
        return spec->isInterface(scope);
    }

    virtual const SequenceType* isSequence(const Node* scope) const
    {
        if (!type)
        {
            return 0;
        }
        return spec->isSequence(scope);
    }

    virtual const StructType* isStruct(const Node* scope) const
    {
        if (!type)
        {
            return 0;
        }
        return spec->isStruct(scope);
    }

    virtual const ArrayDcl* isArray(const Node* scope) const
    {
        if (!type)
        {
            return 0;
        }
        return spec->isArray(scope);
    }

    virtual void accept(Visitor* visitor);
};

class ArrayDcl : public Member
{
public:
    ArrayDcl() :
        Member("")
    {
    }

    virtual const ArrayDcl* isArray(const Node* scope) const
    {
        return this;
    }

    int getDimension() const
    {
        assert(children);
        return children->size();
    }

    virtual void accept(Visitor* visitor);
};

class Attribute : public Member
{
    bool readonly;

public:
    Attribute(std::string identifier, Node* spec, bool readonly = false) :
        Member(identifier, spec),
        readonly(readonly)
    {
        separator = ";\n";
    }

    bool isReadonly() const
    {
        return readonly;
    }

    virtual void accept(Visitor* visitor);
};

class ConstDcl : public Member
{
    Node* exp;
    mutable size_t value;

public:
    ConstDcl(Node* spec, std::string identifier, Node* exp) :
        Member(identifier, spec),
        exp(exp)
    {
        separator = ";\n";
    }

    ~ConstDcl()
    {
        delete exp;
    }

    Node* getExp() const
    {
        return exp;
    }

    size_t getValue() const
    {
        return value;
    }

    void setValue(size_t value) const
    {
        assert(value == 0 || this->value == 0);
        this->value = value;
    }

    Type* getType() const
    {
        Node* type = getSpec();
        Node* scope = getParent();
        while (ScopedName* scopedName = dynamic_cast<ScopedName*>(type))
        {
            Member* member = dynamic_cast<Member*>(scopedName->search(scope));
            assert(member);
            type = member->getSpec();
            scope = member->getParent();
        }
        assert(dynamic_cast<Type*>(type));
        return static_cast<Type*>(type);
    }

    virtual void accept(Visitor* visitor);
};

class OpDcl : public Member
{
    Node* raises;
    int paramCount;

public:
    OpDcl(std::string identifier, Node* spec) :
        Member(identifier, spec),
        raises(0),
        paramCount(0)
    {
        separator = ";\n";
        children = new NodeList;
    }

    virtual void add(Node* node);

    Node* getRaises() const
    {
        return raises;
    }

    void setRaises(Node* raises)
    {
        this->raises = raises;
    }

    int getParamCount() const
    {
        return paramCount;
    }

    int getRaiseCount() const
    {
        if (raises == 0)
        {
            return 0;
        }

        return raises->getSize();
    }

    virtual void accept(Visitor* visitor);
};

class ParamDcl : public Member
{
    int attr;

public:
    static const int In = 0;
    static const int Out = 1;
    static const int InOut = 2;

    ParamDcl(std::string identifier, Node* spec, int attr) :
        Member(identifier, spec),
        attr(attr)
    {
        separator = ", ";
    }

    int getAttr() const
    {
        return attr;
    }

    virtual void accept(Visitor* visitor);
};

class Visitor
{
    Node* prev;

protected:
    void visitChildren(const Node* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            (*i)->accept(this);
        }
    }

public:
    Visitor()
    {
        prev = setCurrent(0);
    }

    ~Visitor()
    {
        setCurrent(prev);
    }

    virtual void at(const Node* node)
    {
    }

    virtual void at(const Include* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const ScopedName* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const Module* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const EnumType* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const StructType* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const ExceptDcl* node)
    {
        at(static_cast<const StructType*>(node));
    }

    virtual void at(const Interface* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const Type* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const SequenceType* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const BinaryExpr* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const UnaryExpr* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const GroupingExpression* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const Literal* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const PragmaID* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const Member* node)
    {
        at(static_cast<const Node*>(node));
    }

    virtual void at(const ArrayDcl* node)
    {
        at(static_cast<const Member*>(node));
    }

    virtual void at(const Attribute* node)
    {
        at(static_cast<const Member*>(node));
    }

    virtual void at(const ConstDcl* node)
    {
        at(static_cast<const Member*>(node));
    }

    virtual void at(const OpDcl* node)
    {
        at(static_cast<const Member*>(node));
    }

    virtual void at(const ParamDcl* node)
    {
        at(static_cast<const Member*>(node));
    }
};

inline void Node::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void Include::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void ScopedName::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void Module::accept(Visitor* visitor)
{
    Node* prev = setCurrent(this);
    visitor->at(this);
    setCurrent(prev);
}

inline void EnumType::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void StructType::accept(Visitor* visitor)
{
    Node* prev = setCurrent(this);
    visitor->at(this);
    setCurrent(prev);
}

inline void ExceptDcl::accept(Visitor* visitor)
{
    Node* prev = setCurrent(this);
    visitor->at(this);
    setCurrent(prev);
}

inline void Interface::accept(Visitor* visitor)
{
    Node* prev = setCurrent(this);
    visitor->at(this);
    setCurrent(prev);
}

inline void Type::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void SequenceType::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void BinaryExpr::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void UnaryExpr::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void GroupingExpression::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void Literal::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void PragmaID::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void Member::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void ArrayDcl::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void Attribute::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void ConstDcl::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void OpDcl::accept(Visitor* visitor)
{
    visitor->at(this);
}

inline void ParamDcl::accept(Visitor* visitor)
{
    visitor->at(this);
}

extern void print();
extern void printCxx(const std::string& filename);
extern void printEnt(const std::string& filename);

extern std::string getOutputFilename(const char* input, const char* suffix);

#endif  // NINTENDO_ESIDL_H_INCLUDED
