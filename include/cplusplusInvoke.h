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

#ifndef ESIDL_CPLUSPLUS_INVOKE_H_INCLUDED
#define ESIDL_CPLUSPLUS_INVOKE_H_INCLUDED

#include <algorithm>
#include <set>
#include <vector>
#include "cplusplus.h"

class CPlusPlusInvoke : public CPlusPlus
{
    unsigned methodNumber;
    std::string className;
    unsigned offset;

    int writeInvoke(const Node* node, Node* spec, const Node* nameNode)
    {
        int hasCast = 0;

        writetab();
        if (spec->isVoid(node))
        {
            write("invoke(_object");
        }
        else if (spec->isAny(node))
        {
            write("return invoke(_object");
        }
        else if (spec->isInterface(node))
        {
            write("return interface_cast<");
            spec->accept(this);
            write("*>(static_cast<Object*>(invoke(_object");
            hasCast = 2;
        }
        else
        {
            write("return static_cast<");
            spec->accept(this);
            write(" >(invoke(_object");
            hasCast = 1;
        }

        write(", interfaceNumber, methodNumber, getMetaData(), %u", offset);

        ++methodNumber;

        return hasCast;
    }

    void writeTemplateSpecification()
    {
        writeln("template<class ARGUMENT, Any (*invoke)(Object*, unsigned, unsigned, const char*, unsigned, unsigned, ARGUMENT*)>");
    }

public:
    CPlusPlusInvoke(const Formatter* formattter) :
        CPlusPlus(formattter, "std::string", "Object", true)
    {
        currentNode = 0;
    }

    virtual void at(const Member* node)
    {
    }

    virtual void at(const ConstDcl* node)
    {
        offset += node->getMeta().length();
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
            qualifiedModuleName = currentNode->getQualifiedModuleName();
            className = node->getName();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

        methodNumber = 0;
        offset = node->getMeta().length();
        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            const Node* saved = currentNode;
            currentNode = *i;
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                visitInterfaceElement(*i, *j);
            }
            currentNode = saved;
        }
    }

    virtual void at(const Attribute* node)
    {
        // getter
        static Type replaceable("any");
        std::string cap = node->getName().c_str();
        cap[0] = toupper(cap[0]);
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        std::string name = getBufferName(node);

        writeTemplateSpecification();
        writetab();
        write("static ");
        spec->accept(this);
        if (spec->isInterface(node->getParent()))
        {
            write("*");
        }
        write(" get%s(Object* _object, unsigned interfaceNumber, unsigned methodNumber)", cap.c_str());
        writeln("{");
            int hasCast = writeInvoke(node, spec, node);
            write(", 0, 0");
            while (0 < hasCast--)
            {
                write(")");
            }
            write(");\n");
        writeln("}");
        offset += node->getMetaGetter().length();

        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return;
        }

        // setter
        if (node->isPutForwards())
        {
            Interface* target = dynamic_cast<Interface*>(dynamic_cast<ScopedName*>(spec)->search(node->getParent()));
            assert(target);
            Attribute* forwards = dynamic_cast<Attribute*>(target->search(node->getPutForwards()));
            assert(forwards);
            spec = forwards->getSpec();
        }

        writeTemplateSpecification();
        writetab();
        write("static ");
        write("void set%s(Object* _object, unsigned interfaceNumber, unsigned methodNumber, ", cap.c_str());
        spec->accept(this);
        if (spec->isInterface(node->getParent()))
        {
            write("*");
        }
        write(" %s)", name.c_str());
        writeln("{");
            writeln("ARGUMENT _argument = %s;", name.c_str());
            writeln("invoke(_object, interfaceNumber, methodNumber, getMetaData(), %u, 1, &_argument);", offset);
        writeln("}");
        offset += node->getMetaSetter().length();

        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writeTemplateSpecification();
        writetab();
        write("static ");
        Node* spec = node->getSpec();
        spec->accept(this);
        if (spec->isInterface(node->getParent()))
        {
            write("*");
        }
        write(" %s(", getEscapedName(node->getName()).c_str());
        write("Object* _object, unsigned interfaceNumber, unsigned methodNumber");
        writeParameters(node, true);
        write(")");
        writeln("{");

            // Invoke
            if (0 < getParamCount())
            {
                writeln("ARGUMENT _arguments[%u];", getParamCount());
                NodeList::iterator it = node->begin();
                for (int i = 0; i < getParamCount(); ++i, ++it)
                {
                    ParamDcl* param = static_cast<ParamDcl*>(*it);
                    writeln("_arguments[%d] = %s;", i, getEscapedName(param->getName()).c_str());
                }
            }
            int hasCast = writeInvoke(node, spec, spec);
            write(", %u", getParamCount());
            write((0 < getParamCount()) ? ", _arguments" : ", 0");
            while (0 < hasCast--)
            {
                write(")");
            }
            write(");\n");

        writeln("}");
        offset += node->getMetaOp(optionalStage).length();
    }
};

#endif  // ESIDL_CPLUSPLUS_INVOKE_H_INCLUDED
