/*
 * Copyright 2010, 2011 Esrille Inc.
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

#ifndef ESIDL_MESSENGER_H_INCLUDED
#define ESIDL_MESSENGER_H_INCLUDED

#define USE_CONSTRUCTOR

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "esidl.h"
#include "formatter.h"

class Messenger : public Visitor, public Formatter
{
public:
    static bool useVirtualBase;

protected:
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;

    bool constructorMode;
    int optionalStage;
    int optionalCount;

    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

    std::string prefixedModuleName;
    std::string targetModuleName;

    bool noDefaultArgument;

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
        return getEscapedName(name);
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
    Messenger(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        Formatter(file, indent),
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        constructorMode(false),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0),
        noDefaultArgument(false)
    {
        targetModuleName = prefixedModuleName = currentNode->getPrefixedModuleName();
    }

    Messenger(const Formatter* formatter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        Formatter(formatter),
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        constructorMode(false),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0),
        noDefaultArgument(false)
    {
        targetModuleName = prefixedModuleName = currentNode->getPrefixedModuleName();
    }

    virtual void at(const Node* node)
    {
        if (0 < node->getName().size())
        {
            write("%s", getEscapedName(node->getName()).c_str());
        }
        else
        {
            printChildren(node);
        }
    }

    virtual void at(const ScopedName* node)
    {
        Node* resolved = node->searchCplusplus(currentNode);
        node->check(resolved, "%s could not resolved.", node->getName().c_str());
        const Node* saved = currentNode;
        if (resolved->getParent())
        {
            currentNode = resolved->getParent();
        }
        if (!dynamic_cast<Interface*>(resolved) && !dynamic_cast<ExceptDcl*>(resolved) && !resolved->isTypedef(resolved->getParent()))
        {
            resolved->accept(this);
        }
        else
        {
            std::string name = resolved->getPrefixedName();
            name = getInterfaceName(name);
            name = getScopedName(targetModuleName, name);
            write("%s", name.c_str());
        }
        currentNode = saved;
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
            write("Nullable<");
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
        else if (node->getName() == "Date")
        {
            write("unsigned long long");
        }
        else
        {
            write("%s", node->getName().c_str());
        }
        if (node->getAttr() & Node::Nullable)
        {
            write(">");
        }
    }

    virtual void at(const SequenceType* node)
    {
        Node* spec = node->getSpec();
        write("Sequence<");
        spec->accept(this);
        write(">");
    }

    virtual void at(const VariadicType* node)
    {
        Node* spec = node->getSpec();
        write("Variadic<");
        spec->accept(this);
        write(">");
    }

    virtual void at(const ArrayType* node)
    {
        Node* spec = node->getSpec();
        Type* type = dynamic_cast<Type*>(spec);
        // Note we don't need separate array types for primitive types in C++.
        std::string name = getScopedName(targetModuleName, std::string(Node::getDefaultPrefix()) + "::ObjectArray");
        write("%s<", name.c_str());
        spec->accept(this);
        write(">");
    }

    void getter(const Attribute* node, const std::string& className = "")
    {
        static Type replaceable("any");
        std::string cap = node->getName().c_str();
        cap[0] = toupper(cap[0]);
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        std::string name = getBufferName(node);

        spec->accept(this);
        write(" ");
        if (!className.empty())
            write("%s::", className.c_str());
        write("get%s()", cap.c_str());
        if (useExceptions && node->getGetRaises())
        {
            write(" throw(");
            printChildren(node->getGetRaises(), ", ");
            write(")");
        }
    }

    bool setter(const Attribute* node, const std::string& className = "")
    {
        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return false;
        }

        static Type replaceable("any");
        std::string cap = node->getName().c_str();
        cap[0] = toupper(cap[0]);
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
        write("void ");
        if (!className.empty())
            write("%s::", className.c_str());
        write("set%s(", cap.c_str());
        spec->accept(this);
#if 0
        if (spec->isInterface(node->getParent()))
        {
            write("&");
        }
#endif
        write(" %s)", name.c_str());
        if (useExceptions && node->getSetRaises())
        {
            write(" throw(");
            printChildren(node->getSetRaises(), ", ");
            write(")");
        }
        return true;
    }

    int getParamCount(const OpDcl* node)
    {
        optionalCount = 0;
        variadicParam = 0;
        int count = 0;
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
            ++count;
            if (param->isVariadic())
            {
                variadicParam = param;
            }
        }
        optionalCount = 0;
        return count;
    }

    void writeParameters(const OpDcl* node, bool needComma = false)
    {
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
    }

    virtual void at(const OpDcl* node, const std::string& className = "")
    {
        if (constructorMode)
            {
            if (!className.empty())
                write("%s::", className.c_str());
            write("%s(", getEscapedName(node->getParent()->getParent()->getName()).c_str());
            writeParameters(node);
            write(")");
            return;
        }

        Node* spec = node->getSpec();
        spec->accept(this);
        write(" ");
        if (!className.empty())
            write("%s::", className.c_str());
        write("%s(", getEscapedName(node->getName()).c_str());
        writeParameters(node);
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
        static VariadicType variadic(0);

        Node* spec = node->getSpec();
        if (node->isVariadic())
        {
            variadicParam = node;
            variadic.setSpec(spec);
            spec = &variadic;
        }

        spec->accept(this);
#if 0
        if (spec->isInterface(node->getParent()))
        {
            write("&");
        }
#endif
        write(" %s", getEscapedName(node->getName()).c_str());

        // Default argument for the variadic parameter
        if (!noDefaultArgument && node->isVariadic())
        {
            write(" = ");
            spec->accept(this);
            write("()");
        }
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

    static std::string getIncludedName(const std::string headerFilename, const std::string& indent = "es")
    {
        std::string included(headerFilename);
        bool capitalize = true;

        if (indent == "google")
        {
            included += "_";
        }
        else if (indent == "es")
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

    std::string getClassName(const Node* node)
    {
        if (node->isBaseObject())
        {
            return objectTypeName;
        }
        std::string className = node->getName();
#ifdef USE_CONSTRUCTOR
        if (node->getAttr() & Node::Constructor)
        {
            className = node->getParent()->getName() + node->getCtorScope() + className;
        }
#endif
        return className;
    }

    bool getConstructorMode() const {
        return constructorMode;
    }
    void setConstructorMode(bool value) {
        constructorMode = value;
    }

    static std::string getEscapedName(std::string name);
};

#endif  // ESIDL_MESSENGER_H_INCLUDED
