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

#ifndef ESIDL_STRING_H_INCLUDED
#define ESIDL_STRING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "esidl.h"

class StringOffsetter : public Visitor
{
    std::map<std::string, size_t>& dict;
    size_t offset;

    void add(const std::string& name)
    {
        if (0 < name.size() && dict.find(name) == dict.end())
        {
            dict[name] = offset;
            offset += name.length() + 1;
        }
    }

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
        add(node->getName());
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
        add(node->getQualifiedName());
        at(static_cast<const Node*>(node));
        if (Interface* super = node->getSuper())
        {
            add(super->getQualifiedName());
            super->accept(this);
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

#endif  // ESIDL_STRING_H_INCLUDED
