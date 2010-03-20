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

#ifndef ESIDL_CPLUSPLUS_META_H_INCLUDED
#define ESIDL_CPLUSPLUS_META_H_INCLUDED

#include <map>
#include <string>
#include "esidl.h"
#include "expr.h"
#include "cplusplus.h"
#include "reflect.h"

// Generate the string-encoded interface information for reflection
class CPlusPlusMeta : public CPlusPlus
{
    unsigned offset;
    std::map<std::string, unsigned> symbolTable;
    std::map<std::string, unsigned> constantTable;

    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (element->isSequence(interface) ||
            element->isNative(interface) ||
            element->isTypedef(interface) ||
            element->isInterface(interface))  // Do not process Constructor
        {
            return;
        }
        writeln("");
        writetab();
        write("/* %u */ ", offset);
        element->accept(this);
    }

    void addSymbol(std::string symbol, unsigned offset)
    {
        symbolTable.insert(std::pair<std::string, unsigned>(symbol, offset));
    }

    void addConstant(std::string symbol, unsigned offset)
    {
        constantTable.insert(std::pair<std::string, unsigned>(symbol, offset));
    }

public:
    CPlusPlusMeta(const char* source, Formatter* formatter) :
        CPlusPlus(source, formatter, "std::string", "Object", true),
        offset(0)
    {
        formatter->flush();
        currentNode = 0;
    }

    virtual void at(const Module* node)
    {
        // CPlusPlusMeta{} visiter should be applied for interfaces.
    }

    virtual void at(const Attribute* node)
    {
        addSymbol(node->getName(), offset);
        write("\"%s\"", node->getMetaGetter().c_str());
        offset += node->getMetaGetter().length();
        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            writeln("");
            writetab();
            write("/* %u */ ", offset);
            addSymbol(node->getName(), offset);
            write("\"%s\"", node->getMetaSetter().c_str());
            offset += node->getMetaSetter().length();
        }
    }

    virtual void at(const OpDcl* node)
    {
        for (int i = 0; i < node->getMethodCount(); ++i)
        {
            if (i != 0)
            {
                writeln("");
                writetab();
                write("/* %u */ ", offset);
            }
            if (constructorMode)
            {
                write("\"%c%s\"", Reflect::kConstructor, node->getMetaOp(i).c_str() + 1);
            }
            else
            {
                addSymbol(node->getName(), offset);
                write("\"%c%s\"", Reflect::kOperation, node->getMetaOp(i).c_str() + 1);
            }
            offset += node->getMetaOp(i).length();
        }
    }

    virtual void at(const Interface* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        currentNode = node;
        offset = 0;

        writeln("static const char* const getMetaData() {");
            writetab();
            write("return");
            indent();

            writeln("");
            writetab();
            write("/* %u */ ", offset);
            write("\"%s\"", node->getMeta().c_str());

            offset += node->getMeta().length();

            // Expand supplementals
            std::list<const Interface*> interfaceList;
            node->collectSupplementals(&interfaceList);
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
            write(";\n");

            unindent();
        writeln("}");


        writeln("static const Reflect::SymbolData* const getSymbolTable() {");
            writeln("static const Reflect::SymbolData symbolTable[] = {");
                for (std::map<std::string, unsigned>::iterator i = symbolTable.begin();
                    i != symbolTable.end();
                    ++i)
                {
                    writeln("{ \"%s\", %u },", (*i).first.c_str(), (*i).second);
                }
                for (std::map<std::string, unsigned>::iterator i = constantTable.begin();
                    i != constantTable.end();
                    ++i)
                {
                    writeln("{ \"%s\", %u },", (*i).first.c_str(), (*i).second);
                }
                writeln("{ 0, 0 },");
            writeln("};");
            writeln("return symbolTable;");
        writeln("}");

        flush();
    }

    virtual void at(const ConstDcl* node)
    {
        addConstant(node->getName(), offset);
        write("\"%s\"", node->getMeta().c_str());
        offset += node->getMeta().length();
    }

    virtual void at(const Member* node)
    {
        assert(node->isTypedef(node->getParent()));
        node->getSpec()->accept(this);
    }
};

#endif  // ESIDL_CPLUSPLUS_META_H_INCLUDED
