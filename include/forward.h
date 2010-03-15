/*
 * Copyright 2009, 2010 Google Inc.
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

#ifndef ESIDL_FORWARD_H_INCLUDED
#define ESIDL_FORWARD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "esidl.h"
#include "expr.h"

// Generate the forward declarations automatically for C++ and other languages.
class Forward : public Visitor
{
    const char* source;
    const Node* currentNode;
    NodeList    declared;     // List of declared names.
    NodeList    notDeclared;  // List of names that need forward declarations.

    void addDeclared(const Node* node)
    {
        Node* resolved = resolve(currentNode, node->getName());
        if (resolved)
        {
            declared.push_back(resolved);
        }
        // TODO: show an error message
    }

    bool isDeclared(const Node* node)
    {
        Node* resolved = resolve(currentNode, node->getName());
        if (resolved)
        {
            for (NodeList::iterator i = declared.begin();
                 i != declared.end();
                 ++i)
            {
                if (*i == resolved)
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            // TODO: show an error message
            return true;
        }
    }

    void addNotDeclared(const Node* node)
    {
        Node* resolved = resolve(currentNode, node->getName());
        Member* m = dynamic_cast<Member*>(resolved);
        if (m && m->isTypedef(m->getParent()))
        {
            m->getSpec()->accept(this);
        }
        if (resolved)
        {
            notDeclared.push_back(resolved);
            declared.push_back(resolved);
        }
        // TODO: show an error message
    }

public:
    Forward(const char* source) :
        source(source),
        currentNode(getSpecification())
    {
    }

    ~Forward()
    {
    }

    virtual void at(const Node* node)
    {
        if (0 < node->getName().size())
        {
            if (node->isInterface(currentNode))
            {
                if (!isDeclared(node))
                {
                    addNotDeclared(node);
                }
            }
        }
        else
        {
            const Node* saved = currentNode;
            visitChildren(node);
            currentNode = saved;
        }
    }

    virtual void at(const Module* node)
    {
        visitChildren(node);
    }

    virtual void at(const Type* node)
    {
    }

    void getter(const Attribute* node)
    {
        node->getSpec()->accept(this);
        visitChildren(node->getGetRaises());
    }

    bool setter(const Attribute* node)
    {
        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return false;
        }

        Node* spec = node->getSpec();
        if (node->isPutForwards())
        {
            Interface* target = dynamic_cast<Interface*>(dynamic_cast<ScopedName*>(spec)->search(node->getParent()));
            assert(target);
            Attribute* forwards = dynamic_cast<Attribute*>(target->search(node->getPutForwards()));
            assert(forwards);
            spec = forwards->getSpec();
        }
        spec->accept(this);
        visitChildren(node->getSetRaises());
        return true;
    }

    virtual void at(const Attribute* node)
    {
        getter(node);
        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            setter(node);
        }
    }

    virtual void at(const OpDcl* node)
    {
        node->getSpec()->accept(this);
        visitChildren(node);
        visitChildren(node->getRaises());
    }

    virtual void at(const ParamDcl* node)
    {
        node->getSpec()->accept(this);
    }

    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (element->isInterface(interface))  // Do not process Constructor
        {
            return;
        }
        element->accept(this);
    }

    virtual void at(const Interface* node)
    {
        if (node->getSource() != source)
        {
            return;
        }

        addDeclared(node);

        if (node->isLeaf())
        {
            return;
        }

        currentNode = node;

        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                visitInterfaceElement(*i, *j);
            }
        }

        if (Interface* constructor = node->getConstructor())
        {
            // Process constructors.
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(node, *i);
            }
        }
    }

    virtual void at(const ConstDcl* node)
    {
        node->getSpec()->accept(this);
        Type* type = node->getType();
        assert(type);
        if (type->getName() == "boolean")
        {
            EvalInteger<bool> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "octet")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "byte")
        {
            EvalInteger<int8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "unsigned byte")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "short")
        {
            EvalInteger<int16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "unsigned short")
        {
            EvalInteger<uint16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "long")
        {
            EvalInteger<int32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "unsigned long")
        {
            EvalInteger<uint32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "long long")
        {
            EvalInteger<int64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "unsigned long long")
        {
            EvalInteger<uint64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "float")
        {
            EvalFloat<float> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else if (type->getName() == "double")
        {
            EvalFloat<double> eval(node->getParent());
            node->getExp()->accept(&eval);
        }
        else
        {
            fprintf(stderr, "Inv. const type.\n");
            exit(EXIT_FAILURE);
        }
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef(node->getParent()))
        {
            if (node->getSource() != source)
            {
                return;
            }
            addDeclared(node);
        }
        node->getSpec()->accept(this);
    }

    void generateForwardDeclarations()
    {
        setBaseFilename(source);
        for (NodeList::reverse_iterator i = notDeclared.rbegin();
             i != notDeclared.rend();
             ++i)
        {
            Node* node = *i;
            Member* m = dynamic_cast<Member*>(node);
            if (m && m->isTypedef(node->getParent()))
            {
                Member* t = new Member(*m);
                node->getParent()->addFront(t);
            }
            else if (!(node->getAttr() & Interface::Supplemental))
            {
                node->getParent()->addFront(new Interface(node->getName().c_str(), 0, true));
            }
        }
        setBaseFilename("");
    }
};

#endif  // ESIDL_FORWARD_H_INCLUDED
