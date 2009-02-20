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

#ifndef NINTENDO_ESIDL_H_INCLUDED
#define NINTENDO_ESIDL_H_INCLUDED

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <list>
#include <string>
#include <vector>
#include <es/uuid.h>
#include "guid.h"

class Node;
    class Include;
    class ScopedName;
    class Module;
    class StructType;
    class ExceptDcl;
    class Interface;
    class Type;
    class NativeType;
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
    class ExtendedAttribute;
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
std::string& popJavadoc();
void pushJavadoc();

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
    NodeList*           extendedAttributes;

    static int          level;          // current include level
    static const char*  baseObjectName; // default base object name

public:
    // Attribute bits
    static const u32 AttrMask =                 0x00000003;
    static const u32 AttrOperation =            0x00000000;
    static const u32 AttrGetter =               0x00000001;
    static const u32 AttrSetter =               0x00000002;
    static const u32 AttrIn =                   0x00000000;
    static const u32 AttrOut =                  0x00000001;
    static const u32 AttrInOut =                0x00000002;
    // Extended attribute bits
    // [IndexCreator], [IndexDeleter], [IndexGetter] and [IndexSetter]
    static const u32 IndexMask =                0x0000003c;
    static const u32 IndexCreator =             0x00000004;
    static const u32 IndexDeleter =             0x00000008;
    static const u32 IndexGetter =              0x00000010;
    static const u32 IndexSetter =              0x00000020;
    // [NameCreator], [NameDeleter], [NameGetter] and [NameSetter]
    static const u32 NameMask =                 0x000003c0;
    static const u32 NameCreator =              0x00000040;
    static const u32 NameDeleter =              0x00000080;
    static const u32 NameGetter =               0x00000100;
    static const u32 NameSetter =               0x00000200;
    // [NoIndexingOperations]
    static const u32 NoIndexingOperations =     0x00000400;
    // [Callback]
    static const u32 CallbackMask =             0x00000006;
    static const u32 Callback =                 0x00001800;
    static const u32 CallbackIsFunctionOnly =   0x00000800;
    static const u32 CallbackIsPropertyOnly =   0x00001000;
    // [NoInterfaceObject]
    static const u32 NoInterfaceObject =        0x00002000;
    // [PrototypeRoot ]
    static const u32 PrototypeRoot =            0x00004000;
    // [Null]
    static const u32 NullIsEmpty =              0x00008000;
    static const u32 NullIsNull =               0x00010000;
    // [Undefined]
    static const u32 UndefinedIsEmpty =         0x00020000;
    static const u32 UndefinedIsNull =          0x00040000;
    // [Stringifies]
    static const u32 Stringifies =              0x00080000;
    // [Replaceable]
    static const u32 Replaceable =              0x00100000;
    // [Callable]
    static const u32 Callable =                 0x00200000;
    // [Optional]
    static const u32 Optional =                 0x00400000;
    // [Variadic]
    static const u32 Variadic =                 0x00800000;

    Node() :
        parent(0),
        children(0),
        separator(", "),
        offset(0),
        extendedAttributes(0),
        rank(level)
    {
    }

    Node(std::string name) :
        parent(0),
        name(name),
        children(0),
        separator(", "),
        offset(0),
        extendedAttributes(0),
        rank(level)
    {
    }

    Node(NodeList* children) :
        parent(0),
        separator(", "),
        offset(0),
        extendedAttributes(0),
        rank(level)
    {
        setChildren(children);
    }

    Node(std::string name, NodeList* children) :
        parent(0),
        name(name),
        separator(", "),
        offset(0),
        extendedAttributes(0),
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
                children->pop_front();
                delete child;
            }
        }
        if (extendedAttributes)
        {
            while (0 < extendedAttributes->size())
            {
                Node* attr = extendedAttributes->front();
                extendedAttributes->pop_front();
                delete attr;
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

    NodeList::reverse_iterator rbegin() const
    {
        assert(children);
        return children->rbegin();
    }

    NodeList::reverse_iterator rend() const
    {
        assert(children);
        return children->rend();
    }

    std::string& getName()
    {
        return name;
    }

    const std::string& getName() const
    {
        return name;
    }

    std::string getQualifiedName() const
    {
        std::string name;
        for (const Node* node = this; node && node->name != ""; node = node->getParent())
        {
            name = "::" + node->name + name;
        }
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

    virtual bool isVoid(const Node* scope) const
    {
        return compare("void", scope) == 0;
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

    virtual bool isShort(const Node* scope) const
    {
        return compare("short", scope) == 0;
    }

    virtual bool isLong(const Node* scope) const
    {
        return compare("long", scope) == 0;
    }

    virtual bool isLongLong(const Node* scope) const
    {
        return compare("long long", scope) == 0;
    }

    virtual bool isUnsignedShort(const Node* scope) const
    {
        return compare("unsigned short", scope) == 0;
    }

    virtual bool isUnsignedLong(const Node* scope) const
    {
        return compare("unsigned long", scope) == 0;
    }

    virtual bool isUnsignedLongLong(const Node* scope) const
    {
        return compare("unsigned long long", scope) == 0;
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

    virtual bool isInterface(const Node* scope) const
    {
        return compare("Object", scope) == 0;
    }

    virtual bool isFloat(const Node* scope) const
    {
        return compare("float", scope) == 0;
    }

    virtual bool isDouble(const Node* scope) const
    {
        return compare("double", scope) == 0;
    }

    virtual SequenceType* isSequence(const Node* scope)
    {
        return 0;
    }

    virtual StructType* isStruct(const Node* scope)
    {
        return 0;
    }

    virtual ArrayDcl* isArray(const Node* scope)
    {
        return 0;
    }

    virtual NativeType* isNative(const Node* scope)
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

    virtual void setExtendedAttributes(NodeList* list)
    {
        assert(extendedAttributes == 0);
        extendedAttributes = list;
    }

    bool hasExtendedAttributes()
    {
        return (extendedAttributes == 0) ? false : true;
    }

    const bool hasExtendedAttributes() const
    {
        return (extendedAttributes == 0) ? false : true;
    }

    NodeList* getExtendedAttributes()
    {
        return extendedAttributes;
    }

    const NodeList* getExtendedAttributes() const
    {
        return extendedAttributes;
    }

    static int incLevel()
    {
        ++level;
    }

    static int decLevel()
    {
        --level;
    }

    static void setBaseObjectName(const char* name)
    {
        baseObjectName = name;
    }

    static const char* getBaseObjectName()
    {
        return baseObjectName;
    }

    bool isBaseObject() const
    {
        if (name == "Object")
        {
            return true;
        }
        if (baseObjectName && name == baseObjectName)
        {
            return true;
        }
        return false;
    }
};

class Include : public Node
{
    bool system;

public:
    Include(std::string identifier, bool system = false) :
        Node(identifier),
        system(system)
    {
        separator = "\n";

        if (name[0] == '"')
        {
            name = name.substr(1, name.size() - 2);
        }
    }

    bool isSystem() const
    {
        return system;
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

    Node* search(const Node* scope) const;

    virtual bool compare(const char* str, const Node* scope) const
    {
        Node* node = search(scope);
        if (!node)
        {
            return false;
        }
        return node->compare(str, node->getParent());
    }

    virtual bool isInterface(const Node* scope) const
    {
        Node* node = search(scope);
        if (!node)
        {
            return false;
        }
        return node->isInterface(node->getParent());
    }

    virtual SequenceType* isSequence(const Node* scope)
    {
        Node* node = search(scope);
        if (!node)
        {
            return 0;
        }
        return node->isSequence(node->getParent());
    }

    virtual StructType* isStruct(const Node* scope)
    {
        Node* node = search(scope);
        if (!node)
        {
            return 0;
        }
        return node->isStruct(node->getParent());
    }

    virtual NativeType* isNative(const Node* scope)
    {
        Node* node = search(scope);
        if (!node)
        {
            return 0;
        }
        return node->isNative(node->getParent());
    }

    virtual ArrayDcl* isArray(const Node* scope)
    {
        Node* node = search(scope);
        if (!node)
        {
            return 0;
        }
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
    virtual void setExtendedAttributes(NodeList* list);

    int getInterfaceCount() const
    {
        return interfaceCount;
    }

    void incInterfaceCount()
    {
        ++interfaceCount;
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

class StructType : public Node
{
    int memberCount;

public:
    StructType(std::string identifier) :
        Node(identifier),
        memberCount(0)
    {
        separator = ";\n";
        javadoc = ::getJavadoc();
    }

    virtual void add(Node* node);

    int getMemberCount() const
    {
        return memberCount;
    }

    virtual StructType* isStruct(const Node* scope)
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
    Interface* constructor;
    std::list<const Interface*> implementedOn;
    u32 attr;
    std::string callable;
    std::string stringifies;

public:
    Interface(std::string identifier, Node* extends = 0, bool forward = false) :
        Node(identifier),
        extends(extends),
        constCount(0),
        methodCount(0),
        constructor(0),
        attr(0)
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
    virtual void setExtendedAttributes(NodeList* list);

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

    virtual bool isInterface(const Node* scope) const
    {
        return true;
    }

    int getConstCount() const
    {
        return constCount;
    }

    int getMethodCount() const
    {
        return methodCount;
    }

    int addMethodCount(int count)
    {
        methodCount += count;
        return methodCount;
    }

    Interface* getSuper() const
    {
        Interface* super = 0;
        if (Node* extends = getExtends())
        {
            for (NodeList::iterator i = extends->begin(); i != extends->end(); ++i)
            {
                ScopedName* scoped = static_cast<ScopedName*>(*i);
                Node* base = scoped->search(this);
                super = dynamic_cast<Interface*>(base);
                assert(super);
                break;  // Multiple inheritance is not allowed.
            }
        }
        return super;
    }

    Interface* getConstructor() const
    {
        return constructor;
    }

    const std::list<const Interface*>* getImplementedOn() const
    {
        return &implementedOn;
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

    u32 getAttr() const
    {
        return attr;
    }

    u32 isCallback() const
    {
        return (attr & CallbackMask);
    }

    std::string getCallable() const
    {
        return callable;
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

class NativeType : public Type
{
public:
    NativeType(std::string identifier) :
        Type(identifier)
    {
    }

    virtual NativeType* isNative(const Node* scope)
    {
        return this;
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

    void setSpec(Node* spec)
    {
        this->spec = spec;
    }

    Node* getMax() const
    {
        return max;
    }

    virtual SequenceType* isSequence(const Node* scope)
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

    virtual bool isInterface(const Node* scope) const
    {
        if (!type)
        {
            return 0;
        }
        return spec->isInterface(scope);
    }

    virtual SequenceType* isSequence(const Node* scope)
    {
        if (!type)
        {
            return 0;
        }
        return spec->isSequence(scope);
    }

    virtual StructType* isStruct(const Node* scope)
    {
        if (!type)
        {
            return 0;
        }
        return spec->isStruct(scope);
    }

    virtual NativeType* isNative(const Node* scope)
    {
        if (!type)
        {
            return 0;
        }
        return spec->isNative(scope);
    }

    virtual ArrayDcl* isArray(const Node* scope)
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

    virtual ArrayDcl* isArray(const Node* scope)
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
    u32 attr;
    std::string putForwards;

public:
    Attribute(std::string identifier, Node* spec, bool readonly = false) :
        Member(identifier, spec),
        readonly(readonly),
        attr(0)
    {
        separator = ";\n";
    }

    bool isReadonly() const
    {
        return readonly;
    }

    void setStringifies()
    {
        attr |= Stringifies;
    }

    virtual void setExtendedAttributes(NodeList* list);

    u32 getAttr() const
    {
        return attr;
    }

    bool isReplaceable() const
    {
        return attr & Replaceable;
    }

    bool isPutForwards() const
    {
        return putForwards != "";
    }

    const std::string& getPutForwards() const
    {
        return putForwards;
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
    u32 attr;
    int methodCount;
    std::vector<int> paramCounts;  // for each method

public:
    OpDcl(std::string identifier, Node* spec) :
        Member(identifier, spec),
        raises(0),
        paramCount(0),
        attr(0),
        methodCount(1)
    {
        separator = ";\n";
        children = new NodeList;
    }

    virtual void add(Node* node);
    virtual void setExtendedAttributes(NodeList* list);

    Node* getRaises() const
    {
        return raises;
    }

    void setRaises(Node* raises);

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

    u32 getAttr() const
    {
        return attr;
    }

    void setCallable()
    {
        attr |= Callable;
    }

    int getMethodCount() const
    {
        return methodCount;
    }

    int getParamCount(int i) const
    {
        assert(0 <= i && i < methodCount);
        return (methodCount == 1) ? paramCount : paramCounts[i];
    }

    virtual void accept(Visitor* visitor);
};

class ParamDcl : public Member
{
    u32 attr;

public:
    ParamDcl(std::string identifier, Node* spec, u32 attr) :
        Member(identifier, spec),
        attr(attr)
    {
        separator = ", ";
    }

    u32 getAttr() const
    {
        return attr;
    }

    bool isInput() const
    {
        return (attr & AttrMask) == AttrIn;
    }

    bool isOptional() const
    {
        return attr & Optional;
    }

    bool isVariadic() const
    {
        return attr & Variadic;
    }

    virtual void setExtendedAttributes(NodeList* list);

    virtual void accept(Visitor* visitor);
};

class ExtendedAttribute : public Node
{
    Node* details;

public:
    ExtendedAttribute(std::string name, Node* details = 0) :
        Node(name),
        details(details)
    {
    }

    Node* getDetails() const
    {
        return details;
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

    virtual void at(const NativeType* node)
    {
        at(static_cast<const Type*>(node));
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

    virtual void at(const ExtendedAttribute* node)
    {
        at(static_cast<const Node*>(node));
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

inline void NativeType::accept(Visitor* visitor)
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

inline void ExtendedAttribute::accept(Visitor* visitor)
{
    visitor->at(this);
}

extern void print();
extern void printCxx(const std::string& filename);
extern void printEnt(const std::string& filename);
extern void printNpapi(const char* idlFilename, bool isystem);

extern std::string getOutputFilename(const char* input, const char* suffix);
extern std::string getIncludedName(const std::string& header);

extern Node* resolve(const Node* scope, std::string name);

extern std::string getScopedName(std::string moduleName, std::string absoluteName);

#endif  // NINTENDO_ESIDL_H_INCLUDED
