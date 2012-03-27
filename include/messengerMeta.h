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

#ifndef ESIDL_MESSENGER_META_H_INCLUDED
#define ESIDL_MESSENGER_META_H_INCLUDED

#include <map>
#include <string>
#include "esidl.h"
#include "expr.h"
#include "messenger.h"

/**
 *  name -> digits identifier  // digits represents the # of characters in the identifier
 *
 *  interface ->
 *    I constructor? name
 *    extends?
 *    [ operation, property, constant ]*
 *
 *  operation -> F special* name
 *  attribute -> A special* name
 *  constant  -> K name
 *
 *  extends -> X name
 *  constructor -> c
 *  special ->
 *    r: read-only
 *    g: getter
 *    s: setter
 *    c: creator
 *    d: deleter
 *    f: caller
 *    t: stringifier
 *    o: omittable
 */

// Generate the string-encoded interface information for reflection
class MessengerMeta : public Messenger
{
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
        element->accept(this);
    }

    void processSpecials(const Node* node)
    {
        if (node->getAttr() & Node::Stringifier)
        {
            write("t");
        }
        if (node->getAttr() & Node::IndexCreator)
        {
            write("c");
        }
        if (node->getAttr() & Node::IndexDeleter)
        {
            write("d");
        }
        if (node->getAttr() & Node::IndexGetter)
        {
            write("g");
        }
        if (node->getAttr() & Node::IndexSetter)
        {
            write("s");
        }
        if (node->getAttr() & Node::Caller)
        {
            write("f");
        }
        if (node->getAttr() & Node::Omittable)
        {
            write("o");
        }
    }

public:
    MessengerMeta(Formatter* formatter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        Messenger(formatter, stringTypeName, objectTypeName, useExceptions)
    {
        formatter->flush();
        currentNode = 0;
    }

    virtual void at(const Module* node)
    {
        // MessengerMeta{} visiter should be applied for interfaces.
    }

    virtual void at(const Attribute* node)
    {

        write("\"A");
        if (node->isReadonly() && !node->isPutForwards() && node->isReplaceable())
        {
            write("r");
        }
        processSpecials(node);
        write("%u%s\"", node->getName().length(), node->getName().c_str());
    }

    virtual void at(const OpDcl* node)
    {
        if (constructorMode)
        {
            write("\"C");
        }
        else
        {
            write("\"F");
        }
        processSpecials(node);
        if (!(node->getAttr() & OpDcl::UnnamedProperty))
            write("%u%s\"", node->getName().length(), node->getName().c_str());
        else
            write("0\"");
    }

    virtual void at(const Interface* node)
    {
        if (node->isLeaf())
        {
            return;
        }

        currentNode = node;

        writeln("static const char* const getMetaData() {");
            writeln("static const char* metaData =");
            indent();
            writetab();
            write("\"I");

#ifdef USE_CONSTRUCTOR
            if (node->getConstructor())
            {
                write("c");
            }
#endif
            write("%u%s\"", node->getPrefixedName().length(), node->getPrefixedName().c_str());

            if (node->getExtends())
            {
                for (NodeList::iterator i = node->getExtends()->begin();
                    i != node->getExtends()->end();
                    ++i)
                {
                    if (!(*i)->isBaseObject())
                    {
                        ScopedName* scopedName = static_cast<ScopedName*>(*i);
                        Node* resolved = scopedName->search(node->getParent());
                        scopedName->check(resolved, "could not resolve '%s'.", scopedName->getName().c_str());
                        writeln("");
                        writetab();
                        write("\"X%u%s\"", resolved->getPrefixedName().length(), resolved->getPrefixedName().c_str());
                    }
                }
            }

            // Expand supplementals
            std::list<const Interface*> interfaceList;
            node->collectSupplementals(&interfaceList);
            for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
                i != interfaceList.end();
                ++i)
            {
                const Node* saved = currentNode;
                constructorMode = node->isConstructor();
                for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
                {
                    currentNode = *i;
                    visitInterfaceElement(*i, *j);
                }
                constructorMode = false;
                currentNode = saved;
            }

            write(";\n");
            unindent();
            writeln("return metaData;");
        writeln("}");

        writeln("static const char* const getPrefixedName() {");
            writeln("static const char* const prefixedName = \"%s\";", node->getPrefixedName().c_str());
            writeln("return prefixedName;");
        writeln("}");

        writeln("static bool hasInstance(Object& object) {");
            writeln("return static_cast<bool>(object.message_(0, getPrefixedName(), IS_KIND_OF_, 0));");
        writeln("}");
        writeln("static bool hasInstance(Object* object) {");
            writeln("return object && static_cast<bool>(object->message_(0, getPrefixedName(), IS_KIND_OF_, 0));");
        writeln("}");

        flush();
    }

    virtual void at(const ConstDcl* node)
    {
        write("\"K%u%s", node->getName().length(), node->getName().c_str());

        Type* type = node->getType();
        assert(type);
        if (type->getName() == "boolean")
        {
            EvalInteger<bool> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("b%d ", eval.getValue());
        }
        else if (type->getName() == "octet")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("h%u ", eval.getValue());
        }
        else if (type->getName() == "byte")
        {
            EvalInteger<int8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("g%d ", eval.getValue());
        }
        else if (type->getName() == "unsigned byte")
        {
            EvalInteger<uint8_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("h%u ", eval.getValue());
        }
        else if (type->getName() == "short")
        {
            EvalInteger<int16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("s%d ", eval.getValue());
        }
        else if (type->getName() == "unsigned short")
        {
            EvalInteger<uint16_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("t%u ", eval.getValue());
        }
        else if (type->getName() == "long")
        {
            EvalInteger<int32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("l%ld ", eval.getValue());
        }
        else if (type->getName() == "unsigned long")
        {
            EvalInteger<uint32_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("m%lu ", eval.getValue());
        }
        else if (type->getName() == "long long")
        {
            EvalInteger<int64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("x%lld ", eval.getValue());
        }
        else if (type->getName() == "unsigned long long")
        {
            EvalInteger<uint64_t> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("y%llu ", eval.getValue());
        }
        else if (type->getName() == "float")
        {
            EvalFloat<float> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("f%a ", eval.getValue());
        }
        else if (type->getName() == "double")
        {
            EvalFloat<double> eval(node->getParent());
            node->getExp()->accept(&eval);
            write("d%a ", eval.getValue());
        }
        else
        {
            node->report("Inv. const type.");
            exit(EXIT_FAILURE);
        }
        // TODO: Support DOMString and Nullable
        write("\"");
    }

    virtual void at(const Enum* node)
    {
    }

    virtual void at(const Member* node)
    {
        assert(node->isTypedef(node->getParent()));
        node->getSpec()->accept(this);
    }
};

#endif  // ESIDL_MESSENGER_META_H_INCLUDED
