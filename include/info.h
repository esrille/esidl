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

#ifndef ESIDL_INFO_H_INCLUDED
#define ESIDL_INFO_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "esidl.h"
#include "expr.h"
#include "formatter.h"
#include "reflect.h"

// Generate the string-encoded interface information for reflection
class Info : public Visitor, public Formatter
{
    Node* getSpec(const Member* node) const
    {
        Node* spec = node->getSpec();
        if (Member* member = spec->isTypedef(currentNode))
        {
            if (ScopedName* scopedName = dynamic_cast<ScopedName*>(spec))
            {
                spec = scopedName->search(currentNode);
                assert(spec);
            }
        }
        else if (NativeType* native = spec->isNative(currentNode))
        {
            spec = native;
        }
        return spec;
    }

protected:
    bool constructorMode;
    int optionalStage;
    int optionalCount;

    std::string moduleName;
    std::string objectTypeName;
    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

    void writeName(const Node* node)
    {
        write("%zu%s", node->getName().length(), node->getName().c_str());
    }

    int getParamCount() const
    {
        return paramCount;
    }

    const ParamDcl* getVariadic() const
    {
        return variadicParam;
    }

    void printChildren(const Node* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        const Node* saved = currentNode;
        std::string separator;
        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            if (1 < (*i)->getRank())
            {
                continue;
            }
            if ((*i)->isNative(node->getParent()))
            {
                continue;
            }
            currentNode = (*i);
            (*i)->accept(this);
        }
        currentNode = saved;
    }

public:
    Info(Formatter* f, std::string& moduleName, std::string& objectTypeName) :
        Formatter(f),
        constructorMode(false),
        moduleName(moduleName),
        objectTypeName(objectTypeName),
        currentNode(getSpecification()),
        paramCount(0),
        variadicParam(0)
    {
    }

    virtual void at(const Node* node)
    {
        if (0 < node->getName().size())
        {
            if (node->isInterface(currentNode))
            {
                write("%c", Reflect::kObject);
            }

            std::string name = node->getName();
            Node* resolved = resolve(currentNode, name);
            if (resolved)
            {
                name = resolved->getQualifiedName();
                name = getInterfaceName(name);
            }
            write("%zu%s", name.length(), name.c_str());
        }
        else
        {
            printChildren(node);
        }
    }

    virtual void at(const Module* node)
    {
        // Info{} visiter should be applied for interfaces.
    }

    virtual void at(const Type* node)
    {
        if (node->getAttr() & Node::Nullable)
        {
            write("%c", Reflect::kNullable);
        }
        if (node->getName() == "void")
        {
            write("%c", Reflect::kVoid);
        }
        else if (node->getName() == "boolean")
        {
            write("%c", Reflect::kBoolean);
        }
        else if (node->getName() == "octet")
        {
            write("%c", Reflect::kOctet);
        }
        else if (node->getName() == "short")
        {
            write("%c", Reflect::kShort);
        }
        else if (node->getName() == "unsigned short")
        {
            write("%c", Reflect::kUnsignedShort);
        }
        else if (node->getName() == "long")
        {
            write("%c", Reflect::kLong);
        }
        else if (node->getName() == "unsigned long")
        {
            write("%c", Reflect::kUnsignedLong);
        }
        else if (node->getName() == "long long")
        {
            write("%c", Reflect::kLongLong);
        }
        else if (node->getName() == "unsigned long long")
        {
            write("%c", Reflect::kUnsignedLongLong);
        }
        else if (node->getName() == "float")
        {
            write("%c", Reflect::kFloat);
        }
        else if (node->getName() == "double")
        {
            write("%c", Reflect::kDouble);
        }
        else if (node->getName() == "any")
        {
            write("%c", Reflect::kAny);
        }
        else if (node->getName() == "string")
        {
            write("%c", Reflect::kString);
        }
        else
        {
            fprintf(stderr, "Unsupported type name: %s.\n",
                    node->getName().c_str());
            exit(EXIT_FAILURE);
        }
    }

    virtual void at(const NativeType* node)
    {
        if (node->getName() == "void_pointer")
        {
            write("%c", Reflect::kPointer);  // XXX To be unsupported
        }
        else
        {
            fprintf(stderr, "Unsupported type name: %s.\n",
                    node->getName().c_str());
            exit(EXIT_FAILURE);
        }
    }

    virtual void at(const SequenceType* node)
    {
        if (node->getAttr() & Node::Nullable)
        {
            write("%c", Reflect::kNullable);
        }
        write("%c", Reflect::kSequence);
        Node* spec = node->getSpec();
        spec->accept(this);
    }

    void getter(const Attribute* node)
    {
        static Type replaceable("any");
        Node* spec = getSpec(node);
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

        write("%c", Reflect::kGetter);
        if (node->isStringifier())
        {
            write("%c", Reflect::kSpecialStringifier);
        }
        write("0");

        if (seq)
        {
            seq->accept(this);
        }
        else if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        writeName(node);

        if (node->getGetRaises())
        {
            for (NodeList::iterator i = node->getGetRaises()->begin();
                 i != node->getGetRaises()->end();
                 ++i)
            {
                write("%c", Reflect::kRaises);
                (*i)->accept(this);
            }
        }
    }

    bool setter(const Attribute* node)
    {
        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return false;
        }

        static Type replaceable("any");
        Node* spec = getSpec(node);
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
            spec = getSpec(forwards);
        }
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

        write("%c1%c", Reflect::kSetter, Reflect::kVoid);
        writeName(node);
        if (seq)
        {
            seq->accept(this);
        }
        else if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }

        if (node->getSetRaises())
        {
            for (NodeList::iterator i = node->getSetRaises()->begin();
                 i != node->getSetRaises()->end();
                 ++i)
            {
                write("%c", Reflect::kRaises);
                (*i)->accept(this);
            }
        }
        return true;
    }

    virtual void at(const Attribute* node)
    {
        writetab();
        write("\"");
        getter(node);
        write("\"");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            writeln("");
            writetab();
            write("\"");
            setter(node);
            write("\"");
        }
    }

    virtual void at(const OpDcl* node)
    {
        writetab();
        write("\"");
        if (constructorMode)
        {
            write("%c", Reflect::kConstructor);
        }
        else
        {
            write("%c", Reflect::kOperation);
        }

        if (node->getAttr() & OpDcl::Omittable)
        {
            write("%c", Reflect::kSpecialOmittable);
        }

        if (node->getAttr() & OpDcl::IndexGetter)
        {
            write("%c", Reflect::kSpecialGetter);
        }
        if (node->getAttr() & OpDcl::IndexSetter)
        {
            write("%c", Reflect::kSpecialSetter);
        }
        if (node->getAttr() & OpDcl::IndexCreator)
        {
            write("%c", Reflect::kSpecialCreator);
        }
        if (node->getAttr() & OpDcl::IndexDeleter)
        {
            write("%c", Reflect::kSpecialDeleter);
        }
        if (node->getAttr() & OpDcl::Caller)
        {
            write("%c", Reflect::kSpecialCaller);
        }
        if (node->getAttr() & OpDcl::Stringifier)
        {
            write("%c", Reflect::kSpecialStringifier);
        }

        NodeList::reverse_iterator last = node->rbegin();
        if (last != node->rend())
        {
            const ParamDcl* param = dynamic_cast<const ParamDcl*>(*last);
            if (param && param->isVariadic())
            {
                write("%c", Reflect::kVariadic);
            }
        }

        write("%u", node->getParamCount(optionalStage));

        Node* spec = getSpec(node);
        spec->accept(this);
        writeName(node);

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
            ++paramCount;
            param->accept(this);
        }

        if (node->getRaises())
        {
            for (NodeList::iterator i = node->getRaises()->begin(); i != node->getRaises()->end(); ++i)
            {
                write("%c", Reflect::kRaises);
                (*i)->accept(this);
            }
        }
        write("\"");
    }

    virtual void at(const ParamDcl* node)
    {
        Node* spec = getSpec(node);
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));
        if (seq)
        {
            seq->accept(this);
        }
        else if (spec->isStruct(node->getParent()))
        {
            spec->accept(this);
        }
        else if (spec->isArray(node->getParent()))
        {
            spec->accept(this);
        }
        else if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else if (spec->isInterface(node->getParent()))
        {
            spec->accept(this);
        }
        else
        {
            spec->accept(this);
        }
        writeName(node);
    }

    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (element->isSequence(interface) ||
            element->isNative(interface) ||
            element->isTypedef(interface) ||
            element->isInterface(interface))  // Do not process Constructor
        {
            return;
        }
        optionalStage = 0;
        do
        {
            write("\n");
            optionalCount = 0;
            element->accept(this);
            ++optionalStage;
        } while (optionalStage <= optionalCount);
    }

    virtual void at(const Interface* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        currentNode = node;

        write("\n");
        writetab();
        write("\"%c", Reflect::kInterface);
        writeName(node);
        write("\"");

        if (node->getExtends())
        {
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                std::string name = (*i)->getName();
                Node* resolved = resolve(currentNode, name);
                (*i)->check(resolved, "could not resolve `%s`.", name.c_str());
                name = resolved->getQualifiedName();
                write("\n");
                writetab();
                write("\"%c", Reflect::kExtends);
                name = getInterfaceName(name);
                write("%zu%s", name.length(), name.c_str());
                write("\"");
            }
        }

        for (std::list<const Interface*>::const_iterator i = node->getMixins()->begin();
             i != node->getMixins()->end();
             ++i)
        {
            std::string name = (*i)->getQualifiedName();
            write("\n");
            writetab();
            write("\"%c", Reflect::kImplements);
            name = getInterfaceName(name);
            write("%zu%s", name.length(), name.c_str());
            write("\"");
        }

        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            visitInterfaceElement(node, *i);
        }

        // Expand mixins
        for (std::list<const Interface*>::const_iterator i = node->getMixins()->begin();
                i != node->getMixins()->end();
                ++i)
        {
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                visitInterfaceElement(*i, *j);
            }
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
    }

    virtual void at(const ConstDcl* node)
    {
        Type* type = node->getType();
        assert(type);
        writetab();
        write("\"C");
        type->accept(this);
        writeName(node);
        if (type->getName() == "boolean")
        {
            EvalInteger<bool> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%u ", eval.getValue());
        }
        else if (type->getName() == "octet")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%u ", eval.getValue());
        }
        else if (type->getName() == "short")
        {
            EvalInteger<int16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%d ", eval.getValue());
        }
        else if (type->getName() == "unsigned short")
        {
            EvalInteger<uint16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%u ", eval.getValue());
        }
        else if (type->getName() == "long")
        {
            EvalInteger<int32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%d ", eval.getValue());
        }
        else if (type->getName() == "unsigned long")
        {
            EvalInteger<uint32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%u ", eval.getValue());
        }
        else if (type->getName() == "long long")
        {
            EvalInteger<int64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%lld ", eval.getValue());
        }
        else if (type->getName() == "unsigned long long")
        {
            EvalInteger<uint64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%llu ", eval.getValue());
        }
        else if (type->getName() == "float")
        {
            EvalFloat<float> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%.10g ", eval.getValue());
        }
        else if (type->getName() == "double")
        {
            EvalFloat<double> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("%.20g ", eval.getValue());
        }
        else
        {
            fprintf(stderr, "Inv. const type.\n");
            exit(EXIT_FAILURE);
        }
        write("\"");
    }

    virtual void at(const Member* node)
    {
        assert(node->isTypedef(node->getParent()));
        getSpec(node)->accept(this);
    }

    virtual void at(const ArrayDcl* node)
    {
        assert(node->isTypedef(node->getParent()));
        assert(node->getDimension() == 1);
        EvalInteger<uint32_t> eval(node->getParent());
        (*(node->begin()))->accept(&eval);
        write("%c%u", Reflect::kArray, eval.getValue());
        getSpec(node)->accept(this);
    }

    std::string getInterfaceName(std::string qualifiedName)
    {
        if (qualifiedName == Node::getBaseObjectName())
        {
            qualifiedName.replace(2, qualifiedName.length(), objectTypeName);
        }
        return qualifiedName;
    }
};

#endif  // ESIDL_INFO_H_INCLUDED
