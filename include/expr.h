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

#ifndef ESIDL_EXPR_H_INCLUDED
#define ESIDL_EXPR_H_INCLUDED

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <algorithm>
#include "esidl.h"
#include "utf.h"

inline Literal::operator bool() const
{
    if (name == "true")
    {
        return true;
    }
    else if (name == "false")
    {
        return false;
    }

    // syntax error
}

inline Literal::operator char() const
{
    if (name[0] != '\'' || name[name.size() - 1] != '\'')
    {
        // syntax error
    }

    std::string s = unescape(name, 1, name.size() - 2);
    return s[0];
}

inline Literal::operator int8_t() const
{
    int64_t value = static_cast<int64_t>(*this);
    return static_cast<int8_t>(value);
}

inline Literal::operator int16_t() const
{
    int64_t value = static_cast<int64_t>(*this);
    return static_cast<int16_t>(value);
}

inline Literal::operator int32_t() const
{
    int64_t value = static_cast<int64_t>(*this);
    return static_cast<int32_t>(value);
}

inline Literal::operator int64_t() const
{
    const char* n = name.c_str();
    char* e;
    long long value = strtoll(n, &e, 0);
    if (!e || *e != '\0')
    {
        // syntax error
    }
    return static_cast<int64_t>(value);
}

inline Literal::operator uint8_t() const
{
    uint64_t value = static_cast<uint64_t>(*this);
    return static_cast<uint8_t>(value);
}

inline Literal::operator uint16_t() const
{
    uint64_t value = static_cast<uint64_t>(*this);
    return static_cast<uint16_t>(value);
}

inline Literal::operator uint32_t() const
{
    uint64_t value = static_cast<uint64_t>(*this);
    return static_cast<uint32_t>(value);
}

inline Literal::operator uint64_t() const
{
    const char* n = name.c_str();
    char* e;
    unsigned long long value = strtoull(n, &e, 0);
    if (!e || *e != '\0')
    {
        // syntax error
    }
    return static_cast<uint64_t>(value);
}

inline Literal::operator float() const
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

inline Literal::operator double() const
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

inline Literal::operator std::string() const
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

#endif  // ESIDL_EXPR_H_INCLUDED
