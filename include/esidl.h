/*
 * Copyright 2008-2010 Google Inc.
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

#ifndef ESIDL_H_INCLUDED
#define ESIDL_H_INCLUDED

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <string>
#include <vector>

const std::string getBaseFilename();
void setBaseFilename(const char* name);
const std::string getFilename();
void setFilename(const char* name);

class Node;
    class Include;
    class ScopedName;
    class Module;
    class StructType;
    class ExceptDcl;
    class Implements;
    class Interface;
    class Type;
    class NativeType;
    class SequenceType;
    class ArrayType;
    class BinaryExpr;
    class UnaryExpr;
    class GroupingExpression;
    class Literal;
    class Member;
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

Node* resolve(const Node* scope, std::string name);
Node* resolveInBase(const Interface* interface, std::string name);

std::string& getJavadoc();
void setJavadoc(const char* doc);
std::string& popJavadoc();
void pushJavadoc();

class Node
{
protected:
    uint32_t            attr;
    Node*               parent;
    NodeList*           children;
    std::string         name;
    mutable size_t      offset;
    int                 rank;
    std::string         javadoc;
    NodeList*           extendedAttributes;

    const std::string   source;         // source IDL filename
    int                 firstLine;
    int                 firstColumn;
    int                 lastLine;
    int                 lastColumn;

    mutable std::string meta;           // TODO: meta should not be mutable. fix later.

    static int          level;          // current include level
    static const char*  baseObjectName; // default base object name
    static const char*  namespaceName;  // flat namespace name if non zero

public:
    // Attribute bits
    static const uint32_t AttrMask =                 0x00000003;
    static const uint32_t AttrOperation =            0x00000000;
    static const uint32_t AttrGetter =               0x00000001;
    static const uint32_t AttrSetter =               0x00000002;
    static const uint32_t AttrIn =                   0x00000000;
    // Special attribute bits
    static const uint32_t IndexMask =                0x0000003c;
    static const uint32_t IndexCreator =             0x00000004;
    static const uint32_t IndexDeleter =             0x00000008;
    static const uint32_t IndexGetter =              0x00000010;
    static const uint32_t IndexSetter =              0x00000020;
    static const uint32_t Caller =                   0x00000040;
    static const uint32_t Stringifier =              0x00000080;
    static const uint32_t Omittable =                0x00000100;
    static const uint32_t Variadic =                 0x00000200;
    static const uint32_t Nullable =                 0x00000400;
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
    // [AllowAny]
    static const uint32_t AllowAny =                 0x00080000;  // TODO: Update meta
    // [Replaceable]
    static const uint32_t Replaceable =              0x00100000;
    // [Optional]
    static const uint32_t Optional =                 0x00400000;
    // [ImplementedOn]
    static const uint32_t ImplementedOn =            0x01000000;
    // [Supplemental]
    static const uint32_t Supplemental =             0x02000000;
    // [Constructor]
    static const uint32_t Constructor =              0x04000000;
    // [OverrideBuiltins]
    static const uint32_t OverrideBuiltins=          0x08000000;  // TODO: Update meta

    // misc. bits
    static const uint32_t HasIndexedProperties =     0x10000000;
    static const uint32_t HasNamedProperties =       0x20000000;
    static const uint32_t UnnamedProperty =          0x40000000;  // This should be banned.

    void setLocation(struct YYLTYPE* yylloc);
    void setLocation(struct YYLTYPE* first, struct YYLTYPE* last);

    void vreport(const char* message, va_list ap) const
    {
        fprintf(stderr, "%s:%d.%d-%d.%d: ",
                source.c_str(), firstLine, firstColumn, lastLine, lastColumn);
        vfprintf(stderr, message, ap);
        fprintf(stderr, "\n");
    }

    void report(const char* message, ...) const
    {
        va_list ap;
        va_start(ap, message);
        vreport(message, ap);
        va_end(ap);
    }

    bool check(bool cond, const char* message, ...) const
    {
        if (cond)
        {
            return true;
        }

        va_list ap;
        va_start(ap, message);
        vreport(message, ap);
        va_end(ap);

        exit(EXIT_FAILURE);
        return false;
    }

    Node() :
        attr(0),
        parent(0),
        children(0),
        offset(0),
        extendedAttributes(0),
        source(getFilename()),
        rank(level)
    {
    }

    Node(std::string name) :
        attr(0),
        parent(0),
        name(name),
        children(0),
        offset(0),
        extendedAttributes(0),
        source(getFilename()),
        rank(level)
    {
    }

    Node(NodeList* children) :
        attr(0),
        parent(0),
        offset(0),
        extendedAttributes(0),
        source(getFilename()),
        rank(level)
    {
        setChildren(children);
    }

    Node(std::string name, NodeList* children) :
        attr(0),
        parent(0),
        name(name),
        offset(0),
        extendedAttributes(0),
        source(getFilename()),
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

    const std::string& getSource() const
    {
        return source;
    }

    std::string& getMeta() const
    {
        return meta;
    }

    uint32_t getAttr() const
    {
        return attr;
    }

    void setAttr(uint32_t attr)
    {
        this->attr = attr;
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

    virtual void addFront(Node* node)
    {
        if (children == 0)
        {
            children = new NodeList;
        }
        children->push_front(node);
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

    int getChildrenCount() const
    {
        if (!children)
        {
            return 0;
        }
        return children->size();
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

    Node* front() const
    {
        assert(children);
        return children->front();
    }

    std::string& getName()
    {
        return name;
    }

    const std::string& getName() const
    {
        return name;
    }

    std::string getQualifiedName() const;

    std::string getQualifiedModuleName() const;

    // If there is an interface definition as well as interface forward declaration,
    // search() must return the interface definition.
    virtual Node* search(const std::string& elem, size_t pos = 0) const;

    virtual int compare(const char* str, const Node* scope) const
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

    virtual bool isByte(const Node* scope) const
    {
        return compare("byte", scope) == 0;
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

    virtual bool isUnsignedByte(const Node* scope) const
    {
        return compare("unsigned byte", scope) == 0;
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

    virtual bool isInterface(const Node* scope) const
    {
        return false;
    }

    virtual bool isFloat(const Node* scope) const
    {
        return compare("float", scope) == 0;
    }

    virtual bool isDouble(const Node* scope) const
    {
        return compare("double", scope) == 0;
    }

    virtual Module* isModule(const Node* scope)
    {
        return 0;
    }

    virtual Member* isTypedef(const Node* scope) const
    {
        return 0;
    }

    virtual SequenceType* isSequence(const Node* scope)
    {
        return 0;
    }

    virtual ArrayType* isArray(const Node* scope)
    {
        return 0;
    }

    virtual StructType* isStruct(const Node* scope)
    {
        return 0;
    }

    virtual NativeType* isNative(const Node* scope)
    {
        return 0;
    }

    virtual bool isConstructor() const
    {
        return false;
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

    void setRank(int r)
    {
        rank = r;
    }

    bool isDefinedIn(const char* source) const
    {
        return getRank() == 1 && (!source || getSource() == source || getSource() == "");
    }

    void setExtendedAttributes(NodeList* list)
    {
        // TODO: The following assert should be enabled.
        // assert(extendedAttributes == 0);
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

    static const char* getBaseObjectName()
    {
        return baseObjectName;
    }

    bool isBaseObject() const
    {
        return getQualifiedName() == getBaseObjectName();
    }

    static void setFlatNamespace(const char* name)
    {
        namespaceName = name;
    }

    static const char* getFlatNamespace()
    {
        return namespaceName ;
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

    virtual int compare(const char* str, const Node* scope) const
    {
        Node* node = search(scope);
        if (!node)
        {
            return *str;
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

    virtual ArrayType* isArray(const Node* scope)
    {
        Node* node = search(scope);
        if (!node)
        {
            return 0;
        }
        return node->isArray(node->getParent());
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

    virtual Module* isModule(const Node* scope)
    {
        Node* node = search(scope);
        if (!node)
        {
            return 0;
        }
        return node->isModule(node->getParent());
    }

    virtual Member* isTypedef(const Node* scope) const;

    virtual void accept(Visitor* visitor);

    std::string getIdentifier() const
    {
        size_t pos = getName().rfind("::");
        if (pos == std::string::npos)
        {
            return getName();
        }
        return getName().substr(pos + 2);
    }
};

class Module : public Node
{
    int interfaceCount; // of rank 1
    int constCount;     // of rank 1
    int moduleCount;    // of rank 1
    std::string prefix;

public:
    Module(std::string identifier) :
        Node(identifier),
        interfaceCount(0),
        constCount(0),
        moduleCount(0)
    {
        rank = 1;   // 'cause namespace is open.
        javadoc = ::getJavadoc();
    }

    virtual void add(Node* node);

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

    bool hasPredeclarations() const;

    std::string getPrefix() const
    {
        return prefix;
    }

    void processExtendedAttributes();

    std::string getPrefixedName() const
    {
        std::string body;
        if (0 < getName().length())
        {
            body = "::" + getName();
        }

        if (prefix.compare(0, 2, "::") == 0)
        {
            return prefix + body;
        }
        if (0 < prefix.length())
        {
            if (Module* parent = dynamic_cast<Module*>(getParent()))
            {
                return parent->getPrefixedName() + "::" + prefix + body;
            }
            return "::" + prefix + body;
        }
        if (getQualifiedName() == "::dom")
        {
            return "::org::w3c::dom";
        }
        if (Module* parent = dynamic_cast<Module*>(getParent()))
        {
            return parent->getPrefixedName() + body;
        }
        return "::org::w3c::dom" + body;
    }

    virtual Module* isModule(const Node* scope)
    {
        return this;
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

class Implements : public Node
{
    ScopedName* first;
    ScopedName* second;
public:
    Implements(ScopedName* first, ScopedName* second) :
        first(first),
        second(second)
    {
    }

    ScopedName* getFirst()
    {
        return first;
    }
    ScopedName* getSecond()
    {
        return second;
    }

    void resolve(bool importImplements);

    virtual void accept(Visitor* visitor);
};

class Interface : public Node
{
    Node* extends;
    int constCount;
    int methodCount;
    mutable int interfaceCount;  // total number of non-supplemental interfaces this interface will implement excluding Object.
    Interface* constructor;
    std::list<const Interface*> supplementalList;
    std::list<const Interface*> implementList;
    std::list<const Interface*> superList;

public:
    Interface(std::string identifier, Node* extends = 0, bool forward = false) :
        Node(identifier),
        extends(extends),
        constCount(0),
        methodCount(0),
        interfaceCount(0),
        constructor(0)
    {
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
    void processExtendedAttributes();
    void processExtendedAttributes(OpDcl* op);
    void processExtendedAttributes(Attribute* attr);

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
            if (!base)
            {
                continue;
            }
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

    int getInterfaceCount() const;
    void getInterfaceList(std::list<const Interface*>* list) const;

    int addMethodCount(int count)
    {
        methodCount += count;
        return methodCount;
    }

    int getSuperCount() const
    {
        Node* extends = getExtends();
        if (!extends)
        {
            return 0;
        }
        return extends->getChildrenCount();
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
                if (!super)
                {
                    fprintf(stderr, "%s not found\n", scoped->getName().c_str());
                    exit(EXIT_FAILURE);
                }
                break;  // XXX Needs to support multiple inheritance is not allowed.
            }
        }
        return super;
    }

    virtual bool isConstructor() const
    {
        return dynamic_cast<const Interface*>(getParent());
    }

    Interface* getConstructor() const
    {
        return constructor;
    }

    const std::list<const Interface*>* getSuperList() const
    {
        return &superList;
    }

    const std::list<const Interface*>* getSupplementals() const
    {
        return &supplementalList;
    }

    const std::list<const Interface*>* getImplements() const
    {
        return &implementList;
    }

    uint32_t isCallback() const
    {
        return (getAttr() & CallbackMask);
    }

    virtual void accept(Visitor* visitor);

    void implements(Interface* mixin, bool importImplements)
    {
        // cf. http://lists.w3.org/Archives/Public/public-webapps/2009JulSep/0528.html
        if (!(mixin->attr & Supplemental))
        {
            implementList.push_back(mixin);
        }
        if ((mixin->attr & Supplemental) || importImplements)
        {
            supplementalList.push_back(mixin);

            mixin->setAttr(mixin->getAttr() | ImplementedOn);
        }
    }

    void collectSupplementals(std::list<const Interface*>* interfaceList) const
    {
        for (std::list<const Interface*>::const_reverse_iterator i = getSupplementals()->rbegin();
             i != getSupplementals()->rend();
             ++i)
        {
            assert(!(*i)->isLeaf());
            (*i)->collectSupplementals(interfaceList);
        }
        interfaceList->push_front(this);
    }

    void collectMixins(std::list<const Interface*>* interfaceList) const;

    void adjustMethodCount();
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
        Node("seqence"),  // Default name
        spec(spec),
        max(max)
    {
    }

    ~SequenceType()
    {
        // delete spec;
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

    unsigned getLength(const Node* scope);

    virtual SequenceType* isSequence(const Node* scope)
    {
        return this;
    }

    virtual void accept(Visitor* visitor);
};

class ArrayType : public Node
{
    Node* spec;
    Node* max;

public:
    ArrayType(Node* max = 0) :
        Node("array"),  // Default name
        spec(0),
        max(max)
    {
    }

    ~ArrayType()
    {
        // delete spec;
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
        if (ArrayType* array = dynamic_cast<ArrayType*>(this->spec))
        {
            array->setSpec(spec);
        }
        else
        {
            this->spec = spec;
        }
    }

    Node* getMax() const
    {
        return max;
    }

    unsigned getLength(const Node* scope);

    virtual bool isInterface(const Node* scope) const
    {
        return true;
    }

    virtual ArrayType* isArray(const Node* scope)
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
    operator int8_t() const;
    operator int16_t() const;
    operator int32_t() const;
    operator int64_t() const;
    operator uint8_t() const;
    operator uint16_t() const;
    operator uint32_t() const;
    operator uint64_t() const;
    operator float() const;
    operator double() const;
    operator std::string() const;

    virtual void accept(Visitor* visitor);
};

class Member : public Node
{
    Node* spec;
    bool  type;  // true if this is a typedef

public:
    Member(std::string identifier, Node* spec = 0, uint32_t attr = 0) :
        Node(identifier),
        spec(spec),
        type(false)
    {
        javadoc = ::getJavadoc();
        setAttr(attr);
    }

    Member(const Member& m) :
        Node(m.getName()),
        spec(m.spec),
        type(m.type)
    {
        setAttr(m.attr);
    }

    ~Member()
    {
        delete spec;
    }

    virtual Node* getSpec() const
    {
        return spec;
    }

    void setSpec(Node* node)
    {
        spec = node;
    }

    void setTypedef(bool type)
    {
        this->type = type;
    }

    virtual int compare(const char* str, const Node* scope) const
    {
        if (!type)
        {
            return *str;
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

    virtual ArrayType* isArray(const Node* scope)
    {
        if (!type)
        {
            return 0;
        }
        return spec->isArray(scope);
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

    virtual Module* isModule(const Node* scope)
    {
        if (!type)
        {
            return 0;
        }
        return spec->isModule(scope);
    }

    virtual Member* isTypedef(const Node* scope) const
    {
        return type ? const_cast<Member*>(this) : 0;
    }

    virtual void accept(Visitor* visitor);
};

class Attribute : public Member
{
    bool readonly;
    Node* getraises;
    Node* setraises;
    std::string putForwards;

    mutable std::string metaGetter;
    mutable std::string metaSetter;

public:
    Attribute(std::string identifier, Node* spec, bool readonly = false) :
        Member(identifier, spec, 0),
        readonly(readonly),
        getraises(0),
        setraises(0)
    {
    }

    std::string& getMetaGetter() const
    {
        return metaGetter;
    }

    std::string& getMetaSetter() const
    {
        return metaSetter;
    }

    bool isReadonly() const
    {
        return readonly;
    }

    Node* getGetRaises() const
    {
        return getraises;
    }

    void setGetRaises(Node* raises)
    {
        getraises = raises;
    }

    int getGetRaisesCount() const
    {
        if (getraises == 0)
        {
            return 0;
        }
        return getraises->getSize();
    }

    Node* getSetRaises() const
    {
        return setraises;
    }

    void setSetRaises(Node* raises)
    {
        setraises = raises;
    }

    int getSetRaisesCount() const
    {
        if (setraises == 0)
        {
            return 0;
        }
        return setraises->getSize();
    }

    bool isStringifier() const
    {
        return getAttr() & Stringifier;
    }

    void processExtendedAttributes();

    bool isReplaceable() const
    {
        return getAttr() & Replaceable;
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
    mutable uint64_t value;

public:
    ConstDcl(Node* spec, std::string identifier, Node* exp) :
        Member(identifier, spec),
        exp(exp)
    {
    }

    ~ConstDcl()
    {
        delete exp;
    }

    Node* getExp() const
    {
        return exp;
    }

    uint64_t getValue() const
    {
        return value;
    }

    void setValue(uint64_t value) const
    {
        assert(value == 0 || this->value == 0);
        this->value = value;
    }

    Type* getType() const
    {
        Node* type = getSpec();
        if (ScopedName* scopedName = dynamic_cast<ScopedName*>(type))
        {
            type = scopedName->search(getParent());
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
    int methodCount;
    std::vector<int> paramCounts;  // for each method

    mutable std::vector<std::string> metaOps;

public:
    OpDcl(std::string identifier, Node* spec) :
        Member(identifier, spec, 0),
        raises(0),
        paramCount(0),
        methodCount(1)
    {
        children = new NodeList;
        if (identifier == "")
        {
            attr |= UnnamedProperty;
        }
    }

    virtual void add(Node* node);
    void processExtendedAttributes();

    std::string& getMetaOp(int i) const
    {
        assert(i < methodCount);
        if (metaOps.size() < methodCount)
        {
            metaOps.resize(methodCount);
        }
        return metaOps.at(i);
    }

    Node* getRaises() const
    {
        return raises;
    }

    void setRaises(Node* raises)
    {
        this->raises = raises;
    }

    int getRaisesCount() const
    {
        if (raises == 0)
        {
            return 0;
        }
        return raises->getSize();
    }

    // Adjust methodCount for [Callback] and [Optional].
    // Need to be called after the source file is completely read.
    void adjustMethodCount();

    int getParamCount() const
    {
        return paramCount;
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

    virtual Node* getSpec() const;

    virtual void accept(Visitor* visitor);
};

class ParamDcl : public Member
{
public:
    ParamDcl(std::string identifier, Node* spec, uint32_t attr) :
        Member(identifier, spec, attr)
    {
    }

    bool isOptional() const
    {
        return getAttr() & Optional;
    }

    bool isVariadic() const
    {
        return getAttr() & Variadic;
    }

    void processExtendedAttributes();

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
        if (!node || node->isLeaf())
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

    virtual void at(const Implements* node)
    {
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

    virtual void at(const ArrayType* node)
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

    virtual void at(const Member* node)
    {
        at(static_cast<const Node*>(node));
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

inline void Implements::accept(Visitor* visitor)
{
    visitor->at(this);
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

inline void ArrayType::accept(Visitor* visitor)
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

inline void Member::accept(Visitor* visitor)
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

class ProcessExtendedAttributes : public Visitor
{
public:
    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const Module* node)
    {
        const_cast<Module*>(node)->processExtendedAttributes();
        visitChildren(node);
    }

    virtual void at(const Interface* node)
    {
        if (!node->isLeaf())
        {
            for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
            {
                (*i)->accept(this);
                if (OpDcl* op = dynamic_cast<OpDcl*>(*i))
                {
                    const_cast<Interface*>(node)->processExtendedAttributes(op);
                }
                else if (Attribute* attr = dynamic_cast<Attribute*>(*i))
                {
                    const_cast<Interface*>(node)->processExtendedAttributes(attr);
                }
            }
        }
        const_cast<Interface*>(node)->processExtendedAttributes();
    }

    virtual void at(const Attribute* node)
    {
        const_cast<Attribute*>(node)->processExtendedAttributes();
        visitChildren(node);
    }

    virtual void at(const OpDcl* node)
    {
        const_cast<OpDcl*>(node)->processExtendedAttributes();
        visitChildren(node);
    }

    virtual void at(const ParamDcl* node)
    {
        const_cast<ParamDcl*>(node)->processExtendedAttributes();
        visitChildren(node);
    }
};

class AdjustMethodCount : public Visitor
{
    bool importImplements;
public:
    AdjustMethodCount(bool importImplements) :
        importImplements(importImplements)
    {
    }
    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const Implements* node)
    {
        const_cast<Implements*>(node)->resolve(importImplements);
    }

    virtual void at(const OpDcl* node)
    {
        const_cast<OpDcl*>(node)->adjustMethodCount();
    }

    virtual void at(const Interface* node)
    {
        visitChildren(node);
        const_cast<Interface*>(node)->adjustMethodCount();
    }
};

int printJava(const char* indent);
int printCPlusPlus(const char* stringTypeName, const char* objectTypeName,
                   bool useExceptions, bool useVirtualBase, const char* indent);

extern void print();
extern void printCxx(const char* source, const char* stringTypeName, const char* objectTypeName,
                     bool useExceptions, bool useVirtualBase, const char* indent);
extern void printSkeleton(const char* source, bool isystem, const char* indent);
extern void printTemplate(const char* source, const char* stringTypeName, const char* objectTypeName,
                          bool useExceptions, bool isystem, const char* indent);

extern std::string getOutputFilename(std::string, const char* suffix);
extern std::string getIncludedName(const std::string& header);

extern std::string getScopedName(std::string moduleName, std::string absoluteName);

extern const char* getIncludePath();
extern void setIncludePath(const char* path);

extern int input(int fd,
                 bool isystem, bool useExceptions, const char* stringTypeName);

extern int output(const char* filename,
                  bool isystem, bool useExceptions, bool useMultipleInheritance,
                  const char* stringTypeName, const char* objectTypeName, const char* indent,
                  bool skeleton,
                  bool generic);

#endif  // ESIDL_H_INCLUDED
