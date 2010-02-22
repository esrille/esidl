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

    int writeInvoke(const Node* node, Node* spec, SequenceType* seq, const Node* nameNode)
    {
        bool hasBuffer = false;
        bool hasLength = false;
        int hasCast = 0;

        writetab();
        if (seq)
        {
            write("return static_cast<int>(invoke(this");
            hasBuffer = true;
            if (!seq->getMax())
            {
                hasLength = true;
            }
            hasCast = 1;
        }
        else if (spec->isVoid(node))
        {
            write("invoke(this");
        }
        else if (spec->isAny(node))
        {
            write("return invoke(this");
            hasBuffer = hasLength = true;
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

        write(", I, %u", methodNumber);

        if (!hasCustomStringType() && spec->isString(node->getParent()))
        {
            hasBuffer = hasLength = true;
        }
        if (hasBuffer)
        {
            write(", %s", getBufferName(nameNode).c_str());
        }
        if (hasLength)
        {
            write(", %sLength", getBufferName(nameNode).c_str());
        }

        ++methodNumber;

        return hasCast;
    }

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
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));
        std::string name = getBufferName(node);

        writeln("case %u:", methodNumber);
        indent();
            writetab();
            write("return ");
            write("get%s(", cap.c_str());

            // TODO: seq, any, etc.
            if (seq)
            {
                write("0");
                if (!seq->getMax())
                {
                    write(", 0");
                }
            }
            else if (!hasCustomStringType() && spec->isString(node->getParent()))
            {
                write("0, 0");
            }
            else if (spec->isAny(node->getParent()))
            {
                write("0, 0");
            }

            write(");\n");
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
        seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

        writeln("case %u:", methodNumber);
        indent();
            writetab();
            if (seq)
            {
                write("return ");
            }
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
            if (seq && !seq->getMax())
            {
                write(", arguments.getLength(0)");  // TODO: Any for seq should provide length info.
            }
            write(");\n");
            if (!seq)
            {
                writeln("return Any();");
            }
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

            bool needComma = false;
            Node* spec = node->getSpec();
            SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

            // TODO: seq, any, etc.
            if (seq)
            {
                write("0");
                if (!seq->getMax())
                {
                    write(", 0");
                }
                needComma = true;
            }
            else if (!hasCustomStringType() && spec->isString(node->getParent()))
            {
                write("0, 0");
                needComma = true;
            }
            else if (spec->isAny(node->getParent()))
            {
                write("0, 0");
                needComma = true;
            }

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
                if (needComma || i != node->begin())
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
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));
        if (node->isVariadic())
        {
            variadicParam = node;
            variadicSequence.setSpec(spec);
            seq = &variadicSequence;
        }

#if 0
        if (seq && !seq->getSpec()->isInterface(currentNode) ||
            spec->isString(node->getParent()))
        {
            write("const ");
        }
#endif

        bool hasLength = false;

        if (node->isVariadic())
        {
            spec = seq;  // TODO: Check
            hasLength = true;
        }
        else if (seq)
        {
            if (!seq->getMax())
            {
                hasLength = true;
            }
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

        if (hasLength)
        {
            write(", arguments.getLength(%u)", getParamCount());  // TODO: Any for seq should provide length info.
        }
     }
 };

#endif  // ESIDL_CPLUSPLUS_CALL_H_INCLUDED
