/*
 * Copyright 2008, 2009 Google Inc.
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

#ifndef ESIDL_JAVA_H_INCLUDED
#define ESIDL_JAVA_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "esidl.h"
#include "formatter.h"

class Java : public Visitor, public Formatter
{
protected:
    const char* source;

    std::string objectTypeName;
    bool useExceptions;

    bool constructorMode;
    int optionalStage;
    int optionalCount;

    std::string moduleName;
    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

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
            currentNode = (*i);
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

public:
    Java(const char* source, FILE* file, const char* indent = "es") :
        Formatter(file, indent),
        source(source),
        objectTypeName("Object"),
        useExceptions(true),
        constructorMode(false),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0)
    {
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
                name = getScopedName(moduleName, name);
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
                moduleName += "::";
                moduleName += node->getName();
                printChildren(node);
                moduleName.erase(moduleName.size() - node->getName().size() - 2);
            writeln("}");
        }
        else
        {
            printChildren(node);
        }
    }

    virtual void at(const Type* node)
    {
        bool nullable = (node->getAttr() & Node::Nullable) ? true : false;
        if (node->getName() == "boolean")
        {
            write(nullable ? "Boolean" : "boolean");
        }
        else if (node->getName() == "octet")
        {
            write(nullable ? "Byte" : "byte");
        }
        else if (node->getName() == "short" || node->getName() == "unsigned short")
        {
            write(nullable ? "Short" : "short");
        }
        else if (node->getName() == "long" || node->getName() == "unsigned long")
        {
            write(nullable ? "Integer" : "int");
        }
        else if (node->getName() == "long long" || node->getName() == "unsigned long long")
        {
            write(nullable ? "Long" : "long");
        }
        else if (node->getName() == "float")
        {
            write(nullable ? "Float" : "float");
        }
        else if (node->getName() == "double")
        {
            write(nullable ? "Double" : "double");
        }
        else if (node->getName() == "string")
        {
            write("String");
        }
        else if (node->getName() == "any")
        {
            write("Object");
        }
        else
        {
            write("%s", node->getName().c_str());
        }
    }

    virtual void at(const SequenceType* node)
    {
        Node* spec = node->getSpec();
        spec->accept(this);
        write("[]");
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

        write("public ");
        if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        write(" get%s()", cap.c_str());
        if (useExceptions && node->getGetRaises())
        {
            write(" throws ");
            printChildren(node->getGetRaises(), ", ");
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
        write("public ");
        write("void set%s(", cap.c_str());
        if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        write(" %s)", name.c_str());
        if (useExceptions && node->getSetRaises())
        {
            write(" throws ");
            printChildren(node->getSetRaises(), ", ");
        }
        return true;
    }

    virtual void at(const OpDcl* node)
    {
        if (!constructorMode)
        {
            write("public ");
        }
        else
        {
            write("static ");
        }

        bool needComma = true;  // true to write "," before the 1st parameter
        Node* spec = node->getSpec();
        if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        write(" %s(", node->getName().c_str());
        needComma = false;

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
            write(" throws ");
            printChildren(node->getRaises(), ", ");
        }
    }

    virtual void at(const ParamDcl* node)
    {
        Node* spec = node->getSpec();
        if (node->isVariadic())
        {
            spec->accept(this);
            write("...");
        }
        else if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        write(" %s", node->getName().c_str());
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
};

#endif  // ESIDL_JAVA_H_INCLUDED
