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

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "esidl.h"
#include "parser.h"

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
    char  filename[PATH_MAX + 1];
    char* includePath;
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

char* getFilename()
{
    return filename;
}

void setFilename(const char* name)
{
    if (*name == '"')
    {
        size_t len = strlen(name) - 2;
        assert(len < PATH_MAX);
        memmove(filename, name + 1, len);
        filename[len] = '\0';
    }
    else
    {
        strncpy(filename, name, PATH_MAX);
    }
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

std::string getOutputFilename(const char* input, const char* suffix)
{
    std::string filename(input);

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

    if (includePath)
    {
        int pos = filename.find(includePath);
        if (pos == 0)
        {
            filename.replace(pos, strlen(includePath) + 1, "");
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
#ifdef USE_FUNCTION_CALLBACK
        int callbackStage = 0;
        int callbackCount;
        do
        {
            callbackCount = 0;
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

                Node* spec = param->getSpec();
                if (spec->isInterface(this) && dynamic_cast<ScopedName*>(spec))  // spec can be 'Object'
                {
                    Interface* callback = dynamic_cast<Interface*>(dynamic_cast<ScopedName*>(spec)->search(getParent()));
                    if (callback && callback->isCallback() == Interface::Callback)
                    {
                        ++callbackCount;
                    }
                }
            }
            paramCounts.push_back(paramCount);
            ++callbackStage;
        } while (callbackStage < (1u << callbackCount));
#else  // USE_FUNCTION_CALLBACK
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
#endif  // USE_FUNCTION_CALLBACK
        ++optionalStage;
    } while (optionalStage <= optionalCount);
    Interface* interface = dynamic_cast<Interface*>(getParent());
    assert(interface);
    interface->addMethodCount(methodCount - 1);
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
        if (ext->getName() == "NoIndexingOperations")
        {
            attr |= NoIndexingOperations;
        }
        else if (ext->getName() == "Callback")
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
        else if (ext->getName() == "ImplementedOn")
        {
            if (ScopedName* scopedName = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                Node* on = scopedName->search(getParent());
                if (Interface* interface = dynamic_cast<Interface*>(on))
                {
                    implementedOn.push_back(interface);
                    interface->implementedOn.push_back(this);
                }
            }
        }
        else if (ext->getName() == "Callable")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                assert(callable == "");
                callable = name->getName();
            }
        }
        else if (ext->getName() == "Stringifies")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                assert(stringifies == "");
                stringifies = name->getName();
            }
            else
            {
                // TODO: Add custom toString() attribute
            }
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
    for (NodeList::iterator i = list->begin(); i != list->end(); ++i)
    {
        ExtendedAttribute* ext = dynamic_cast<ExtendedAttribute*>(*i);
        assert(ext);
        if (ext->getName() == "Replaceable")
        {
            attr |= Replaceable;
        }
        else if (ext->getName() == "Null")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "Empty")
                {
                    attr |= NullIsEmpty;
                }
                else if (name->getName() == "Null")
                {
                    attr |= NullIsNull;
                }
            }
        }
        else if (ext->getName() == "Undefined")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "Empty")
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
}

void OpDcl::processExtendedAttributes()
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
        if (ext->getName() == "IndexCreator")
        {
            attr |= IndexCreator;
        }
        else if (ext->getName() == "IndexDeleter")
        {
            attr |= IndexDeleter;
        }
        else if (ext->getName() == "IndexGetter")
        {
            attr |= IndexGetter;
            check(getParamCount() == 1,
                  "[IndexGetter] MUST take a single argument.");
            check(dynamic_cast<ParamDcl*>(*(begin()))->getSpec()->getName() == "unsigned long",
                  "[IndexGetter] MUST take a single argument of type unsigned long.");
        }
        else if (ext->getName() == "IndexSetter")
        {
            attr |= IndexSetter;
        }
        else if (ext->getName() == "NameCreator")
        {
            attr |= NameCreator;
        }
        else if (ext->getName() == "NameDeleter")
        {
            attr |= NameDeleter;
        }
        else if (ext->getName() == "NameGetter")
        {
            attr |= NameGetter;
        }
        else if (ext->getName() == "NameSetter")
        {
            attr |= NameSetter;
        }
        else if (ext->getName() == "Null")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "Empty")
                {
                    attr |= NullIsEmpty;
                }
                else if (name->getName() == "Null")
                {
                    attr |= NullIsNull;
                }
            }
        }
        else if (ext->getName() == "Undefined")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "Empty")
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
}

void ParamDcl::processExtendedAttributes()
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
        if (ext->getName() == "Optional")
        {
            attr |= Optional;
        }
        else if (ext->getName() == "Variadic")
        {
            attr |= Variadic;
        }
        else if (ext->getName() == "Null")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "Empty")
                {
                    attr |= NullIsEmpty;
                }
                else if (name->getName() == "Null")
                {
                    attr |= NullIsNull;
                }
            }
        }
        else if (ext->getName() == "Undefined")
        {
            if (ScopedName* name = dynamic_cast<ScopedName*>(ext->getDetails()))
            {
                if (name->getName() == "Empty")
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
}

Node* ScopedName::search(const Node* scope) const
{
    Node* resolved = resolve(scope, name);
    if (resolved)
    {
        if (Member* member = dynamic_cast<Member*>(resolved))
        {
            if (member->isTypedef() && !member->isArray(member->getParent()))
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
    return included + "_INCLUDED";
}

void Interface::processExtendedAttributes(OpDcl* op)
{
    attr |= (op->getAttr() & (IndexMask | NameMask));
    ++methodCount;
    if (callable == op->getName())
    {
        op->setCallable();
    }
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
    if (stringifies == attr->getName())
    {
        attr->setStringifies();
    }
}

class ProcessExtendedAttributes : public Visitor
{
public:
    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const Interface* node)
    {
        const_cast<Interface*>(node)->processExtendedAttributes();
        if (node->isLeaf())
        {
            return;
        }
        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            (*i)->accept(this);
            if (OpDcl* op = dynamic_cast<OpDcl*>(*i))
            {
                const_cast<Interface*>(node)->processExtendedAttributes(op);
            }
            else if (Attribute* attr = dynamic_cast<Attribute*>(*i))
            {
                const_cast<Interface*>(node)->processExtendedAttributes(attr);
            }
        }
    }

    virtual void at(const Attribute* node)
    {
        const_cast<Attribute*>(node)->processExtendedAttributes();
        visitChildren(node);
    }

    virtual void at(const OpDcl* node)
    {
        const_cast<OpDcl*>(node)->processExtendedAttributes();
        visitChildren(node);
    }

    virtual void at(const ParamDcl* node)
    {
        const_cast<ParamDcl*>(node)->processExtendedAttributes();
        visitChildren(node);
    }
};

class AdjustMethodCount : public Visitor
{
public:
    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const OpDcl* node)
    {
        const_cast<OpDcl*>(node)->adjustMethodCount();
    }
};

void yyerror(const char* message, ...)
{
    va_list ap;

    va_start(ap, message);
    fprintf(stderr, "%d.%d-%d.%d: ",
            yylloc.first_line, yylloc.first_column,
            yylloc.last_line, yylloc.last_column);
    vfprintf(stderr, message, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

int main(int argc, char* argv[])
{
    bool ent = false;
    bool npapi = false;
    bool isystem = false;
    bool useExceptions = true;
    const char* stringTypeName = "char*";   // C++ string type name to be used

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'I')
            {
                if (argv[i][2])
                {
                    includePath = &argv[i][2];
                }
                else
                {
                    ++i;
                    includePath = argv[i];
                }
            }
            else if (strcmp(argv[i], "-isystem") == 0)
            {
                ++i;
                includePath = argv[i];
                isystem = true;
            }
            else if (strcmp(argv[i], "-debug") == 0)
            {
                bool stop = true;
                while (stop)
                {
                    fprintf(stderr, ".");
                    sleep(1);
                }
            }
            else if (strcmp(argv[i], "-ent") == 0)
            {
                ent = true;
            }
            else if (strcmp(argv[i], "-npapi") == 0)
            {
                npapi = true;
            }
            else if (strcmp(argv[i], "-fexceptions") == 0)
            {
                useExceptions = true;
            }
            else if (strcmp(argv[i], "-fno-exceptions") == 0)
            {
                useExceptions = false;
            }
            else if (strcmp(argv[i], "-namespace") == 0)
            {
                ++i;
                Node::setFlatNamespace(argv[i]);
            }
            else if (strcmp(argv[i], "-object") == 0)
            {
                ++i;
                Node::setBaseObjectName(argv[i]);
            }
            else if (strcmp(argv[i], "-string") == 0)
            {
                ++i;
                stringTypeName = argv[i];
            }
        }
    }

    Module* node = new Module("");
    setSpecification(node);
    setCurrent(node);

    if (strcmp(Node::getBaseObjectName(), "::Object") == 0)
    {
        // Manually install 'Object' interface forward declaration.
        Interface* object = new Interface("Object", 0, true);
        object->setRank(2);
        getCurrent()->add(object);
    }

    if (Node::getFlatNamespace())
    {
        Module* module = new Module(Node::getFlatNamespace());
        getCurrent()->add(module);
        setCurrent(module);
    }

    yylloc.first_line = yylloc.last_line = 1;
    yylloc.first_column = yylloc.last_column = 0;
    try
    {
        yyin = stdin;
        if (yyparse() != 0)
        {
            return EXIT_FAILURE;
        }
        fclose(yyin);
    }
    catch (...)
    {
        return EXIT_FAILURE;
    }

    if (Node::getFlatNamespace())
    {
        setCurrent(getCurrent()->getParent());
    }

    ProcessExtendedAttributes processExtendedAttributes;
    getSpecification()->accept(&processExtendedAttributes);

    AdjustMethodCount adjustMethodCount;
    getSpecification()->accept(&adjustMethodCount);

#ifdef VERBOSE
    printf("-I %s\n", includePath);
    printf("-----------------------------------\n");
    print();
#endif
    printf("-----------------------------------\n");
    printCxx(getOutputFilename(getFilename(), "h"), stringTypeName, useExceptions);
    printf("-----------------------------------\n");
    if (ent)
    {
        printEnt(getOutputFilename(getFilename(), "ent"));
        printf("-----------------------------------\n");
    }
    if (npapi)
    {
        printNpapi(getFilename(), isystem);
        printf("-----------------------------------\n");
    }

    delete node;

    return EXIT_SUCCESS;
}

void printNpapi(const char* idlFilename, bool isystem) __attribute__ ((weak));
void printNpapi(const char* idlFilename, bool isystem)
{
}
