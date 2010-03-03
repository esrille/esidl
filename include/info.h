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

#ifndef ESIDL_INFO_H_INCLUDED
#define ESIDL_INFO_H_INCLUDED

#include <stdlib.h>
#include <map>
#include <string>
#include "esidl.h"
#include "expr.h"
#include "formatter.h"
#include "reflect.h"

// Generate the string-encoded interface information for reflection
class Info : public Visitor, public Formatter
{
protected:
    bool constructorMode;
    int optionalStage;
    int optionalCount;

    std::string objectTypeName;
    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

    int getParamCount() const
    {
        return paramCount;
    }

    const ParamDcl* getVariadic() const
    {
        return variadicParam;
    }

    void printChildren(const Node* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        const Node* saved = currentNode;
        std::string separator;
        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            if (1 < (*i)->getRank())
            {
                continue;
            }
            if ((*i)->isNative(node->getParent()))
            {
                continue;
            }
            currentNode = (*i);
            (*i)->accept(this);
        }
        currentNode = saved;
    }

public:
    Info(Formatter* f, const std::string& objectTypeName) :
        Formatter(f),
        constructorMode(false),
        objectTypeName(objectTypeName),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0)
    {
    }

    virtual void at(const Module* node)
    {
        // Info{} visiter should be applied for interfaces.
    }

    virtual void at(const Attribute* node)
    {
        writetab();
        write("\"%s\"", node->getMetaGetter().c_str());
        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            writeln("");
            writetab();
            write("\"%s\"", node->getMetaSetter().c_str());
        }
    }

    virtual void at(const OpDcl* node)
    {
        writetab();
        if (constructorMode)
        {
            write("\"%c%s\"", Reflect::kConstructor, node->getMeta().c_str() + 1);

        }
        else
        {
            write("\"%c%s\"", Reflect::kOperation, node->getMeta().c_str() + 1);
        }
    }

    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (element->isSequence(interface) ||
            element->isNative(interface) ||
            element->isTypedef(interface) ||
            element->isInterface(interface))  // Do not process Constructor
        {
            return;
        }
        optionalStage = 0;
        do
        {
            write("\n");
            optionalCount = 0;
            element->accept(this);
            ++optionalStage;
        } while (optionalStage <= optionalCount);
    }

    virtual void at(const Interface* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        currentNode = node;

        writeln("");
        writetab();
        write("\"%s\"", node->getMeta().c_str());

        // Expand supplementals
        std::list<const Interface*> interfaceList;
        node->collectMixins(&interfaceList, node);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            const Node* saved = currentNode;
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                currentNode = *i;
                visitInterfaceElement(*i, *j);
            }
            currentNode = saved;
        }

        if (Interface* constructor = node->getConstructor())
        {
            // Process constructors.
            constructorMode = true;
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(node, *i);
            }
            constructorMode = false;
        }
    }

    virtual void at(const ConstDcl* node)
    {
        writetab();
        write("\"%s\"", node->getMeta().c_str());
    }

    virtual void at(const Member* node)
    {
        assert(node->isTypedef(node->getParent()));
        node->getSpec()->accept(this);
    }

    std::string getInterfaceName(std::string qualifiedName)
    {
        if (qualifiedName == Node::getBaseObjectName())
        {
            qualifiedName.replace(2, qualifiedName.length(), objectTypeName);
        }
        return qualifiedName;
    }
};

#endif  // ESIDL_INFO_H_INCLUDED
