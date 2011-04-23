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

#ifndef ESIDL_MESSENGER_INVOKE_H_INCLUDED
#define ESIDL_MESSENGER_INVOKE_H_INCLUDED

#include <algorithm>
#include <set>
#include <vector>
#include "messenger.h"

class MessengerInvoke : public Messenger
{
    unsigned methodNumber;
    std::string className;
    unsigned offset;

    const char* writeInvoke(const Node* node, Node* spec)
    {
        const char* post = ")";

        writetab();
        if (spec->isVoid(node))
        {
            write("message_(");
        }
        else if (spec->isAny(node) || spec->isSequence(node) || (spec->getAttr() & Node::Nullable))
        {
            write("return message_(");
        }
        else if (spec->isInterface(node) || spec->isArray(node))
        {
            if (!constructorMode)
                write("return message_(");
            else
                write("*this = getConstructor().message_(");
            post = ").toObject()";
        }
        else if (spec->isString(node))
        {
            write("return message_(");
            post = ").toString()";
        }
        else
        {
            write("return static_cast<");
            spec->accept(this);
            write(">(message_(");
            post = "))";
        }
        if (constructorMode || (node->getAttr() & Node::UnnamedProperty))
            write("0, \"\", ");
        else
            write("0x%x, \"%s\", ", node->getHash(), node->getName().c_str());
        ++methodNumber;
        return post;
    }

public:
    MessengerInvoke(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        Messenger(file, stringTypeName, objectTypeName, useExceptions, indent)
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
            className = node->getName();
            if (constructorMode && (node->getAttr() & Interface::Constructor))
            {
                className = currentNode->getName();
            }
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

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
        getter(node, className);
        writeln("{");
            const char* post = writeInvoke(node, spec);
            write("GETTER_, 0%s;\n", post);
        writeln("}");
        offset += node->getMetaGetter().length();

        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return;
        }

        // setter
        writetab();
        setter(node, className);
        writeln("{");
            writeln("Any argument_ = %s;", name.c_str());
            writeln("message_(0x%x, \"%s\", SETTER_, &argument_);", node->getHash(), node->getName().c_str());
        writeln("}");
        offset += node->getMetaSetter().length();

        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        noDefaultArgument = true;
        Messenger::at(node, className);
        noDefaultArgument = false;
        if (constructorMode)
        {
            Interface* prototype = dynamic_cast<Interface*>(interface->getParent());
            if (prototype && prototype->getExtends())
            {
                const char* separator = " : ";
                for (NodeList::iterator i = prototype->getExtends()->begin();
                    i != prototype->getExtends()->end();
                    ++i)
                {
                    write(separator);
                    separator = ", ";
                    (*i)->accept(this);
                    write("(0)");
                }
            }
        }
        writeln("{");

            // Invoke
            std::string variadicName;
            int paramCount = getParamCount();
            if (0 < paramCount)
            {
                if (!getVariadic())
                {
                    writeln("Any arguments_[%u];", paramCount);
                }
                else
                {
                    --paramCount;
                    writeln("Any arguments_[%u + MAX_VARIADIC_];", paramCount);
                }
                NodeList::iterator it = node->begin();
                int i;
                for (i = 0; i < paramCount; ++i, ++it)
                {
                    ParamDcl* param = static_cast<ParamDcl*>(*it);
                    writeln("arguments_[%d] = %s;", i, getEscapedName(param->getName()).c_str());
                }
                if (getVariadic())
                {
                    ParamDcl* param = static_cast<ParamDcl*>(*it);
                    variadicName = getEscapedName(param->getName());
                    writeln("for (size_t i = 0; i < %s.size(); ++i) {", variadicName.c_str());
                        writeln("arguments_[%d + i] = %s[i];", i, variadicName.c_str());
                    writeln("}");
                }
            }
            const char* post = writeInvoke(node, node->getSpec());
            switch (node->getAttr() & OpDcl::IndexMask)
            {
            case OpDcl::IndexCreator:
                write("SPECIAL_CREATOR_");
                break;
            case OpDcl::IndexDeleter:
                write("SPECIAL_DELETER_");
                break;
            case OpDcl::IndexGetter:
                write("SPECIAL_GETTER_");
                break;
            case OpDcl::IndexSetter:
                write("SPECIAL_SETTER_");
                break;
            case OpDcl::IndexSetter | OpDcl::IndexCreator:
                write("SPECIAL_SETTER_CREATOR_");
                break;
            case 0:
                if (interface->isCallback())
                {
                    write("CALLBACK_ | ");
                }
                if (!getVariadic())
                {
                    write("%u", paramCount);
                }
                else
                {
                    write("%u + %s.size()", paramCount, variadicName.c_str());
                }
                break;
            default:
                node->check(node->getAttr(), "%s has invalid specials", node->getName().c_str(), node->getAttr());
                break;
            }
            write((0 < getParamCount()) ? ", arguments_" : ", 0");
            write("%s;\n", post);

        writeln("}");
        offset += node->getMetaOp(optionalStage).length();
    }
};

#endif  // ESIDL_MESSENGER_INVOKE_H_INCLUDED
