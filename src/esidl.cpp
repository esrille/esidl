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
#include <iostream>
#include <sstream>

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
const char* Node::baseObjectName = "::object";
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
    size_t begin = filename.rfind(".");
    size_t end = filename.size();
    if (begin != std::string::npos)
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
        size_t pos = filename.find(getIncludePath());
        if (pos == 0)
        {
            filename.replace(pos, strlen(getIncludePath()) + 1, "");
        }
        else
        {
            size_t slash = filename.rfind("/");
            if (slash != std::string::npos)
            {
                filename.replace(0, slash + 1, "");
            }
        }
    }

    std::string dir;
    std::string path(filename);
    for (;;)
    {
        size_t slash = path.find("/");
        if (slash == std::string::npos)
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

std::string Node::getQualifiedModuleName() const
{
    const Node* node = this;
    while (node)
    {
        if (dynamic_cast<const Module*>(node))
        {
            return node->getQualifiedName();
        }
        node = node->getParent();
    }
    return "";
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

void Implements::resolve(bool importImplements)
{
    Interface* interface = dynamic_cast<Interface*>(getFirst()->search(getParent()));
    Interface* mixin = dynamic_cast<Interface*>(getSecond()->search(getParent()));
    getFirst()->check(interface, "could not resolve '%s'.", getFirst()->getName().c_str());
    getSecond()->check(mixin, "could not resolve '%s'.", getSecond()->getName().c_str());
    interface->implements(mixin, importImplements);
}

void Module::processExtendedAttributes()
{
    NodeList* list = getExtendedAttributes();
    if (!list)
    {
        return;
    }

    for (NodeList::iterator i = list->begin(); i != list->end(); ++i)
    {
        ExtendedAttribute* ext = dynamic_cast<ExtendedAttribute*>(*i);
        assert(ext);
        if (ext->getName() == "Prefix")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                prefix = name->getName();
            }
        }
        else if (ext->getName() == "ExceptionConsts")
        {
            ext->report("Warning: '%s' has been deprecated.", ext->getName().c_str());
        }
    }
}

void Interface::processExtendedAttributes()
{
    static unsigned supplementalCount = 0;

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
                ScopedName* name = new ScopedName(getBaseObjectName());
                extends = new Node();
                extends->add(name);
                constructor = new Interface("Constructor", extends);
                constructor->setRank(getRank());
                constructor->setAttr(constructor->getAttr() | Interface::Constructor);
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
        else if (ext->getName() == "Supplemental")
        {
            attr |= Supplemental;
        }
        else if (ext->getName() == "NoIndexingOperations" ||
                 ext->getName() == "ImplementedOn" ||
                 ext->getName() == "Callable" ||
                 ext->getName() == "Stringifies")
        {
            ext->report("Warning: '%s' has been deprecated.", ext->getName().c_str());
        }
    }

    if ((attr & (Supplemental | NoInterfaceObject)) == Supplemental)
    {
        std::ostringstream oss;
        oss << name << '-' << ++supplementalCount;
        name = oss.str();
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
        else if (ext->getName() == "Null" ||
                 ext->getName() == "Undefined")
        {
            ext->report("Warning: '%s' has been deprecated.", ext->getName().c_str());
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
        else if (ext->getName() == "Null" ||
                 ext->getName() == "Undefined" ||
                 ext->getName() == "IndexCreator" ||
                 ext->getName() == "IndexDeleter" ||
                 ext->getName() == "IndexGetter" ||
                 ext->getName() == "IndexSetter" ||
                 ext->getName() == "NameCreator" ||
                 ext->getName() == "NameDeleter" ||
                 ext->getName() == "NameGetter" ||
                 ext->getName() == "NameSetter")
        {
            ext->report("Warning: '%s' has been deprecated.", ext->getName().c_str());
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
        else if (ext->getName() == "Null" ||
                 ext->getName() == "Undefined" ||
                 ext->getName() == "Optional" ||
                 ext->getName() == "Variadic")
        {
            ext->report("Warning: '%s' has been deprecated.", ext->getName().c_str());
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

Node* resolveInBase(const Interface* interface, std::string name)
{
    Node* extends = interface->getExtends();
    if (!extends)
    {
        return 0;
    }
    for (NodeList::iterator i = extends->begin();
         i != extends->end();
         ++i)
    {
        ScopedName* baseName = static_cast<ScopedName*>(*i);
        Interface* base = dynamic_cast<Interface*>(baseName->search(interface->getParent()));
        if (base)
        {
            if (base->getParent()->search(name) == base)
            {
                return base;
            }
            // Search the interface members to check method overrides.
            if (Node* found = base->search(name))
            {
                return found;
            }
            if (Node* found = resolveInBase(base, name))
            {
                return found;
            }
        }
    }
    return 0;
 }

Node* resolve(const Node* scope, std::string name)
{
    if (name.compare(0, 2, "::") == 0)
    {
        return getSpecification()->search(name, 2);
    }

    for (const Node* node = scope; node; node = node->getParent())
    {
        if (Node* found = node->search(name))
        {
            return found;
        }
        if (const Interface* base = dynamic_cast<const Interface*>(node))
        {
            if (Node* found = resolveInBase(base, name))
            {
                return found;
            }
        }
    }
    return 0;
}

Member* ScopedName::isTypedef(const Node* scope) const
{
    Node* resolved = resolve(scope, name);
    if (!resolved)
    {
        return 0;
    }
    if (Member* member = dynamic_cast<Member*>(resolved))
    {
        return member->isTypedef(member->getParent());
    }
    return 0;
}

Node* ScopedName::search(const Node* scope) const
{
    Node* resolved = resolve(scope, name);
    if (resolved)
    {
        if (Member* member = dynamic_cast<Member*>(resolved))
        {
            if (member->isTypedef(member->getParent()))
            {
                resolved = member->getSpec();
                if (ScopedName* node = dynamic_cast<ScopedName*>(resolved))
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

// This method works only after AdjustMethodCount visitor has been applied.
int Interface::getInterfaceCount() const
{
    if (interfaceCount != 0)
    {
        return interfaceCount;
    }

    interfaceCount = 1;
    for (std::list<const Interface*>::const_iterator i = superList.begin();
         i != superList.end();
         ++i)
    {
        if (!(*i)->isBaseObject())
        {
            interfaceCount += (*i)->getInterfaceCount();
        }
    }
    for (std::list<const Interface*>::const_reverse_iterator i = implementList.rbegin();
            i != implementList.rend();
            ++i)
    {
        interfaceCount += (*i)->getInterfaceCount();
    }
    return interfaceCount;
}

void Interface::getInterfaceList(std::list<const Interface*>* list) const
{
    for (std::list<const Interface*>::const_reverse_iterator i = implementList.rbegin();
         i != implementList.rend();
         ++i)
    {
        if (!((*i)->getAttr() & Interface::Supplemental))
        {
            (*i)->getInterfaceList(list);
        }
    }
    for (std::list<const Interface*>::const_iterator i = superList.begin();
         i != superList.end();
         ++i)
    {
        if (!((*i)->getAttr() & Interface::Supplemental) && !(*i)->isBaseObject())
        {
            (*i)->getInterfaceList(list);
        }
    }
    list->push_front(this);
}

void Interface::adjustMethodCount()
{
    if (extends)
    {
        for (NodeList::iterator i = extends->begin(); i != extends->end(); ++i)
        {
            Interface* super = dynamic_cast<Interface*>(static_cast<ScopedName*>(*i)->search(getParent()));
            check(super, "could not resolve '%s'.", name.substr(0, name.rfind('-')).c_str());
            superList.push_back(super);
        }
    }

    switch (attr & (Supplemental | NoInterfaceObject))
    {
    case Supplemental:
    {
        ScopedName* org = new ScopedName(name.substr(0, name.rfind('-')));
        Interface* supplemental = dynamic_cast<Interface*>(org->search(getParent()));
        check(supplemental, "could not resolve '%s'.", name.substr(0, name.rfind('-')).c_str());
        supplemental->implements(this, true);
        break;
    }
    case Supplemental | NoInterfaceObject:
        if (extends)
        {
            for (NodeList::iterator i = extends->begin(); i != extends->end(); ++i)
            {
                Interface* supplemental = dynamic_cast<Interface*>(static_cast<ScopedName*>(*i)->search(getParent()));
                supplemental->implements(this, true);
            }
        }
        break;
    default:
        break;
    }
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

unsigned SequenceType::getLength(const Node* scope)
{
    EvalInteger<unsigned> eval(scope);

    if (!max)
    {
        return 0;
    }
    max->accept(&eval);
    return eval.getValue();
}

unsigned ArrayType::getLength(const Node* scope)
{
    EvalInteger<unsigned> eval(scope);

    if (!max)
    {
        return 0;
    }
    max->accept(&eval);
    return eval.getValue();
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
           bool useVirtualBase,
           const char* stringTypeName,
           const char* objectTypeName,
           const char* indent,
           bool skeleton,
           bool generic,
           bool java,
           bool cplusplus)
{
    Forward forward(filename);
    getSpecification()->accept(&forward);
    forward.generateForwardDeclarations();

    if (java)
    {
        printJava(filename, indent);
    }
    else if (cplusplus)
    {
        printCPlusPlus(filename, stringTypeName, objectTypeName, useExceptions, useVirtualBase, indent);
    }
    else
    {
        printCxx(filename, stringTypeName, objectTypeName, useExceptions, useVirtualBase, indent);
        if (skeleton)
        {
            printSkeleton(filename, isystem, indent);
        }
        if (generic)
        {
            printTemplate(filename, stringTypeName, objectTypeName, useExceptions, isystem, indent);
        }
    }
    return EXIT_SUCCESS;
}
