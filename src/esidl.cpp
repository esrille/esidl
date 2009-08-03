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

#include "esidl.h"
#include "parser.h"
#include "forward.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifdef __cplusplus
extern "C" {
#endif

int yyparse(void);

extern FILE* yyin;

#ifdef __cplusplus
}
#endif

namespace
{
    Node* specification;
    Node* current;          // Current name space
    const char* includePath;
    std::string javadoc;
    std::string savedJavadoc;
}

int Node::level = 1;
const char* Node::baseObjectName = "::Object";
const char* Node::namespaceName;

Node* getSpecification()
{
    return specification;
}

Node* setSpecification(Node* node)
{
    Node* prev = specification;
    specification = node;
    return prev;
}

const char* getIncludePath()
{
    return includePath;
}

void setIncludePath(const char* path)
{
    includePath = path;
}

Node* getCurrent()
{
    return current;
}

Node* setCurrent(const Node* node)
{
    Node* prev = current;
    current = const_cast<Node*>(node);
    return prev;
}

std::string& getJavadoc()
{
    return javadoc;
}

void setJavadoc(const char* doc)
{
    javadoc = doc ? doc : "";
}

std::string& popJavadoc()
{
    javadoc = savedJavadoc;
    return javadoc;
}

void pushJavadoc()
{
    savedJavadoc = getJavadoc();
    setJavadoc(0);
}

std::string getOutputFilename(std::string filename, const char* suffix)
{
    int begin = filename.rfind(".");
    int end   = filename.size();
    if (0 <= begin)
    {
        filename.replace(begin + 1, end, suffix);
    }
    else
    {
        filename += ".";
        filename += suffix;
    }

    if (getIncludePath())
    {
        int pos = filename.find(getIncludePath());
        if (pos == 0)
        {
            filename.replace(pos, strlen(getIncludePath()) + 1, "");
        }
        else
        {
            int slash = filename.rfind("/");
            if (0 <= slash)
            {
                filename.replace(0, slash + 1, "");
            }
        }
    }

    std::string dir;
    std::string path(filename);
    for (;;)
    {
        int slash = path.find("/");
        if (slash < 0)
        {
            break;
        }
        dir += path.substr(0, slash);
        path.erase(0, slash + 1);
        mkdir(dir.c_str(), 0777);
        dir += '/';
    }

    return filename;
}

void Node::setLocation(struct YYLTYPE* yylloc)
{
    firstLine = yylloc->first_line;
    firstColumn = yylloc->first_column;
    lastLine = yylloc->last_line;
    lastColumn = yylloc->last_column;
}

void Node::setLocation(struct YYLTYPE* first, struct YYLTYPE* last)
{
    firstLine = first->first_line;
    firstColumn = first->first_column;
    lastLine = last->last_line;
    lastColumn = last->last_column;
}

bool Module::hasPredeclarations() const
{
    if (isLeaf())
    {
        return false;
    }
    for (NodeList::iterator i = begin(); i != end(); ++i)
    {
        const Interface* interface = dynamic_cast<const Interface*>(*i);
        if (interface && 1 == interface->getRank() && interface->isLeaf())
        {
            return true;
        }
        const Module* module = dynamic_cast<const Module*>(*i);
        if (module && module->hasPredeclarations())
        {
            return true;
        }
    }
    return false;
}

void Module::add(Node* node)
{
    if (node->getRank() == 1)
    {
        if (Interface* interface = dynamic_cast<Interface*>(node))
        {
            if (!node->isLeaf())
            {
                ++interfaceCount;
            }
        }
        if (dynamic_cast<ConstDcl*>(node))
        {
            ++constCount;
        }
        if (dynamic_cast<Module*>(node))
        {
            ++moduleCount;
        }
    }
    Node::add(node);
}

void StructType::add(Node* node)
{
    ++memberCount;
    Node::add(node);
}

void Interface::add(Node* node)
{
    if (dynamic_cast<ConstDcl*>(node))
    {
        ++constCount;
    }
    else if (Interface* interface = dynamic_cast<Interface*>(node))
    {
        if (node->getRank() == 1)
        {
            assert(!interface->isLeaf());
            if (Module* module = dynamic_cast<Module*>(getParent()))
            {
                module->incInterfaceCount();
            }
        }
    }
    Node::add(node);
}

void OpDcl::add(Node* node)
{
    if (dynamic_cast<ParamDcl*>(node))
    {
        ++paramCount;
    }
    Node::add(node);
}

void OpDcl::adjustMethodCount()
{
    methodCount = 0;
    int optionalStage = 0;
    int optionalCount;
    do
    {
        optionalCount = 0;
        ++methodCount;
        int paramCount = 0;
        for (NodeList::iterator i = begin(); i != end(); ++i)
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

        }
        paramCounts.push_back(paramCount);
        ++optionalStage;
    } while (optionalStage <= optionalCount);
    Interface* interface = dynamic_cast<Interface*>(getParent());
    assert(interface);
    interface->addMethodCount(methodCount - 1);
}

void Implements::resolve()
{
    Interface* interface = dynamic_cast<Interface*>(getFirst()->search(getParent()));
    Interface* mixin = dynamic_cast<Interface*>(getSecond()->search(getParent()));
    assert(interface && mixin);
    interface->implements(mixin);
}

void Interface::processExtendedAttributes()
{
    NodeList* list = getExtendedAttributes();
    if (!list)
    {
        return;
    }

    ScopedName* interfaceName;
    for (NodeList::iterator i = list->begin(); i != list->end(); ++i)
    {
        ExtendedAttribute* ext = dynamic_cast<ExtendedAttribute*>(*i);
        assert(ext);
        if (ext->getName() == "Callback")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "FunctionOnly")
                {
                    attr |= CallbackIsFunctionOnly;
                }
                else if (name->getName() == "PropertyOnly")
                {
                    attr |= CallbackIsPropertyOnly;
                }
            }
            else
            {
                attr |= Callback;
            }
        }
        else if (ext->getName() == "NoInterfaceObject")
        {
            attr |= NoInterfaceObject;
        }
        else if (ext->getName() == "PrototypeRoot")
        {
            attr |= PrototypeRoot;
        }
        else if (ext->getName() == "Constructor" || ext->getName() == "NamedConstructor")
        {
            if (constructor == NULL)
            {
                Node* extends = 0;
                if (const char* base = getBaseObjectName())
                {
                    ScopedName* name = new ScopedName(base);
                    extends = new Node();
                    extends->add(name);
                }
                constructor = new Interface("Constructor", extends);
                constructor->setRank(getRank());
                add(constructor);

                interfaceName = new ScopedName(getName());
            }
            OpDcl* op;
            if (op = dynamic_cast<OpDcl*>(ext->getDetails()))
            {
                op->setSpec(interfaceName);
                op->getName() = "createInstance";
            }
            else
            {
                // Default constructor
                op = new OpDcl("createInstance", interfaceName);
            }
            constructor->add(op);
        }
    }
}

void Attribute::processExtendedAttributes()
{
    NodeList* list = getExtendedAttributes();
    if (!list)
    {
        return;
    }
    uint32_t attr = getAttr();
    for (NodeList::iterator i = list->begin(); i != list->end(); ++i)
    {
        ExtendedAttribute* ext = dynamic_cast<ExtendedAttribute*>(*i);
        assert(ext);
        if (ext->getName() == "Replaceable")
        {
            attr |= Replaceable;
        }
        else if (ext->getName() == "TreatNullAs")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "EmptyString")
                {
                    attr |= NullIsEmpty;
                }
            }
        }
        else if (ext->getName() == "TreatUndefinedAs")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "EmptyString")
                {
                    attr |= UndefinedIsEmpty;
                }
                else if (name->getName() == "Null")
                {
                    attr |= UndefinedIsNull;
                }
            }
        }
        else if (ext->getName() == "PutForwards")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                putForwards = name->getName();
            }
        }
    }
    setAttr(attr);
}

void OpDcl::processExtendedAttributes()
{
    NodeList* list = getExtendedAttributes();
    if (!list)
    {
        return;
    }
    uint32_t attr = getAttr();
    for (NodeList::iterator i = list->begin(); i != list->end(); ++i)
    {
        ExtendedAttribute* ext = dynamic_cast<ExtendedAttribute*>(*i);
        assert(ext);
        if (ext->getName() == "TreatNullAs")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "EmptyString")
                {
                    attr |= NullIsEmpty;
                }
            }
        }
        else if (ext->getName() == "TreatUndefinedAs")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "EmptyString")
                {
                    attr |= UndefinedIsEmpty;
                }
                else if (name->getName() == "Null")
                {
                    attr |= UndefinedIsNull;
                }
            }
        }
        else
        {
            // ignore
        }
    }
    setAttr(attr);
}

void ParamDcl::processExtendedAttributes()
{
    NodeList* list = getExtendedAttributes();
    if (!list)
    {
        return;
    }
    uint32_t attr = getAttr();
    for (NodeList::iterator i = list->begin(); i != list->end(); ++i)
    {
        ExtendedAttribute* ext = dynamic_cast<ExtendedAttribute*>(*i);
        assert(ext);
        if (ext->getName() == "TreatNullAs")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "EmptyString")
                {
                    attr |= NullIsEmpty;
                }
            }
        }
        else if (ext->getName() == "TreatUndefinedAs")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "EmptyString")
                {
                    attr |= UndefinedIsEmpty;
                }
                else if (name->getName() == "Null")
                {
                    attr |= UndefinedIsNull;
                }
            }
        }
    }
    setAttr(attr);
}

Node* Node::search(const std::string& elem, size_t pos) const
{
    if (elem.size() <= pos)
    {
        return const_cast<Node*>(this);
    }

    if (!children)
    {
        return 0;
    }

    Interface* forwardDecl = 0;

    size_t n = elem.find("::", pos);
    assert(n != 0);
    if (n == std::string::npos)
    {
        n = elem.size();
    }
    for (NodeList::reverse_iterator i = children->rbegin();
            i != children->rend();
            ++i)
    {
        if ((*i)->getName().compare(0, (*i)->getName().size(), elem, pos, n - pos) == 0)
        {
            Interface* interface = dynamic_cast<Interface*>(*i);
            if (interface && interface->isLeaf() && elem.size() <= n)
            {
                forwardDecl = interface;
                continue;
            }
            return (*i)->search(elem, n + 2);
        }
    }
    return forwardDecl;
}

Node* ScopedName::search(const Node* scope) const
{
    Node* resolved = resolve(scope, name);
    if (resolved)
    {
        if (Member* member = dynamic_cast<Member*>(resolved))
        {
            if (member->isTypedef(member->getParent()) && !member->isArray(member->getParent()))
            {
                if (ScopedName* node = dynamic_cast<ScopedName*>(member->getSpec()))
                {
                    if (Node* found = node->search(member->getParent()))
                    {
                        resolved = found;
                    }
                }
            }
        }
    }
    return resolved;
}

Node* resolve(const Node* scope, std::string name)
{
    if (name.compare(0, 2, "::") == 0)
    {
        return getSpecification()->search(name, 2);
    }
    else
    {
        for (const Node* node = scope; node; node = node->getParent())
        {
            if (Node* found = node->search(name))
            {
                return found;
            }
        }
    }
    return 0;
}

std::string getScopedName(std::string moduleName, std::string absoluteName)
{
    while (moduleName != "")
    {
        if (absoluteName.compare(0, moduleName.size(), moduleName) == 0)
        {
            return absoluteName.substr(moduleName.size() + 2);
        }
        size_t pos = moduleName.rfind("::");
        if (pos == std::string::npos)
        {
            moduleName = "";
        }
        else
        {
            moduleName.erase(pos);
        }
    }
    return absoluteName.substr(2);
}

std::string getIncludedName(const std::string& header)
{
    std::string included(header);

    for (int i = 0; i < included.size(); ++i)
    {
        char c = included[i];
        included[i] = toupper(c);
        if (c == '.' || c == '/' || c == '\\')
        {
            included[i] = '_';
        }
    }
    included += "_INCLUDED";
    if (isdigit(included[0]))
    {
        included = "NO" + included;
    }
    return included;
}

void Interface::processExtendedAttributes(OpDcl* op)
{
    attr |= (op->getAttr() & IndexMask);
    ++methodCount;
}

void Interface::processExtendedAttributes(Attribute* attr)
{
    if (attr->isReadonly() && !attr->isPutForwards() && !attr->isReplaceable())
    {
        ++methodCount;
    }
    else
    {
        methodCount += 2;
    }
}

void yyerror(const char* message, ...)
{
    va_list ap;

    va_start(ap, message);
    fprintf(stderr, "%s %d.%d-%d.%d: ",
            getFilename().c_str(),
            yylloc.first_line, yylloc.first_column,
            yylloc.last_line, yylloc.last_column);
    vfprintf(stderr, message, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

int input(int fd,
          bool isystem,
          bool useExceptions,
          const char* stringTypeName)
{
    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 0;
    try
    {
        yyin = fdopen(fd, "r");
        if (yyparse() != 0)
        {
            return EXIT_FAILURE;
        }
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int output(const char* filename,
           bool isystem,
           bool useExceptions,
           const char* stringTypeName,
           const char* indent,
           bool skeleton,
           bool generic)
{
    Forward forward(filename);
    getSpecification()->accept(&forward);
    forward.generateForwardDeclarations();

    printCxx(filename, stringTypeName, useExceptions, indent);
    if (skeleton)
    {
        printSkeleton(filename, isystem, indent);
    }
    if (generic)
    {
        printTemplate(filename, stringTypeName, useExceptions, isystem, indent);
    }
    return EXIT_SUCCESS;
}
