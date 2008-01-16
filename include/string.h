/*
 * Copyright 2008 Google Inc.
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
    u32 offset;

public:
    StringOffsetter(std::map<std::string, size_t>& dict, size_t base) :
        dict(dict),
        offset(base)
    {
    }

    u32 getOffset() const
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
