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

#ifndef ESIDL_CPLUSPLUS_TEMPLATE_H_INCLUDED
#define ESIDL_CPLUSPLUS_TEMPLATE_H_INCLUDED

#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <set>
#include <vector>
#include "cplusplus.h"
#include "info.h"

class CPlusPlusTemplate : public CPlusPlus
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
            write("invoke(this");
        }
        else if (spec->isAny(node))
        {
            write("return invoke(this");
        }
        else if (spec->isInterface(node))
        {
            write("return dynamic_cast<");
            spec->accept(this);
            write("*>(static_cast<Object*>(invoke(this");
            hasCast = 2;
        }
        else
        {
            write("return static_cast<");
            spec->accept(this);
            write(" >(invoke(this");
            hasCast = 1;
        }

        write(", I, %u, %s::getMetaData(), %u", methodNumber, className.c_str(), offset);

        ++methodNumber;

        return hasCast;
    }

public:
    CPlusPlusTemplate(const char* source, FILE* file, const char* indent = "es") :
        CPlusPlus(source, file, "std::string", "Object", true, indent)
    {
        currentNode = 0;
    }

    CPlusPlusTemplate(const char* source, const Formatter* formattter) :
        CPlusPlus(source, formattter, "std::string", "Object", true)
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

        const std::list<const Interface*>* implementList = node->getImplements();

        // Mixin interface
        if (!implementList->empty())
        {
            write("class %s_Mixin : public %s",
                  getEscapedName(node->getName()).c_str(),
                  getEscapedName(node->getName()).c_str());
            for (std::list<const Interface*>::const_iterator i = implementList->begin();
                i != implementList->end();
                ++i)
            {
                std::string name = (*i)->getQualifiedName();
                name = getInterfaceName(name);
                name = getScopedName(qualifiedModuleName, name);
                write(", public %s", name.c_str());
            }
            writeln("{");
            writeln("};");
            writeln("");
        }

        // Bridge template
        writeln("template<class ARGUMENT, Any (*invoke)(Object*, unsigned, unsigned, const char*, unsigned, unsigned, ARGUMENT*), unsigned I = 0, class B = %s%s>",
                getEscapedName(node->getName()).c_str(),
                implementList->empty() ? "" : "_Mixin");
        write("class %s_Bridge : ", getEscapedName(node->getName()).c_str());

        std::list<const Interface*> bridgeList;
        for (std::list<const Interface*>::const_reverse_iterator i = implementList->rbegin();
             i != implementList->rend();
             ++i)
        {
            bridgeList.push_front(*i);
        }
        for (std::list<const Interface*>::const_iterator i = node->getSuperList()->begin();
             i != node->getSuperList()->end();
             ++i)
        {
            bridgeList.push_front(*i);
        }
        bridgeList.push_front(node);

        int baseCount = 0;
        int interfaceCount = 1;

        write("public ");
        for (std::list<const Interface*>::const_iterator i = bridgeList.begin();
             i != bridgeList.end();
             ++i)
        {
            if (*i == node || ((*i)->getAttr() & Interface::Supplemental) || (*i)->isBaseObject())
            {
                continue;
            }
            std::string name = (*i)->getQualifiedName();
            name = getInterfaceName(name);
            name = getScopedName(qualifiedModuleName, name);
            ++baseCount;
            write("%s_Bridge<ARGUMENT, invoke, I + %u, ", name.c_str(), interfaceCount);
            interfaceCount += (*i)->getInterfaceCount();
        }
        write("B");
        for (int i = 0; i < baseCount; ++i)
        {
            write(" >");
        }

        write(" {\n");
        unindent();
        writeln("public:");
        indent();

        methodNumber = 0;
        offset = node->getMeta().length();
        std::list<const Interface*> interfaceList;
        node->collectMixins(&interfaceList, node);
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

        writeln("};");
    }

    virtual void at(const Attribute* node)
    {
        // getter
        static Type replaceable("any");
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));
        std::string name = getBufferName(node);

        writetab();
        CPlusPlus::getter(node);
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
        seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

        writetab();
        CPlusPlus::setter(node);
        writeln("{");
            writeln("ARGUMENT _argument = %s;", name.c_str());
            writeln("invoke(this, I, %u, %s::getMetaData(), %u, 1, &_argument);",
                    methodNumber, className.c_str(), offset);
        writeln("}");
        offset += node->getMetaSetter().length();

        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writetab();
        CPlusPlus::at(node);
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
            Node* spec = node->getSpec();
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

#endif  // ESIDL_CPLUSPLUS_TEMPLATE_H_INCLUDED
