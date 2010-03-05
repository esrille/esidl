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

#ifndef ESIDL_CPLUSPLUS_H_INCLUDED
#define ESIDL_CPLUSPLUS_H_INCLUDED

#define USE_CONSTRUCTOR
#define USE_VIRTUAL_BASE

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "esidl.h"
#include "formatter.h"

class CPlusPlus : public Visitor, public Formatter
{
protected:
    const char* source;

    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;

    bool constructorMode;
    int optionalStage;
    int optionalCount;

    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

    std::string qualifiedModuleName;

    int getParamCount() const
    {
        return paramCount;
    }

    const ParamDcl* getVariadic() const
    {
        return variadicParam;
    }

    void printChildren(const Node* node, const char* separater = 0)
    {
        if (node->isLeaf())
        {
            return;
        }

        const Node* saved = currentNode;
        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            if (!(*i)->isDefinedIn(source))
            {
                continue;
            }
            if ((*i)->isNative(node->getParent()))
            {
                continue;
            }
            if (separater && i != node->begin())
            {
                write("%s", separater);
            }
            if ((*i)->isInterface(currentNode) || (*i)->isModule(currentNode))
            {
                currentNode = (*i);
            }
            (*i)->accept(this);
        }
        currentNode = saved;
    }

    std::string getBufferName(const Node* spec)
    {
        std::string name = spec->getName();
        size_t pos = name.rfind("::");
        if (pos != std::string::npos)
        {
            name = name.substr(pos + 2);
        }
        name[0] = tolower(name[0]);
        return name;
    }

    bool hasCustomStringType() const
    {
        return stringTypeName != "char*";
    }

    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (dynamic_cast<Interface*>(element))
        {
            // Do not process Constructor.
            return;
        }

        optionalStage = 0;
        do
        {
            optionalCount = 0;
            element->accept(this);
            ++optionalStage;
        } while (optionalStage <= optionalCount);
    }

public:
    CPlusPlus(const char* source, FILE* file, const char* stringTypeName = "char*", const char* objectTypeName = "object",
              bool useExceptions = true, const char* indent = "es") :
        Formatter(file, indent),
        source(source),
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        constructorMode(false),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0)
    {
        qualifiedModuleName = currentNode->getQualifiedModuleName();
    }

    CPlusPlus(const char* source, const Formatter* formatter, const char* stringTypeName = "char*", const char* objectTypeName = "object",
              bool useExceptions = true) :
        Formatter(formatter),
        source(source),
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        constructorMode(false),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0)
    {
        qualifiedModuleName = currentNode->getQualifiedModuleName();
    }

    virtual void at(const Node* node)
    {
        if (0 < node->getName().size())
        {
            std::string name = node->getName();
            Node* resolved = resolve(currentNode, name);
            if (resolved)
            {
                name = resolved->getQualifiedName();
                name = getInterfaceName(name);
                name = getScopedName(qualifiedModuleName, name);
            }
            write("%s", name.c_str());
        }
        else
        {
            printChildren(node);
        }
    }

    virtual void at(const Module* node)
    {
        if (0 < node->getName().size())
        {
            if (node->getJavadoc().size())
            {
                write("%s\n", node->getJavadoc().c_str());
                writetab();
            }
            write("namespace %s {\n", node->getName().c_str());
                printChildren(node);
            writeln("}");
        }
        else
        {
            printChildren(node);
        }
    }

    virtual void at(const Type* node)
    {
        if (node->getAttr() & Node::Nullable)
        {
            write ("Nullable<");
        }
        if (node->getName() == "boolean")
        {
            write("bool");
        }
        else if (node->getName() == "octet")
        {
            write("unsigned char");
        }
        else if (node->getName() == "byte")
        {
            write("signed char");
        }
        else if (node->getName() == "unsigned byte")
        {
            write("unsigned char");
        }
        else if (node->getName() == "long")
        {
            write("int");
        }
        else if (node->getName() == "unsigned long")
        {
            write("unsigned int");
        }
        else if (node->getName() == "any")
        {
            write("Any");
        }
        else if (node->getName() == "string")
        {
            write(stringTypeName.c_str());
        }
        else
        {
            write("%s", node->getName().c_str());
        }
        if (node->getAttr() & Node::Nullable)
        {
            write (">");
        }
    }

    virtual void at(const SequenceType* node)
    {
        Node* spec = node->getSpec();
        write("Sequence<");
        spec->accept(this);
        if (spec->isInterface(currentNode))
        {
            write("*");
        }
        write (">");
    }

    virtual void at(const ArrayType* node)
    {
        Node* spec = node->getSpec();
        Type* type = dynamic_cast<Type*>(spec);
        // Note we don't need separate array types for primitive types in C++.
        std::string name = getScopedName(qualifiedModuleName, "::dom::ObjectArray");
        write("%s<", name.c_str());
        spec->accept(this);
        write(">");
    }

    void getter(const Attribute* node)
    {
        static Type replaceable("any");
        std::string cap = node->getName().c_str();
        cap[0] = toupper(cap[0]);   // XXX
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        std::string name = getBufferName(node);

        write("virtual ");
        if (!hasCustomStringType() && spec->isString(node->getParent()))
        {
            write("const ", cap.c_str());
            spec->accept(this);
            write(" get%s(void* %s, int %sLength)", cap.c_str(), name.c_str(), name.c_str());
        }
        else
        {
            if (spec->isInterface(node->getParent()))
            {
                spec->accept(this);
                write("*");
            }
            else if (NativeType* nativeType = spec->isNative(node->getParent()))
            {
                nativeType->accept(this);
            }
            else
            {
                spec->accept(this);
            }
            write(" get%s()", cap.c_str());
        }
        if (useExceptions && node->getGetRaises())
        {
            write(" throw(");
            printChildren(node->getGetRaises(), ", ");
            write(")");
        }
    }

    bool setter(const Attribute* node)
    {
        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return false;
        }

        static Type replaceable("any");
        std::string cap = node->getName().c_str();
        cap[0] = toupper(cap[0]);   // XXX
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        else if (node->isPutForwards())
        {
            Interface* target = dynamic_cast<Interface*>(dynamic_cast<ScopedName*>(spec)->search(node->getParent()));
            assert(target);
            Attribute* forwards = dynamic_cast<Attribute*>(target->search(node->getPutForwards()));
            assert(forwards);
            spec = forwards->getSpec();
        }
        std::string name = getBufferName(node);

        // setter
        write("virtual ");
        if (spec->isString(node->getParent()))
        {
            write("void set%s(const ", cap.c_str());
            spec->accept(this);
            write(" %s)", name.c_str());
        }
        else
        {
            write("void set%s(", cap.c_str());
            spec->accept(this);
            if (spec->isInterface(node->getParent()))
            {
                write("*");
            }
            write(" %s)", name.c_str());
        }
        if (useExceptions && node->getSetRaises())
        {
            write(" throw(");
            printChildren(node->getSetRaises(), ", ");
            write(")");
        }
        return true;
    }

    virtual void at(const OpDcl* node)
    {
        if (!constructorMode)
        {
            write("virtual ");
        }
        else
        {
            write("static ");
        }

        bool needComma = true;  // true to write "," before the 1st parameter
        Node* spec = node->getSpec();
        if (!hasCustomStringType() && spec->isString(node->getParent()))
        {
            std::string name = getBufferName(spec);

            write("const ");
            spec->accept(this);
            write(" %s(", node->getName().c_str());
            write("void* %s, int %sLength", name.c_str(), name.c_str());
        }
        else
        {
            spec->accept(this);
            if (spec->isInterface(node->getParent()))
            {
                write("*");
            }
            write(" %s(", node->getName().c_str());
            needComma = false;
        }

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
            ++paramCount;
            param->accept(this);
        }

        write(")");
        if (useExceptions && node->getRaises())
        {
            write(" throw(");
            printChildren(node->getRaises(), ", ");
            write(")");
        }
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

        spec->accept(this);
        if (spec->isInterface(node->getParent()))
        {
            write("*");
        }
        write(" %s", getEscapedName(node->getName()).c_str());
    }

    virtual void at(const Include* node)
    {
    }

    virtual void at(const NativeType* node)
    {
        if (node->getName() == "void_pointer")
        {
            write("void*");
        }
        else
        {
            write("%s", node->getName().c_str());
        }
    }

    std::string getInterfaceName(std::string qualifiedName)
    {
        if (qualifiedName == Node::getBaseObjectName())
        {
            qualifiedName.replace(2, qualifiedName.length(), objectTypeName);
        }
        return qualifiedName;
    }

    static std::string getIncludedName(const std::string headerFilename, const char* indent = "es")
    {
        std::string included(headerFilename);
        bool capitalize = true;

        if (strcmp(indent, "google") == 0)
        {
            included += "_";
        }
        else if (strcmp(indent, "es") == 0)
        {
            included += "_INCLUDED";
        }
        else
        {
            capitalize = false;
        }
        if (isdigit(included[0]))
        {
            included = "No" + included;
        }
        for (int i = 0; i < included.size(); ++i)
        {
            char c = included[i];
            included[i] = isalnum(c) ? (capitalize ? toupper(c) : c) : '_';
        }
        return included;
    }

    static std::string getPackageName(std::string prefixedName)
    {
        size_t pos = 0;
        for (;;)
        {
            pos = prefixedName.find("::", pos);
            if (pos == std::string::npos)
            {
                break;
            }
            prefixedName.replace(pos, 2, ".");
        }
        if (prefixedName[0] == '.')
        {
            prefixedName = prefixedName.substr(1);
        }
        return prefixedName;
    }

    static std::string getEscapedName(std::string name);
};

#endif  // ESIDL_CPLUSPLUS_H_INCLUDED
