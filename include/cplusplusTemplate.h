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

#include <algorithm>
#include <set>
#include <vector>
#include "cplusplus.h"

class CPlusPlusTemplate : public CPlusPlus
{
    unsigned methodNumber;
    std::string className;
    unsigned offset;

    void writeInvoke(const Node* node, Node* spec)
    {
        writetab();
        if (!spec->isVoid(node))
        {
            write("return ");
        }
    }

public:
    CPlusPlusTemplate(FILE* file, const char* indent = "es") :
        CPlusPlus(file, "std::string", "Object", true, indent)
    {
        currentNode = 0;
    }

    CPlusPlusTemplate(const Formatter* formattter) :
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
            prefixedModuleName = currentNode->getPrefixedModuleName();
            className = getClassName(node);
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

        std::list<const Interface*> mixinList;
        node->collectMixins(&mixinList);

        // Mixin interface
        if (!mixinList.empty())
        {
            write("class %s_Mixin : public %s",
                  getEscapedName(node->getName()).c_str(),
                  getEscapedName(node->getName()).c_str());
            for (std::list<const Interface*>::const_iterator i = mixinList.begin();
                 i != mixinList.end();
                 ++i)
            {
                std::string name = (*i)->getPrefixedName();
                name = getInterfaceName(name);
                name = getScopedName(prefixedModuleName, name);
                write(", public %s", name.c_str());
            }
            writeln("{");
            unindent();
            writeln("public:");
            indent();

                writeln("virtual void* queryInterface(const char* qualifiedName) {");
                    writeln("if (void* object = %s::queryInterface(qualifiedName)) {",
                            getEscapedName(node->getName()).c_str());
                        writeln("return object;");
                    writeln("}");
                    for (std::list<const Interface*>::const_iterator i = mixinList.begin();
                        i != mixinList.end();
                        ++i)
                    {
                        std::string name = (*i)->getPrefixedName();
                        name = getInterfaceName(name);
                        name = getScopedName(prefixedModuleName, name);
                        writeln("if (void* object = %s::queryInterface(qualifiedName)) {", name.c_str());
                            writeln("return object;");
                        writeln("}");
                    }
                    writeln("return 0;");
                writeln("}");
                writeln("virtual const char* getMetaData(unsigned interfaceNumber) const {");
                    writeln("return %s::getMetaData(interfaceNumber);", getEscapedName(node->getName()).c_str());
                writeln("}");
                writeln("virtual const Reflect::SymbolData* const getSymbolTable(unsigned interfaceNumber) const {");
                    writeln("return %s::getSymbolTable(interfaceNumber);", getEscapedName(node->getName()).c_str());
                writeln("}");
                writeln("virtual Any call(unsigned interfaceNumber, unsigned methodNumber, unsigned argumentCount, Any* arguments) {");
                    writeln("return %s::call(interfaceNumber, methodNumber, argumentCount, arguments);", getEscapedName(node->getName()).c_str());
                writeln("}");

            writeln("};");
            writeln("");
        }

        // Bridge template
        writeln("template<class ARGUMENT, Any (*invoke)(Object*, unsigned, unsigned, const char*, unsigned, unsigned, ARGUMENT*), unsigned I = 0, class B = %s%s>",
                getEscapedName(getClassName(node)).c_str(),
                mixinList.empty() ? "" : "_Mixin");
        write("class %s_Bridge : ", getEscapedName(getClassName(node)).c_str());

        std::list<const Interface*> bridgeList;
        for (std::list<const Interface*>::const_reverse_iterator i = mixinList.rbegin();
             i != mixinList.rend();
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
            std::string name = (*i)->getPrefixedName();
            name = getInterfaceName(name);
            name = getScopedName(prefixedModuleName, name);
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

        writeln("};");
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

        writetab();
        CPlusPlus::getter(node);
        writeln("{");
            writeInvoke(node, spec);
            write("%s::get%s<ARGUMENT, invoke>(this, I, %u",
                  className.c_str(),
                  cap.c_str(),
                  methodNumber);
            ++methodNumber;
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

        writetab();
        CPlusPlus::setter(node);
        writeln("{");
            writeln("%s::set%s<ARGUMENT, invoke>(this, I, %u, %s);",
                    className.c_str(),
                    cap.c_str(),
                    methodNumber,
                    name.c_str());
            ++methodNumber;
        writeln("}");
        offset += node->getMetaSetter().length();

        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        writetab();
        CPlusPlus::at(node);
        writeln("{");

            // Invoke
            writeInvoke(node, node->getSpec());
            write("%s::%s<ARGUMENT, invoke>(this, I, %u",
                  className.c_str(),
                  getEscapedName(node->getName()).c_str(),
                  methodNumber);
            ++methodNumber;

            if (0 < getParamCount())
            {
                NodeList::iterator it = node->begin();
                for (int i = 0; i < getParamCount(); ++i, ++it)
                {
                    ParamDcl* param = static_cast<ParamDcl*>(*it);
                    write(", %s", getEscapedName(param->getName()).c_str());
                }
            }
            write(");\n");

        writeln("}");
        offset += node->getMetaOp(optionalStage).length();
    }
};

#endif  // ESIDL_CPLUSPLUS_TEMPLATE_H_INCLUDED
