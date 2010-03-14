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

#ifndef ESIDL_JAVA_H_INCLUDED
#define ESIDL_JAVA_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "esidl.h"
#include "formatter.h"

#define USE_CONSTRUCTOR

class Java : public Visitor, public Formatter
{
protected:
    const char* source;

    bool useExceptions;

    int optionalStage;
    int optionalCount;

    std::string moduleName;
    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

    std::string methodAccessLevel;

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

        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            if (separater && i != node->begin())
            {
                write("%s", separater);
            }
            (*i)->accept(this);
        }
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
        useExceptions(true),
        currentNode(0),
        paramCount(0),
        variadicParam(0),
        methodAccessLevel("public")
    {
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
        Node* resolved = node->search(currentNode);
        node->check(resolved, "%s could not resolved.", node->getName().c_str());
        const Node* saved = currentNode;
        if (resolved->getParent())
        {
            currentNode = resolved->getParent();
        }
        if (!dynamic_cast<Interface*>(resolved) && !dynamic_cast<ExceptDcl*>(resolved))
        {
            resolved->accept(this);
        }
        else if (resolved->isBaseObject())
        {
            write("Object");
        }
        else
        {
            write("%s", getEscapedName(resolved->getName()).c_str());
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

            moduleName += "::";
            moduleName += node->getName();
            printChildren(node);
            moduleName.erase(moduleName.size() - node->getName().size() - 2);
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
        else if (node->getName() == "byte" || node->getName() == "unsigned byte")
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

    virtual void at(const ArrayType* node)
    {
        Node* spec = node->getSpec();
        if (ScopedName* name = dynamic_cast<ScopedName*>(spec))
        {
            spec = name->search(currentNode);
        }
        Type* type = dynamic_cast<Type*>(spec);
        if (type && !(type->getAttr() & Node::Nullable))
        {
            if (type->getName() == "boolean")
            {
                write("BooleanArray");
                return;
            }
            else if (type->getName() == "octet")
            {
                write("OctetArray");
                return;
            }
            else if (type->getName() == "byte")
            {
                write("ByteArray");
                return;
            }
            else if (type->getName() == "unsigned byte")
            {
                write("UnsignedByteArray");
                return;
            }
            else if (type->getName() == "short")
            {
                write("ShortArray");
                return;
            }
            else if (type->getName() == "unsigned short")
            {
                write("UnsignedShortArray");
                return;
            }
            else if (type->getName() == "long")
            {
                write("LongArray");
                return;
            }
            else if (type->getName() == "unsigned long")
            {
                write("UnsignedLongArray");
                return;
            }
            else if (type->getName() == "long long")
            {
                write("LongLongArray");
                return;
            }
            else if (type->getName() == "unsigned long long")
            {
                write("UnsignedLongLongArray");
                return;
            }
            else if (type->getName() == "float")
            {
                write("FloatArray");
                return;
            }
            else if (type->getName() == "double")
            {
                write("DoubleArray");
                return;
            }
        }
        write("ObjectArray<");
        node->getSpec()->accept(this);
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

        write("%s ", methodAccessLevel.c_str());
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
        write("%s ", methodAccessLevel.c_str());
        write("void set%s(", cap.c_str());
        if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        write(" %s)", getEscapedName(name).c_str());
        if (useExceptions && node->getSetRaises())
        {
            write(" throws ");
            printChildren(node->getSetRaises(), ", ");
        }
        return true;
    }

    virtual void at(const OpDcl* node)
    {
        write("%s ", methodAccessLevel.c_str());

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
        write(" %s(", getEscapedName(node->getName()).c_str());
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

#endif  // ESIDL_JAVA_H_INCLUDED
