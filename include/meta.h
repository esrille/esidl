/*
 * Copyright 2011, 2012 Esrille Inc.
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

#ifndef ESIDL_META_H_INCLUDED
#define ESIDL_META_H_INCLUDED

#include <stdlib.h>
#include <string>
#include <sstream>
#include "esidl.h"
#include "expr.h"
#include "reflect.h"

// Set the string-encoded metadata to each node
class Meta : public Visitor
{
protected:
    int optionalStage;
    int optionalCount;

    std::string objectTypeName;
    const Node* currentNode;

    int paramCount;  // The number of parameters of the previously evaluated operation
    const ParamDcl* variadicParam;  // Non-NULL if the last parameter of the previously evaluated operation is variadic

    std::ostringstream output;

    void writeName(const Node* node)
    {
        output << node->getName().length() << node->getName();
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
            if ((*i)->isNative(node->getParent()) || (*i)->isEnum(node->getParent()))
            {
                continue;
            }
            currentNode = (*i);
            (*i)->accept(this);
        }
        currentNode = saved;
    }

    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (element->isSequence(interface) ||
            element->isNative(interface) ||
            element->isEnum(interface) ||
            element->isTypedef(interface))
        {
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
    Meta(const std::string& objectTypeName) :
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
            assert(!node->isInterface(currentNode));
            std::string name = node->getName();
            Node* resolved = resolve(currentNode, name);
            if (resolved)
            {
                name = resolved->getQualifiedName();
            }
            output << name.length() << name;
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
        if (dynamic_cast<ExceptDcl*>(resolved))
        {
            std::string name = resolved->getQualifiedName();
            output << name.length() << name;
        }
        else if (dynamic_cast<Interface*>(resolved))
        {
            std::string name = resolved->getQualifiedName();
            name = getInterfaceName(name);
            output << Reflect::kObject << name.length() << name;
        }
        else
        {
            const Node* saved = currentNode;
            currentNode = resolved->getParent();
            resolved->accept(this);
            currentNode = saved;
        }
    }

    virtual void at(const Module* node)
    {
        printChildren(node);
    }

    virtual void at(const Type* node)
    {
        if (node->getName() == "void")
        {
            output << Reflect::kVoid;
        }
        else if (node->getName() == "boolean")
        {
            output << Reflect::kBoolean;
        }
        else if (node->getName() == "octet")
        {
            output << Reflect::kOctet;
        }
        else if (node->getName() == "byte")
        {
            output << Reflect::kByte;
        }
        else if (node->getName() == "unsigned byte")
        {
            output << Reflect::kUnsignedByte;
        }
        else if (node->getName() == "short")
        {
            output << Reflect::kShort;
        }
        else if (node->getName() == "unsigned short")
        {
            output << Reflect::kUnsignedShort;
        }
        else if (node->getName() == "long")
        {
            output << Reflect::kLong;
        }
        else if (node->getName() == "unsigned long")
        {
            output << Reflect::kUnsignedLong;
        }
        else if (node->getName() == "long long")
        {
            output << Reflect::kLongLong;
        }
        else if (node->getName() == "unsigned long long")
        {
            output << Reflect::kUnsignedLongLong;
        }
        else if (node->getName() == "float")
        {
            output << Reflect::kFloat;
        }
        else if (node->getName() == "double")
        {
            output << Reflect::kDouble;
        }
        else if (node->getName() == "any")
        {
            output << Reflect::kAny;
        }
        else if (node->getName() == "string")
        {
            output << Reflect::kString;
        }
        else if (node->getName() == "Date")
        {
            output << Reflect::kDate;
        }
        else
        {
            node->report("Unsupported type name: %s.", node->getName().c_str());
            exit(EXIT_FAILURE);
        }
        if (node->getAttr() & Node::Nullable)
        {
            output << Reflect::kNullable;
        }
    }

    virtual void at(const NativeType* node)
    {
        if (node->getName() == "void_pointer")
        {
            output << Reflect::kPointer;  // XXX To be unsupported
        }
        else
        {
            node->report("Unsupported type name: %s.", node->getName().c_str());
            exit(EXIT_FAILURE);
        }
    }

    virtual void at(const UnionType* node)
    {
        // TODO: Maybe we should treat union separately.
        output << Reflect::kAny;
    }

    virtual void at(const Enum* node)
    {
        output << Reflect::kString;
    }

    virtual void at(const SequenceType* node)
    {
        output << Reflect::kSequence;
        if (node->getMax())
        {
            output << const_cast<SequenceType*>(node)->getLength(currentNode);
        }
        Node* spec = node->getSpec();
        spec->accept(this);
        if (node->getAttr() & Node::Nullable)
        {
            output << Reflect::kNullable;
        }
    }

    virtual void at(const ArrayType* node)
    {
        output << Reflect::kArray;
        if (node->getMax())
        {
            output << const_cast<ArrayType*>(node)->getLength(currentNode);
        }
        Node* spec = node->getSpec();
        spec->accept(this);
    }

    void getter(const Attribute* node)
    {
        output.str("");

        static Type replaceable("any");
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

        output << Reflect::kGetter;
        if (node->isStringifier())
        {
            output << Reflect::kSpecialStringifier;
        }
        output << '0';

        if (seq)
        {
            spec->accept(this);
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
                output << Reflect::kRaises;
                (*i)->accept(this);
            }
        }

        node->getMetaGetter() = output.str();
    }

    bool setter(const Attribute* node)
    {
        output.str("");

        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return false;
        }

        static Type replaceable("any");
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
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));

        output << Reflect::kSetter << '1' << Reflect::kVoid;
        writeName(node);
        if (seq)
        {
            spec->accept(this);
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
                output << Reflect::kRaises;
                (*i)->accept(this);
            }
        }

        node->getMetaSetter() = output.str();

        return true;
    }

    virtual void at(const Attribute* node)
    {
        getter(node);
        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            setter(node);
        }
        node->getMeta() = node->getMetaGetter() + node->getMetaSetter();
        node->setHash();
    }

    virtual void at(const OpDcl* node)
    {
        output.str("");
        output << Reflect::kOperation;

        if (node->getAttr() & OpDcl::Omittable)
        {
            output << Reflect::kSpecialOmittable;
        }

        if (node->getAttr() & OpDcl::IndexGetter)
        {
            output << Reflect::kSpecialGetter;
        }
        if (node->getAttr() & OpDcl::IndexSetter)
        {
            output << Reflect::kSpecialSetter;
        }
        if (node->getAttr() & OpDcl::IndexCreator)
        {
            output << Reflect::kSpecialCreator;
        }
        if (node->getAttr() & OpDcl::IndexDeleter)
        {
            output << Reflect::kSpecialDeleter;
        }
        if (node->getAttr() & OpDcl::Caller)
        {
            output << Reflect::kSpecialCaller;
        }
        if (node->getAttr() & OpDcl::Stringifier)
        {
            output << Reflect::kSpecialStringifier;
        }

        output << node->getParamCount(optionalStage);

        Node* spec = node->getSpec();
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
                output << Reflect::kRaises;
                (*i)->accept(this);
            }
        }

        node->getMetaOp(optionalStage) = output.str();

        if (variadicParam)
        {
            // Insert Reflect::kVariadic just after Reflect::kOperation.
            node->getMetaOp(optionalStage).insert(1, 1, Reflect::kVariadic);
        }
        node->setHash();
    }

    virtual void at(const ParamDcl* node)
    {
        Node* spec = node->getSpec();
        if (node->isVariadic())
        {
            variadicParam = node;
        }
        spec->accept(this);
        writeName(node);
    }

    virtual void at(const Interface* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        currentNode = node;

        output.str("");
        output << Reflect::kInterface;
        std::string name = node->getQualifiedName();
        name = getInterfaceName(name);
        output << name.length() << name;

        if (node->getExtends())
        {
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                ScopedName* scopedName = static_cast<ScopedName*>(*i);
                Node* resolved = scopedName->search(node->getParent());
                scopedName->check(resolved, "could not resolve '%s'.", scopedName->getName().c_str());
                output << Reflect::kExtends;
                std::string name = resolved->getQualifiedName();
                name = getInterfaceName(name);
                output << name.length() << name;
            }
        }

        for (std::list<const Interface*>::const_iterator i = node->getImplements()->begin();
             i != node->getImplements()->end();
             ++i)
        {
            output << Reflect::kImplements;
            std::string name = (*i)->getQualifiedName();
            name = getInterfaceName(name);
            output << name.length() << name;
        }
        node->getMeta() = output.str();

        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            visitInterfaceElement(node, *i);
        }

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
            at(constructor);
        }
    }

    virtual void at(const ConstDcl* node)
    {
        Type* type = node->getType();
        assert(type);

        output.str("");
        output << Reflect::kConstant;
        type->accept(this);
        writeName(node);
        if (type->getName() == "boolean")
        {
            EvalInteger<bool> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "octet")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "byte")
        {
            EvalInteger<int8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "unsigned byte")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "short")
        {
            EvalInteger<int16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "unsigned short")
        {
            EvalInteger<uint16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "long")
        {
            EvalInteger<int32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "unsigned long")
        {
            EvalInteger<uint32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "long long")
        {
            EvalInteger<int64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "unsigned long long")
        {
            EvalInteger<uint64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "float")
        {
            EvalFloat<float> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else if (type->getName() == "double")
        {
            EvalFloat<double> eval(node->getParent());
            node->getExp()->accept(&eval);
            output << eval.getValue();
        }
        else
        {
            node->report("Inv. const type.");
            exit(EXIT_FAILURE);
        }
        output << ' ';
        node->getMeta() = output.str();
        node->setHash();
    }

    virtual void at(const Member* node)
    {
        assert(node->isTypedef(node->getParent()));
        node->getSpec()->accept(this);
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

#endif  // ESIDL_META_H_INCLUDED
