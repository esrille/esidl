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

#ifndef NINTENDO_ESIDL_EXPR_H_INCLUDED
#define NINTENDO_ESIDL_EXPR_H_INCLUDED

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <algorithm>
#include "esidl.h"
#include "utf.h"

Literal::operator bool() const
{
    if (name == "TRUE")
    {
        return true;
    }
    else if (name == "FALSE")
    {
        return false;
    }

    // syntax error
}

Literal::operator char() const
{
    if (name[0] != '\'' || name[name.size() - 1] != '\'')
    {
        // syntax error
    }

    std::string s = unescape(name, 1, name.size() - 2);
    return s[0];
}

Literal::operator short() const
{
    long long value = static_cast<long long>(*this);
    value = std::max(static_cast<long long>(SHRT_MIN), std::min(static_cast<long long>(SHRT_MAX), value));
    return static_cast<short>(value);
}

Literal::operator int() const
{
    long long value = static_cast<long long>(*this);
    value = std::max(static_cast<long long>(INT_MIN), std::min(static_cast<long long>(INT_MAX), value));
    return static_cast<short>(value);
}

Literal::operator long long() const
{
    const char* n = name.c_str();
    char* e;
    long long value = strtoll(n, &e, 0);
    if (!e || *e != '\0')
    {
        // syntax error
    }
    return value;
}

Literal::operator unsigned char() const
{
    unsigned long long value = static_cast<unsigned long long>(*this);
    value = std::min(static_cast<unsigned long long>(UCHAR_MAX), value);
    return static_cast<unsigned char>(value);
}

Literal::operator unsigned short() const
{
    unsigned long long value = static_cast<unsigned long long>(*this);
    value = std::min(static_cast<unsigned long long>(USHRT_MAX), value);
    return static_cast<unsigned short>(value);
}

Literal::operator unsigned int() const
{
    unsigned long long value = static_cast<unsigned long long>(*this);
    value = std::min(static_cast<unsigned long long>(UINT_MAX), value);
    return static_cast<unsigned int>(value);
}

Literal::operator unsigned long long() const
{
    const char* n = name.c_str();
    char* e;
    unsigned long long value = strtoull(n, &e, 0);
    if (!e || *e != '\0')
    {
        // syntax error
    }
    return value;
}

Literal::operator float() const
{
    const char* n = name.c_str();
    char* e;
    float value = strtof(n, &e);
    if (!e || *e != '\0')
    {
        // syntax error;
    }
    return value;
}

Literal::operator double() const
{
    const char* n = name.c_str();
    char* e;
    double value = strtod(n, &e);
    if (!e || *e != '\0')
    {
        // syntax error;
    }
    return value;
}

Literal::operator long double() const
{
    const char* n = name.c_str();
    char* e;
    long double value = strtold(n, &e);
    if (!e || *e != '\0')
    {
        // syntax error;
    }
    return value;
}

Literal::operator std::string() const
{
    size_t pos = 0;

    if (name[0] == '\"')
    {
        pos = 1;
    }
    else if (name.compare(0, 2, "L\"") == 0)
    {
        pos = 2;
    }
    else
    {
        // syntax error
    }

    return unescape(name, pos, name.size() - pos - 1);
}

template<class T>
class EvalInteger : public Visitor
{
    T value;
    const Node* scope;

public:
    EvalInteger(const Node* scope) :
        scope(scope)
    {
    }

    virtual void at(const ScopedName* node)
    {
        if (ConstDcl* c = dynamic_cast<ConstDcl*>(node->search(scope)))
        {
            c->getExp()->accept(this);
        }
        // else // syntax error
    }

    virtual void at(const BinaryExpr* node)
    {
        node->getLeft()->accept(this);
        T left = value;
        node->getRight()->accept(this);
        T right = value;
        if (node->getName() == "|")
        {
            value = left | right;
        }
        else if (node->getName() == "^")
        {
            value = left ^ right;
        }
        else if (node->getName() == "&")
        {
            value = left & right;
        }
        else if (node->getName() == ">>")
        {
            value = left >> right;
        }
        else if (node->getName() == "<<")
        {
            value = left << right;
        }
        else if (node->getName() == "+")
        {
            value = left + right;
        }
        else if (node->getName() == "-")
        {
            value = left - right;
        }
        else if (node->getName() == "*")
        {
            value = left * right;
        }
        else if (node->getName() == "/")
        {
            value = left / right;
        }
        else if (node->getName() == "%")
        {
            value = left % right;
        }
    }

    virtual void at(const UnaryExpr* node)
    {
        visitChildren(node);
        if (node->getName() == "-")
        {
            value = -value;
        }
        else if (node->getName() == "~")
        {
            value = ~value;
        }
    }

    virtual void at(const Literal* node)
    {
        value = static_cast<T>(*node);
    }

    T getValue() const
    {
        return value;
    }
};

template<class T>
class EvalFloat : public Visitor
{
    T value;
    const Node* scope;

public:
    EvalFloat(const Node* scope) :
        scope(scope)
    {
    }

    virtual void at(const ScopedName* node)
    {
        if (ConstDcl* c = dynamic_cast<ConstDcl*>(node->search(scope)))
        {
            c->getExp()->accept(this);
        }
        // else // syntax error
    }

    virtual void at(const BinaryExpr* node)
    {
        node->getLeft()->accept(this);
        T left = value;
        node->getRight()->accept(this);
        T right = value;
        if (node->getName() == "+")
        {
            value = left + right;
        }
        else if (node->getName() == "-")
        {
            value = left - right;
        }
        else if (node->getName() == "*")
        {
            value = left * right;
        }
        else if (node->getName() == "/")
        {
            value = left / right;
        }
    }

    virtual void at(const UnaryExpr* node)
    {
        if (node->getName() == "-")
        {
            value = -value;
        }
    }

    virtual void at(const Literal* node)
    {
        value = static_cast<T>(*node);
    }

    T getValue() const
    {
        return value;
    }
};

template<class T>
class EvalString : public Visitor
{
    T value;
    const Node* scope;

public:
    EvalString(const Node* scope) :
        scope(scope)
    {
    }

    virtual void at(const ScopedName* node)
    {
        if (ConstDcl* c = dynamic_cast<ConstDcl*>(node->search(scope)))
        {
            c->getExp()->accept(this);
        }
        // else // syntax error
    }

    virtual void at(const Literal* node)
    {
        value = static_cast<T>(*node);
    }

    T getValue() const
    {
        return value;
    }
};

class ConstOffsetter : public Visitor
{
    uint32_t offset;

public:
    ConstOffsetter(size_t base) :
        offset(base)
    {
    }

    uint32_t getOffset() const
    {
        return offset;
    }

    virtual void at(const Node* node)
    {
        if (1 < node->getRank())
        {
            return;
        }

        visitChildren(node);
    }

    virtual void at(const ConstDcl* node)
    {
        if (1 < node->getRank())
        {
            return;
        }

        Type* type = node->getType();
        assert(type);

        printf("const %s = ", node->getName().c_str());

        if (type->getName() == "boolean")
        {
            EvalInteger<bool> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%d", eval.getValue());
        }
        else if (type->getName() == "float")
        {
            EvalFloat<float> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%g", eval.getValue());
        }
        else if (type->getName() == "double")
        {
            offset += 7;
            offset &= ~7;
            node->setValue(offset);
            offset += 8;

            EvalFloat<double> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%g (%lld)", eval.getValue(), node->getValue());
        }
        else if (type->getName() == "long double")
        {
            offset += 15;
            offset &= ~15;
            node->setValue(offset);
            offset += 16;

            EvalFloat<long double> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%Lg (%lld)", eval.getValue(), node->getValue());
        }
        else if (type->getName() == "short")
        {
            EvalInteger<short> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%d", eval.getValue());
        }
        else if (type->getName() == "long")
        {
            EvalInteger<int> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%d", eval.getValue());
        }
        else if (type->getName() == "long long")
        {
            offset += 7;
            offset &= ~7;
            node->setValue(offset);
            offset += 8;

            EvalInteger<long long> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%lld (%lld)", eval.getValue(), node->getValue());
        }
        else if (type->getName() == "octet")
        {
            EvalInteger<unsigned char> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%u", eval.getValue());
        }
        else if (type->getName() == "unsigned short")
        {
            EvalInteger<unsigned short> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%u", eval.getValue());
        }
        else if (type->getName() == "unsigned long")
        {
            EvalInteger<unsigned int> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%u", eval.getValue());
        }
        else if (type->getName() == "unsigned long long")
        {
            offset += 7;
            offset &= ~7;
            node->setValue(offset);
            offset += 8;

            EvalInteger<unsigned long long> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%llu %lld", eval.getValue(), node->getValue());
        }
        else if (type->getName() == "char")
        {
            EvalString<char> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%c", eval.getValue());
        }
        else if (type->getName() == "string")
        {
            EvalString<std::string> eval(node->getParent());

            node->getExp()->accept(&eval);
            printf("%s (%zd)", eval.getValue().c_str(), offset);

            node->setValue(offset);
            offset += eval.getValue().length() + 1;
        }

        printf("\n");
    }
};

#endif  // NINTENDO_ESIDL_EXPR_H_INCLUDED
