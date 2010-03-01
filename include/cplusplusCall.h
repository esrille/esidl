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

#ifndef ESIDL_CPLUSPLUS_CALL_H_INCLUDED
#define ESIDL_CPLUSPLUS_CALL_H_INCLUDED

#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <set>
#include <vector>
#include "cplusplus.h"
#include "info.h"

class CPlusPlusCall : public CPlusPlus
{
    unsigned methodNumber;

public:
    CPlusPlusCall(const char* source, const Formatter* formattter) :
        CPlusPlus(source, formattter, "std::string", "Object", true)
    {
        currentNode = 0;
    }

    virtual void at(const Member* node)
    {
        // To avoid constants
    }

    virtual void at(const Interface* node)
    {
        if (!currentNode)
        {
            currentNode = node->getParent();
            qualifiedModuleName = currentNode->getQualifiedModuleName();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

        writeln("template <class ARGUMENTS, void (*resolve)(Object*, const char*, ARGUMENTS&, unsigned*, unsigned*)>");
        writeln("Any call(const char* method, ARGUMENTS& arguments) {");
            writeln("unsigned interfaceNumber;");
            writeln("unsigned methodNumber;");
            writeln("resolve(this, method, arguments, &interfaceNumber, &methodNumber);");
            writeln("return call(interfaceNumber, methodNumber, arguments);");
        writeln("}");

        writeln("template <class ARGUMENTS>");
        writeln("Any call(unsigned interfaceNumber, unsigned methodNumber, ARGUMENTS& arguments) {");
            writeln("switch (interfaceNumber) {");
            unsigned interfaceNumber = 0;
            std::list<const Interface*> list;
            node->getInterfaceList(&list);
            for (std::list<const Interface*>::const_iterator i = list.begin();
                 i != list.end();
                 ++i, ++interfaceNumber)
            {
                unindent();
                writeln("case %u:", interfaceNumber);
                indent();
                writeln("return %s::call(methodNumber, arguments);", getScopedName(qualifiedModuleName, (*i)->getQualifiedName()).c_str());
            }
            writeln("}");
        writeln("}");

        writeln("template <class ARGUMENTS>");
        writeln("Any call(unsigned methodNumber, ARGUMENTS& arguments) {");
            writeln("switch (methodNumber) {");
            unindent();
            methodNumber = 0;
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
            writeln("default:");
            indent();
                writeln("return Any();");  // TODO:  should raise an exception?
            writeln("}");
        writeln("}");
    }

    virtual void at(const Attribute* node)
    {
        std::string cap = node->getName().c_str();
        cap[0] = toupper(cap[0]);   // XXX

        // getter
        static Type replaceable("any");
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        std::string name = getBufferName(node);

        writeln("case %u:", methodNumber);
        indent();
            writeln("return get%s();", cap.c_str());
        unindent();
        ++methodNumber;

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

        writeln("case %u:", methodNumber);
        indent();
            writetab();
            write("set%s(", cap.c_str());
            if (spec->isInterface(node->getParent()))
            {
                write("dynamic_cast<");
                spec->accept(this);
                write("*>(static_cast<Object*>(arguments[0]))");
            }
            else
            {
                write("static_cast<");
                spec->accept(this);
                write(" >(arguments[0])");
            }
            write(");\n");
            writeln("return Any();");

        unindent();
        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writeln("case %u:", methodNumber);
        indent();
            writetab();
            write("return %s(", getEscapedName(node->getName()).c_str());

            // TODO: Move this to CPlusPlus
            paramCount = 0;
            variadicParam = 0;
            for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
            {
                ParamDcl* param = dynamic_cast<ParamDcl*>(*i);
                assert(param);
                if (param->isOptional())
                {
                    ++optionalCount;
                    if (optionalStage < optionalCount)
                    {
                        break;
                    }
                }
                if (i != node->begin())
                {
                    write(", ");
                }
                param->accept(this);
                ++paramCount;
            }
            // End of the block

            write(");\n");
        unindent();
        ++methodNumber;
    }

    virtual void at(const ParamDcl* node)
    {
        static SequenceType variadicSequence(0);

        Node* spec = node->getSpec();

        if (node->isVariadic())
        {
            variadicParam = node;
            variadicSequence.setSpec(spec);
            spec = &variadicSequence;
        }

        if (spec->isInterface(node->getParent()))
        {
            write("dynamic_cast<");
            spec->accept(this);
            write("*>(static_cast<Object*>(arguments[%u]))", getParamCount());
        }
        else
        {
            write("static_cast<");
            spec->accept(this);
            write(" >(arguments[%u])", getParamCount());
        }
     }
 };

#endif  // ESIDL_CPLUSPLUS_CALL_H_INCLUDED
