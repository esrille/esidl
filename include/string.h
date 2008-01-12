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

#ifndef NINTENDO_ESIDL_STRING_H_INCLUDED
#define NINTENDO_ESIDL_STRING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "esidl.h"

class StringOffsetter : public Visitor
{
    std::map<std::string, size_t>& dict;
    size_t offset;

public:
    StringOffsetter(std::map<std::string, size_t>& dict, size_t base) :
        dict(dict),
        offset(base)
    {
    }

    size_t getOffset() const
    {
        return offset;
    }

    virtual void at(const Node* node)
    {
        if (1 < node->getRank())
        {
            return;
        }

        if (0 < node->getName().size())
        {
            if (dict.find(node->getName()) == dict.end())
            {
                dict[node->getName()] = offset;
                offset += node->getName().length() + 1;
            }
        }
        visitChildren(node);
    }

    virtual void at(const Include* node)
    {
    }

    virtual void at(const ScopedName* node)
    {
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank())
        {
            return;
        }

        at(static_cast<const Node*>(node));
        if (node->getExtends())
        {
            node->getExtends()->accept(this);
        }
    }

    virtual void at(const Type* node)
    {
    }

    virtual void at(const SequenceType* node)
    {
        at(static_cast<const Node*>(node));
        node->getSpec()->accept(this);
        if (node->getMax())
        {
            node->getMax()->accept(this);
        }
    }

    virtual void at(const Literal* node)
    {
    }

    virtual void at(const PragmaID* node)
    {
    }

    virtual void at(const Member* node)
    {
        at(static_cast<const Node*>(node));
        node->getSpec()->accept(this);
    }

    virtual void at(const OpDcl* node)
    {
        at(static_cast<const Member*>(node));
        if (node->getRaises())
        {
            node->getRaises()->accept(this);
        }
    }
};

#endif  // NINTENDO_ESIDL_STRING_H_INCLUDED
