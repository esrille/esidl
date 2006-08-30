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

#include <string>
#include <es/irf.h>
#include <es/list.h>
#include <es/uuid.h>

#ifdef __cplusplus
extern "C"{
#endif  // #ifdef __cplusplus

extern char* UuidToAscii(const Guid* u, char* str);
extern int ScanUuid(const char* str, Guid* u);

#ifdef __cplusplus
}
#endif  // #ifdef __cplusplus

class Visitor;

class Attribute
{
public:
    Link<Attribute> link;

    virtual ~Attribute()
    {
    }
};

class AttributeList : public List<Attribute, &Attribute::link>
{
public:
    void addLast(Attribute* attr)
    {
        if (attr)
        {
            List<Attribute, &Attribute::link>::addLast(attr);
        }
    }
};

class Expression;

class Identifier
{
    std::string name;
    int importDepth;    // depth of nested imports
    char* comment;

public:
    Link<Identifier> link;

    Identifier(const char* name = 0) : importDepth(0), comment(0)
    {
        if (name)
        {
            this->name = name;
        }
    }

    virtual ~Identifier()
    {
        if (comment)
        {
            free(comment);
        }
    }

    virtual void accept(Visitor& visitor);

    const std::string& getName() const
    {
        return name;
    }

    void setName(const char* name)
    {
        this->name = name;
    }

    void setComment(char* comment)
    {
        if (comment)
        {
            this->comment = comment;
        }
    }

    const char* getComment() const
    {
        return comment;
    }

    bool equals(const char* id) const
    {
        return name == id;
    }

    void setImportDepth(const int importDepth)
    {
        this->importDepth = importDepth;
    }

    int getImportDepth()
    {
        return importDepth;
    }

    bool isImported() const
    {
        return (0 < importDepth);
    }
};

typedef List<Identifier, &Identifier::link> IdentifierList;

class Integer : public Identifier
{
    int bits;
    bool sign;
public:
    Integer(int bits = 32, bool sign = true) : bits(bits), sign(sign)
    {
    }

    int getBits() const
    {
        return bits;
    }

    bool isSigned() const
    {
        return sign;
    }

    void accept(Visitor& visitor);
};

class Character : public Identifier
{
public:
    void accept(Visitor& visitor);
};

class Float : public Identifier
{
    int bits;
public:
    Float(int bits = 64) : bits(bits)
    {
    };

    int getBits() const
    {
        return bits;
    }

    void accept(Visitor& visitor);
};

class Boolean : public Identifier
{
public:
    void accept(Visitor& visitor);
};

class Void : public Identifier
{
public:
    void accept(Visitor& visitor);
};

class UuidType : public Identifier
{
public:
    void accept(Visitor& visitor);
};

class Declarator : public Identifier
{
    Identifier* type;
    int pointer;
    AttributeList* attributes;
    bool typeName;

    int iid_is;
    int size_is;

public:
    Declarator(const char* id = 0) :
        Identifier(id), type(0), pointer(0), attributes(0), typeName(false),
        iid_is(-1), size_is(-1)
    {

    }

    const Identifier* getType() const
    {
        return type;
    }

    int getPointer() const
    {
        return pointer;
    }

    const AttributeList* getAttributes() const
    {
        return attributes;
    }

    void setType(Identifier* type)
    {
        this->type = type;
    }

    void setPointer(int pointer)
    {
        this->pointer += pointer;   // XXX hack
    }

    void setAttributes(AttributeList* attributes)
    {
        this->attributes = attributes;
    }

    bool isTypename() const
    {
        return typeName;
    }

    void setTypename(bool typeName)
    {
        this->typeName = typeName;
    }

    void accept(Visitor& visitor);

    void setIid(int n)
    {
        iid_is = n;
    }
    int getIid() const
    {
        return iid_is;
    }

    void setSize(int n)
    {
        size_is = n;
    }
    int getSize() const
    {
        return size_is;
    }
};

// Function or Operation
class Function : public Declarator
{
    mutable IdentifierList params;
    int numParams;

public:
    Function() : numParams(0) { }

    int getNumParams() const
    {
        return numParams;
    }

    void addLast(Declarator* param)
    {
        params.addLast(param);
        ++numParams;
    }

    IdentifierList::Iterator begin() const
    {
        return params.begin();
    }

    IdentifierList::Iterator end() const
    {
        return params.end();
    }

    int getParamNum(const char* name) const
    {
        int paramNum(-1);
        Identifier* id;
        IdentifierList::Iterator iter = begin();
        while ((id = iter.next()))
        {
            ++paramNum;
            if (id->equals(name))
            {
                break;
            }
        }
        return paramNum;
    }

    void accept(Visitor& visitor);
};

class Ref : public Declarator
{
public:
    Ref(const char* id = 0) : Declarator(id)
    {
    }

    void accept(Visitor& visitor);
};

class Array : public Declarator
{
    mutable AttributeList* bounds;

public:
    Array(const char* id, AttributeList* bounds) :
        Declarator(id), bounds(bounds)
    {
    }

    AttributeList::Iterator begin() const
    {
        return bounds->begin();
    }

    AttributeList::Iterator end() const
    {
        return bounds->end();
    }

    void accept(Visitor& visitor);
};

class BoundAttribute : public Attribute
{
    Expression* exp;
public:
    BoundAttribute(Expression* exp = 0) : exp(exp)
    {
    }

    Expression* getExpression() const
    {
        return exp;
    }
};

class InAttribute : public Attribute
{
};

class OutAttribute : public Attribute
{
};

class FieldAttribute : public Attribute
{
    std::string id;
public:
    FieldAttribute(const char* id)
    {
        if (id)
        {
            this->id = id;
        }
    }
    const std::string& getName() const
    {
        return id;
    }
};

class IidIsAttribute : public FieldAttribute
{
public:
    IidIsAttribute(const char* id) : FieldAttribute(id)
    {
    }
};

class SizeIsAttribute : public FieldAttribute
{
public:
    SizeIsAttribute(const char* id) : FieldAttribute(id)
    {
    }
};

class VarAttribute : public Attribute
{
    std::string id;
    int pointer;
public:
    VarAttribute(const char* id = 0, int pointer = 0) : pointer(pointer)
    {
        if (id)
        {
            this->id = id;
        }
    }
};

class RefAttribute : public Attribute
{
};

class UniqueAttribute : public Attribute
{
};

class FullAttribute : public Attribute
{
};

class Expression
{
public:
    virtual ~Expression() { }

    virtual operator int() const
    {
        return 0;
    }

    virtual operator bool() const
    {
        return (operator int()) ? true : false;
    }

    virtual operator double() const
    {
        return operator int();
    }

    virtual operator char() const
    {
        return 0;
    }

    virtual operator const char*() const
    {
        return "(null)";
    }

    virtual int getBase() const
    {
        return 10;
    }
};

template<typename T>
class Literal : public Expression
{
    T   value;
    int base;
public:
    Literal(T value, int base = 10) : value(value), base(base)
    {
    }

    operator T() const
    {
        return value;
    }

    int getBase() const
    {
        return base;
    }
};

template<>
class Literal<char*> : public Expression
{
    std::string value;
public:
    Literal(char* value)
    {
        if (value)
        {
            this->value = value;
        }
    }

    operator const char*() const
    {
        return value.c_str();
    }
};

class Constant : public Expression, public Declarator
{
    Expression* exp;

public:
    Link<Constant> link;

    Constant(Identifier* type, int pointer, const char* id, Expression* exp = 0) :
        Declarator(id), exp(exp)
    {
        setType(type);
        setPointer(pointer);
    }

    Expression* getExpression() const
    {
        return exp;
    }

    void setExpression(Expression* exp)
    {
        this->exp = exp;
    }

    void accept(Visitor& visitor);

    operator int() const
    {
        return static_cast<int>(*exp);
    }

    operator bool() const
    {
        return static_cast<bool>(*exp);
    }

    operator double() const
    {
        return static_cast<double>(*exp);
    }

    operator char() const
    {
        return static_cast<char>(*exp);
    }

    operator const char*() const
    {
        return static_cast<const char*>(*exp);
    }
};

typedef List<Constant, &Constant::link> ConstantList;

class UnaryExpression : public Expression
{
    Expression* operand;
protected:
    UnaryExpression(Expression* operand) :
        operand(operand)
    {

    }

    Expression* getOperand() const
    {
        return operand;
    }
};

class BinaryExpression : public Expression
{
    Expression* left;
    Expression* right;
protected:
    BinaryExpression(Expression* left, Expression* right) :
        left(left), right(right)
    {
    }

    Expression* getLeft() const
    {
        return left;
    }

    Expression* getRight() const
    {
        return right;
    }
};

class TrinaryExpression : public Expression
{
    Expression* left;
    Expression* middle;
    Expression* right;
protected:
    TrinaryExpression(Expression* left, Expression* middle, Expression* right) :
        left(left), middle(middle), right(right)
    {
    }

    Expression* getLeft() const
    {
        return left;
    }

    Expression* getMiddle() const
    {
        return middle;
    }

    Expression* getRight() const
    {
        return right;
    }
};

// +
class Plus : public UnaryExpression
{
public:
    Plus(Expression* exp) : UnaryExpression(exp)
    {

    }

    operator int() const
    {
        return +static_cast<int>(*getOperand());
    }
};

// -
class Minus : public UnaryExpression
{
public:
    Minus(Expression* exp) : UnaryExpression(exp)
    {
    }

    operator int() const
    {
        return -static_cast<int>(*getOperand());
    }
};

// ~
class Tilde : public UnaryExpression
{
public:
    Tilde(Expression* exp) : UnaryExpression(exp)
    {

    }

    operator int() const
    {
        return ~static_cast<int>(*getOperand());
    }
};

// !
class Not : public UnaryExpression
{
public:
    Not(Expression* exp) : UnaryExpression(exp)
    {
    }

    operator int() const
    {
        return !static_cast<int>(*getOperand());
    }
};

// ||
class LogicalOr : public BinaryExpression
{
public:
    LogicalOr(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) || static_cast<int>(*getRight());
    }
};

// &&
class LogicalAnd : public BinaryExpression
{
public:
    LogicalAnd(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) && static_cast<int>(*getRight());
    }
};

// |
class Or : public BinaryExpression
{
public:
    Or(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) | static_cast<int>(*getRight());
    }
};

// ^
class Xor : public BinaryExpression
{
public:
    Xor(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) ^ static_cast<int>(*getRight());
    }
};

// &
class And : public BinaryExpression
{
public:
    And(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) & static_cast<int>(*getRight());
    }
};

// ==
class Eq : public BinaryExpression
{
public:
    Eq(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) == static_cast<int>(*getRight());
    }
};

// !=
class Ne : public BinaryExpression
{
public:
    Ne(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) != static_cast<int>(*getRight());
    }
};

// <
class Lt : public BinaryExpression
{
public:
    Lt(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) < static_cast<int>(*getRight());
    }
};

// <=
class Le : public BinaryExpression
{
public:
    Le(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) <= static_cast<int>(*getRight());
    }
};

// >
class Gt : public BinaryExpression
{
public:
    Gt(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) > static_cast<int>(*getRight());
    }
};

// >=
class Ge : public BinaryExpression
{
public:
    Ge(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) >= static_cast<int>(*getRight());
    }
};

// <<
class ShiftLeft : public BinaryExpression
{
public:
    ShiftLeft(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) << static_cast<int>(*getRight());
    }
};

// >>
class ShiftRight : public BinaryExpression
{
public:
    ShiftRight(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) >> static_cast<int>(*getRight()); // XXX unsigned
    }
};

// +
class Add : public BinaryExpression
{
public:
    Add(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) + static_cast<int>(*getRight());
    }
};

// -
class Sub : public BinaryExpression
{
public:
    Sub(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) - static_cast<int>(*getRight());
    }
};

// *
class Mul : public BinaryExpression
{
public:
    Mul(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) * static_cast<int>(*getRight());
    }
};

// /
class Div : public BinaryExpression
{
public:
    Div(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) / static_cast<int>(*getRight());
    }
};

// %
class Mod : public BinaryExpression
{
public:
    Mod(Expression* left, Expression* right) :
        BinaryExpression(left, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) % static_cast<int>(*getRight());
    }
};

// ? :
class Cond : public TrinaryExpression
{
public:
    Cond(Expression* left, Expression* middle, Expression* right) :
        TrinaryExpression(left, middle, right)
    {
    }

    operator int() const
    {
        return static_cast<int>(*getLeft()) ? static_cast<int>(*getMiddle())
                                             : static_cast<int>(*getRight());
    }
};

class Import
{
    std::string filename;
    std::string header;
    int importDepth;

public:
    Link<Import> link;

    Import(const char* filename, int importDepth = 0) :
        filename(filename),
        importDepth(importDepth)
    {
        header = filename;
        header.erase(header.size() - 1);
        header.erase(0, 1);

        int i = header.rfind(".");
        if (i != header.npos)
        {
            header.erase(i);
        }
        header += ".h";

        header = "<" + header + ">";
    }

    const std::string& getFilename() const
    {
        return filename;
    }

    const std::string& getHeaderName() const
    {
        return header;
    }

    int getImportDepth()
    {
        return importDepth;
    }

    bool isImported() const
    {
        return (0 < importDepth);
    }
};

typedef List<Import, &Import::link> ImportList;

class NameSpace : public Identifier
{
    mutable IdentifierList members;
    mutable ImportList imports;
    NameSpace* parentSpace;
    int numMembers;
    int numInterfaces;

public:
    NameSpace(const char* name = 0, NameSpace* parentSpace = 0) :
        Identifier(name), parentSpace(parentSpace),
        numMembers(0), numInterfaces(0)
    {
    }

    int getNumMembers() const
    {
        return numMembers;
    }

    int getNumInterfaces() const
    {
        return numInterfaces;
    }

    virtual void addLast(Identifier* member, int importDepth = 0);

    IdentifierList::Iterator begin() const
    {
        return members.begin();
    }

    IdentifierList::Iterator end() const
    {
        return members.end();
    }

    Identifier* lookup(const char* id)
    {
        Identifier* t;
        IdentifierList::Iterator iter = begin();
        while ((t = iter.next()))
        {
            if (t->equals(id))
            {
                return t;
            }
        }
        if (parentSpace)
        {
            t = parentSpace->lookup(id);
        }
        return t;
    }

    void addImport(Import* import)
    {
        imports.addLast(import);
    }
    ImportList::Iterator beginImports() const
    {
        return imports.begin();
    }
    ImportList::Iterator endImports() const
    {
        return imports.end();
    }

    NameSpace* parent() const
    {
        return parentSpace;
    }

    //
    // static members
    //
private:
    static Integer      s8Type;
    static Integer      s16Type;
    static Integer      s32Type;
    static Integer      s64Type;
    static Integer      u8Type;
    static Integer      u16Type;
    static Integer      u32Type;
    static Integer      u64Type;
    static Character    characterType;
    static Float        floatType;
    static Float        doubleType;
    static Boolean      booleanType;
    static Void         voidType;
    static UuidType     uuidType;
    static NameSpace    globalSpace;
    static NameSpace*   localSpace;

public:
    static Integer* newInteger(int bits = 32, bool sign = true)
    {
        if (sign)
        {
            switch (bits)
            {
              case 8:
                return &s8Type;
                break;
              case 16:
                return &s16Type;
                break;
              case 32:
                return &s32Type;
                break;
              case 64:
                return &s64Type;
                break;
            }
        }
        else
        {
            switch (bits)
            {
              case 8:
                return &u8Type;
                break;
              case 16:
                return &u16Type;
                break;
              case 32:
                return &u32Type;
                break;
              case 64:
                return &u64Type;
                break;
            }
        }
    }
    static Character* newCharacter()
    {
        return &characterType;
    }

    static Float* newFloat(int bits = 64)
    {
        switch (bits)
        {
          case 32:
            return &floatType;
            break;
          case 64:
            return &doubleType;
            break;
        }
    }

    static Boolean* newBoolean()
    {
        return &booleanType;
    }

    static Void* newVoid()
    {
        return &voidType;
    }

    static UuidType* newUuid()
    {
        return &uuidType;
    }

    static NameSpace& global()
    {
        return globalSpace;
    }

    static NameSpace& local()
    {
        return *localSpace;
    }

    static void setLocal(NameSpace* local)
    {
        localSpace = local;
    }

    void accept(Visitor& visitor);
};

class Enum : public NameSpace
{
public:
    Enum(const char* tag = 0, NameSpace* parent = 0) :
        NameSpace(tag, parent)
    {
    }

    void accept(Visitor& visitor);
};

class Structure : public NameSpace
{
public:
    Structure(const char* tag = 0, NameSpace* parent = 0) :
        NameSpace(tag, parent)
    {
    }

    void accept(Visitor& visitor);
};

class Interface : public Structure
{
    AttributeList* attributes;
    std::string extends;
    int numMethods;

public:
    Interface(const char* tag = 0, NameSpace* parent = 0) :
        Structure(tag, parent), attributes(0), numMethods(0)
    {
    }

    int getNumMethods() const
    {
        return numMethods;
    }

    void addLast(Identifier* member, int importDepth = 0)
    {
        if (member)
        {
            if (dynamic_cast<Function*>(member))
            {
                ++numMethods;
            }
            NameSpace::addLast(member, importDepth);
        }
    }

    void accept(Visitor& visitor);

    const std::string& getExtends() const  // XXX Interface*
    {
        return extends;
    }
    void setExtends(const char* extends)
    {
        if (extends)
        {
            this->extends = extends;
        }
        else
        {
            this->extends = "";
        }
    }

    const Interface* getParentInterface() const
    {
        const std::string& parentName = extends;
        if (parentName != "")
        {
            NameSpace* ns = parent();
            if (!ns)
            {
                ns = &NameSpace::global();
            }

            IdentifierList::Iterator iter = ns->begin();
            Identifier* id;
            while ((id = iter.next()))
            {
                if (dynamic_cast<Interface*>(id) &&
                    id->getName() == parentName)
                {
                    return dynamic_cast<const Interface*>(id);
                }
            }
        }

        return 0;
    }

    void setAttributes(AttributeList* attributes)
    {
        this->attributes = attributes;
    }
    AttributeList::Iterator beginAttributes() const
    {
        return attributes->begin();
    }
    AttributeList::Iterator endAttributes() const
    {
        return attributes->end();
    }
};

inline void NameSpace::addLast(Identifier* member, int importDepth)
{
    if (member)
    {
        if (dynamic_cast<Interface*>(member))
        {
            ++numInterfaces;
        }
        member->setImportDepth(importDepth);
        members.addLast(member);
        ++numMembers;
    }
}

class UuidAttribute : public Attribute
{
    Guid guid;

public:
    UuidAttribute(char* uuid)
    {
        ScanUuid(uuid, &guid);
    }

    const Guid* getValue() const
    {
        return &guid;
    }
};

class ObjectAttribute : public Attribute
{

};

class Visitor
{
public:
    virtual void at(const Identifier* id) { }

    virtual void at(const Integer* i)
    {
        at(static_cast<const Identifier*>(i));
    }
    virtual void at(const Float* f)
    {
        at(static_cast<const Identifier*>(f));
    }
    virtual void at(const Character* ch)
    {
        at(static_cast<const Identifier*>(ch));
    }
    virtual void at(const Boolean* b)
    {
        at(static_cast<const Identifier*>(b));
    }
    virtual void at(const Void* v)
    {
        at(static_cast<const Identifier*>(v));
    }
    virtual void at(const UuidType* u)
    {
        at(static_cast<const Identifier*>(u));
    }

    virtual void at(const Declarator* decl)
    {
        at(static_cast<const Identifier*>(decl));
    }
    virtual void at(const Function* f)
    {
        at(static_cast<const Declarator*>(f));
    }
    virtual void at(const Constant* c)
    {
        at(static_cast<const Declarator*>(c));
    }
    virtual void at(const Ref* r)
    {
        at(static_cast<const Declarator*>(r));
    }
    virtual void at(const Array* a)
    {
        at(static_cast<const Declarator*>(a));
    }

    virtual void at(const NameSpace* ns)
    {
        at(static_cast<const Identifier*>(ns));
    }
    virtual void at(const Enum* e)
    {
        at(static_cast<const NameSpace*>(e));
    }
    virtual void at(const Structure* s)
    {
        at(static_cast<const NameSpace*>(s));
    }
    virtual void at(const Interface* i)
    {
        at(static_cast<const Structure*>(i));
    }
};

inline void Identifier::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Integer::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Float::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Character::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Boolean::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Void::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void UuidType::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Declarator::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Function::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Constant::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Ref::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Array::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void NameSpace::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Enum::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Structure::accept(Visitor& visitor)
{
    visitor.at(this);
}

inline void Interface::accept(Visitor& visitor)
{
    visitor.at(this);
}
