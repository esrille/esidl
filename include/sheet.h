/*
 * Copyright 2010 Google Inc.
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

#ifndef ESIDL_SHEET_H_INCLUDED
#define ESIDL_SHEET_H_INCLUDED

#include "esidl.h"
#include <stdio.h>
#include <string>

#define USE_CONSTRUCTOR

class Sheet : public Visitor
{
    const Node* currentModule;
    std::string className;

    void print(const Node* node = 0)
    {
        printf("\"%s\",\"%s\",\"%s\"\n", currentModule->getName().c_str(), className.c_str(), node? node->getName().c_str() : "");
    }

public:
    Sheet() :
        currentModule(0)
    {
    }

    virtual void at(const Module* node)
    {
        const Node* prev = currentModule;
        currentModule = node;
        className = "";
        if (0 < node->getName().size())
        {
            // print();
        }
        visitChildren(node);
        currentModule = prev;
    }

    virtual void at(const ExceptDcl* node)
    {
        className = node->getName();
        print();
        visitChildren(node);
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() || (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

#ifdef USE_CONSTRUCTOR
        if (node->getAttr() & Interface::Constructor)
        {
            className = node->getParent()->getName() + node->getCtorScope() + node->getName();
        }
        else
        {
            className = node->getName();
        }
#else
        className = node->getName();
#endif

        // Expand supplementals
        bool hasMember = false;
        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                if (!dynamic_cast<Interface*>(*j))
                {
                    hasMember = true;
                    (*j)->accept(this);
                }
            }
        }
        if (!hasMember)
        {
            print();
        }

#ifdef USE_CONSTRUCTOR
        if (Interface* constructor = node->getConstructor())
        {
            constructor->accept(this);
        }
#endif
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef(node->getParent()))
        {
        }
        else
        {
            // This node is an exception class member.
            print(node);
        }
    }

    virtual void at(const ConstDcl* node)
    {
        print(node);
    }

    virtual void at(const Attribute* node)
    {
        print(node);
    }

    virtual void at(const OpDcl* node)
    {
        print(node);
    }
};

#endif  // ESIDL_SHEET_H_INCLUDED
