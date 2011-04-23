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

#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <set>
#include <vector>
#include "messenger.h"
#include "messengerDispatch.h"
#include "messengerInvoke.h"
#include "messengerMeta.h"

bool Messenger::useVirtualBase = false;

namespace
{

std::string createFileName(const std::string prefixedName, const std::string objectTypeName, const char* ext = ".h")
{
    std::string filename = prefixedName;
    if (filename == Node::getBaseObjectName())
    {
        filename.replace(2, filename.length(), objectTypeName);
    }
    size_t pos = 0;
    for (;;)
    {
        pos = filename.find("::", pos);
        if (pos == std::string::npos)
        {
            break;
        }
        filename.replace(pos, 2, "/");
    }
    if (filename[0] == '/')
    {
        filename = filename.substr(1);
    }
    return filename + ext;
}

FILE* createFile(const std::string prefixedName, const std::string objectTypeName, const char* ext = ".h")
{
    std::string filename = createFileName(prefixedName, objectTypeName, ext);
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
    printf("# %s\n", filename.c_str());
    return fopen(filename.c_str(), "w");
}

}  // namespace

std::string Messenger::getEscapedName(std::string name)
{
    static const char* reservedWords[] =
    {
        "asm",
        "auto",
        "bool",
        "break",
        "case",
        "catch",
        "char",
        "class",
        "const",
        "const_cast",
        "continue",
        "default",
        "delete",
        "do",
        "double",
        "dynamic_cast",
        "else",
        "enum",
        "explicit",
        "false",
        "float",
        "for",
        "friend",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "mutable",
        "namespace",
        "new",
        "operator",
        "private",
        "protected",
        "public",
        "register",
        "reinterpret_cast",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "static_cast",
        "struct",
        "switch",
        "template",
        "this",
        "throw",
        "true",
        "try",
        "typedef",
        "typeid",
        "typename",
        "union",
        "unsigned",
        "virtual",
        "void",
        "volatile",
        "wchar_t",
        "while",
    };
    const char** p = find(reservedWords, reservedWords + sizeof reservedWords / sizeof reservedWords[0], name);
    if (p == &reservedWords[sizeof reservedWords / sizeof reservedWords[0]])
    {
        return name;
    }
    return "_" + name;  // TODO: Should not use the '_' prefix in C++
}

class MessengerInterface : public Messenger
{
public:
    MessengerInterface(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        Messenger(file, stringTypeName, objectTypeName, useExceptions, indent)
    {
        currentNode = 0;
    }

    MessengerInterface(const Formatter* formatter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        Messenger(formatter, stringTypeName, objectTypeName, useExceptions)
    {
        currentNode = 0;
    }

    virtual void at(const ExceptDcl* node)
    {
        if (!currentNode)
        {
            currentNode = node->getParent();
            prefixedModuleName = currentNode->getPrefixedModuleName();
        }

        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("struct %s {\n", getEscapedName(node->getName()).c_str());
            writeln("%s(unsigned short code) : code(code) {", getEscapedName(node->getName()).c_str());
            writeln("}");
            printChildren(node);
        writeln("};");
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
            prefixedModuleName = currentNode->getPrefixedModuleName();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

        write("class %s", getEscapedName(getClassName(node)).c_str());

        if (node->getExtends())
        {
            const char* separator = " : public ";
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                write(separator);
                separator = ", public ";
                if (useVirtualBase && (*i)->isBaseObject())
                {
                    write("virtual ");
                }
                (*i)->accept(this);
            }
        }
        write(" {\n");
        unindent();
        writeln("public:");
        indent();

        // Expand supplementals
        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            writeln("// %s", (*i)->getName().c_str());
            const Node* saved = currentNode;
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                currentNode = *i;
                visitInterfaceElement(*i, *j);
            }
            currentNode = saved;
        }

#ifdef USE_CONSTRUCTOR
        if (constructor)
        {
            writeln("");
            constructorMode = true;
            writeln("// [Constructor]");
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(constructor, *i);
            }
            constructorMode = false;
            writeln("");
        }
#endif

        writetab();
        write("%s(Object* object)", getEscapedName(getClassName(node)).c_str());
        if (node->getExtends())
        {
            write(" :\n");
            indent();
            writetab();

            const char* separator = "";
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                write(separator);
                (*i)->accept(this);
                write("(object)");
                separator = ", ";
            }
            write(" ");
            unindent();
            write("{\n");
        }
        writeln("}");

        // copy constructor
        writetab();
        write("%s(const %s& object)", getEscapedName(getClassName(node)).c_str(), getEscapedName(getClassName(node)).c_str());
        if (node->getExtends())
        {
            write(" :\n");
            indent();
            writetab();

            const char* separator = "";
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                write(separator);
                (*i)->accept(this);
                write("(object)");
                separator = ", ";
            }
            write(" ");
            unindent();
            write("{\n");
        }
        writeln("}");

        // assignment
        writeln("%s& operator=(const %s& object) {", getEscapedName(getClassName(node)).c_str(), getEscapedName(getClassName(node)).c_str());
        if (node->getExtends())
        {
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                writetab();
                (*i)->accept(this);
                write("::operator =(object);\n");
            }
        }
            writeln("return *this;");
        writeln("}");

        writeln("");
        MessengerDispatch dispatch(this, stringTypeName, objectTypeName, useExceptions);
        dispatch.at(node);

        if (constructor)
        {
            writeln("");
            writeln("static Object getConstructor();");
        }

        MessengerMeta meta(this, stringTypeName, objectTypeName, useExceptions);
        const_cast<Interface*>(node)->accept(&meta);

        writeln("};");
    }

    virtual void at(const BinaryExpr* node)
    {
        node->getLeft()->accept(this);
        write(" %s ", node->getName().c_str());
        node->getRight()->accept(this);
    }

    virtual void at(const UnaryExpr* node)
    {
        write("%s", node->getName().c_str());
        NodeList::iterator elem = node->begin();
        (*elem)->accept(this);
    }

    virtual void at(const GroupingExpression* node)
    {
        write("(");
        NodeList::iterator elem = node->begin();
        (*elem)->accept(this);
        write(")");
    }

    virtual void at(const Literal* node)
    {
        write("%s", node->getName().c_str());
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef(node->getParent()))
        {
            Node* spec = node->getSpec();
            if (dynamic_cast<ScopedName*>(spec))
                return;
            write("typedef ");
        }
        // This node is an exception class member.
        if (!currentNode)
        {
            currentNode = node->getParent();
            prefixedModuleName = currentNode->getPrefixedModuleName();
        }
        writetab();
        node->getSpec()->accept(this);
        write(" %s;\n", getEscapedName(node->getName()).c_str());
    }

    virtual void at(const ConstDcl* node)
    {
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("static const ");
        node->getSpec()->accept(this);
        write(" %s = ", getEscapedName(node->getName()).c_str());
        node->getExp()->accept(this);
        write(";\n");
    }

    virtual void at(const Attribute* node)
    {
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

        // getter
        Messenger::getter(node);
        write(";\n");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            writetab();
            Messenger::setter(node);
            write(";\n");
        }
    }

    virtual void at(const OpDcl* node)
    {
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        Messenger::at(node);
        write(";\n");
    }
};

class MessengerInclude: public Visitor, public Formatter
{
    const Node* currentNode;
    std::string objectTypeName;
    bool useExceptions;
    std::set<const Node*> includeSet;
    bool overloaded;

public:
    MessengerInclude(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        Formatter(file, indent),
        currentNode(0),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        overloaded(false)
    {
    }

    MessengerInclude(const Formatter* formatter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        Formatter(formatter),
        currentNode(0),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        overloaded(false)
    {
    }

    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const ScopedName* node)
    {
        assert(currentNode);

        Node* resolved = node->search(currentNode);
        node->check(resolved, "could not resolved %s.", node->getName().c_str());
        const Node* saved = currentNode;
        if (resolved->getParent())
        {
            currentNode = resolved->getParent();
        }
        if (!dynamic_cast<Interface*>(resolved))
        {
            resolved->accept(this);
        }
        else if (!resolved->isBaseObject())
        {
            includeSet.insert(resolved);
        }
        currentNode = saved;
    }

    virtual void at(const ExceptDcl* node)
    {
        includeSet.insert(node);
    }

    virtual void at(const SequenceType* node)
    {
        node->getSpec()->accept(this);
    }

    virtual void at(const ArrayType* node)
    {
        node->getSpec()->accept(this);
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

        if (node->getExtends())
        {
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                ScopedName* name = static_cast<ScopedName*>(*i);
                if (name->getName() == Node::getBaseObjectName())
                {
                    // Do not process 'object'.
                    continue;
                }
                Node* base = name->search(node->getParent());
                node->check(base, "could not resolved %s.", name->getName().c_str());
                includeSet.insert(base);
            }
        }

        // Include mixins for the implementation class
        const std::list<const Interface*>* implementList = node->getImplements();
        if (!implementList->empty())
        {
            for (std::list<const Interface*>::const_iterator i = implementList->begin();
                 i != implementList->end();
                 ++i)
            {
                includeSet.insert(*i);
            }
        }

        std::multimap<uint32_t, OpDcl*> operations;

        // Expand supplementals
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
                if (OpDcl* op = dynamic_cast<OpDcl*>(*j))
                {
                    if (!(op->getAttr() & OpDcl::UnnamedProperty))
                        operations.insert(std::pair<uint32_t, OpDcl*>(op->getHash(), op));
                    if (op->getAttr() & (OpDcl::UnnamedProperty | OpDcl::Omittable | OpDcl::Caller))
                        operations.insert(std::pair<uint32_t, OpDcl*>(0, op));
                    continue;
                }
                (*j)->accept(this);
            }
            currentNode = saved;
        }

        for (std::multimap<uint32_t, OpDcl*>::iterator i = operations.begin(); i != operations.end(); ++i)
        {
            int count = operations.count(i->first);
            if (count == 1)
            {
                // Process non-overloaded operations
                overloaded = false;
                const Node* saved = currentNode;
                currentNode = i->second->getParent();
                (i->second)->accept(this);
                currentNode = saved;
            }
            else
            {
                // Process overloaded operations
                overloaded = true;
                for (; 0 < count; --count, ++i) {
                    OpDcl* op = i->second;
                    const Node* saved = currentNode;
                    currentNode = op->getParent();
                    op->accept(this);
                    currentNode = saved;
                };
                --i;
            }
        }

#ifdef USE_CONSTRUCTOR
        visitChildren(constructor);
#endif
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef(node->getParent()))
        {
            node->getSpec()->accept(this);
        }
    }

    virtual void at(const Attribute* node)
    {
        // getter
        if (useExceptions && node->getGetRaises())
        {
            visitChildren(node->getGetRaises());
        }
        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            if (useExceptions && node->getSetRaises())
            {
                visitChildren(node->getSetRaises());
            }
        }
    }

    virtual void at(const OpDcl* node)
    {
        if (Node* spec = node->hasCovariantReturnType())
        {
            // Include the interface definition of the return type
            // to represent it is derived from the return type of
            // the operation in an ancestor.
            // cf. namedItem() in HTMLPropertiesCollection
            includeSet.insert(spec);
        }
        if (overloaded)
        {
            visitChildren(node);
        }
        if (useExceptions && node->getRaises())
        {
            visitChildren(node->getRaises());
        }
    }

    virtual void at(const ParamDcl* node)
    {
        node->getSpec()->accept(this);
    }

    void print()
    {
        if (includeSet.empty())
        {
            return;
        }
        for (std::set<const Node*>::iterator i = includeSet.begin();
             i != includeSet.end();
             ++i)
        {
            std::string name = createFileName((*i)->getPrefixedName(), objectTypeName);
            writeln("#include <%s>", name.c_str());
        }
        write("\n");
    }
};

class MessengerNameSpace : public Formatter
{
    std::vector<std::string> packageName;

public:
    MessengerNameSpace(FILE* file, const std::string& indent) :
        Formatter(file, indent)
    {
    }

    size_t enter(std::string name)
    {
        name = name.substr(2);

        std::vector<std::string> prevName = packageName;
        packageName.clear();

        size_t pos = name.rfind("::");
        if (pos == std::string::npos)
        {
            pos = 0;
        }
        else
        {
            size_t start = 0;
            size_t dot;
            while ((dot = name.find("::", start)) != std::string::npos)
            {
                packageName.push_back(name.substr(start, dot - start));
                start = dot + 2;
            }
            pos += 2;
        }

        int i;
        for (i = 0;
             i < prevName.size() && i < packageName.size() && prevName[i] == packageName[i];
             ++i)
        {
        }
        for (int j = i; j < prevName.size(); ++j)
        {
            writeln("}");
        }
        for (int j = i; j < packageName.size(); ++j)
        {
            writeln("namespace %s {", Messenger::getEscapedName(packageName[j]).c_str());
        }

        return pos + 2;
    }

    void closeAll()
    {
        if (0 < packageName.size())
        {
            write("\n");
            for (int i = 0; i < packageName.size(); ++i)
            {
                writeln("}");
            }
        }
        packageName.clear();
    }
};

// Print the required class forward declarations and typedef statements.
class MessengerImport : public Visitor, public Formatter
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;

    const Node* currentNode;
    bool printed;
    std::set<const Node*> importSet;
    std::list<Member*> typedefList;
    std::set<Member*> typedefSet;
    bool importObjectArray;

    MessengerNameSpace* ns;

public:
    MessengerImport(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent, MessengerNameSpace* ns) :
        Formatter(file, indent),
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        currentNode(0),
        importObjectArray(false),
        ns(ns)
    {
    }

    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const ScopedName* node)
    {
        assert(currentNode);

        Node* resolved = node->search(currentNode);
        node->check(resolved, "could not resolved %s.", node->getName().c_str());
        const Node* saved = currentNode;
        if (resolved->getParent())
        {
            currentNode = resolved->getParent();
        }
        if (!dynamic_cast<Interface*>(resolved) && !dynamic_cast<ExceptDcl*>(resolved))
        {
            resolved->accept(this);
        }
        else if (!resolved->isBaseObject())
        {
            importSet.insert(resolved);
        }
        currentNode = saved;

        if (Member* type = node->isTypedef(currentNode))
        {
            // The order of the definitions needs to be preserved.
            currentNode = type->getParent();
            type->getSpec()->accept(this);
            if (typedefSet.find(type) == typedefSet.end())
            {
                typedefSet.insert(type);
                typedefList.push_back(type);
            }
            currentNode = saved;
        }
    }

    virtual void at(const SequenceType* node)
    {
        Node* spec = node->getSpec();
        spec->accept(this);
    }

    virtual void at(const ArrayType* node)
    {
        importObjectArray = true;
        node->getSpec()->accept(this);
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef(node->getParent()))
        {
            node->getSpec()->accept(this);
        }
    }

    virtual void at(const Interface* node)
    {
        if (currentNode)
        {
            return;
        }
        currentNode = node->getParent();
        visitChildren(node->getExtends());
        currentNode = node;
#ifdef USE_CONSTRUCTOR
        if (node->getAttr() & Interface::Constructor)
        {
            currentNode = node->getParent();
        }
#endif
        // Expand supplementals
        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            currentNode = *i;
            visitChildren(*i);
        }
        currentNode = node->getParent();

#ifdef USE_CONSTRUCTOR
        if (Interface* constructor = node->getConstructor())
        {
            visitChildren(constructor);
        }
#endif
    }

    virtual void at(const Attribute* node)
    {
        node->getSpec()->accept(this);
        visitChildren(node->getGetRaises());
        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            visitChildren(node->getSetRaises());
        }
    }

    virtual void at(const OpDcl* node)
    {
        node->getSpec()->accept(this);
        visitChildren(node);
        visitChildren(node->getRaises());
    }

    virtual void at(const ParamDcl* node)
    {
        node->getSpec()->accept(this);
    }

    virtual void at(const ConstDcl* node)
    {
        node->getSpec()->accept(this);
    }

    void print()
    {
        bool newline = false;

        if (importObjectArray)
        {
            writeln("#include <org/w3c/dom/ObjectArray.h>");
            writeln("");
            newline = true;
        }

        for (std::set<const Node*>::iterator i = importSet.begin();
             i != importSet.end();
             ++i)
        {
            if (Module* module = dynamic_cast<Module*>((*i)->getParent()))
            {
                std::string name = module->getPrefixedName() + "::" + (*i)->getName();
                size_t pos = ns->enter(name);
                writeln("class %s;", name.substr(pos).c_str());
                newline = true;
            }
        }

        for (std::list<Member*>::iterator i = typedefList.begin();
             i != typedefList.end();
             ++i)
        {
            if (dynamic_cast<Module*>((*i)->getParent()))
            {
                size_t pos = ns->enter((*i)->getPrefixedName());
                MessengerInterface messengerInterface(this, stringTypeName, objectTypeName, useExceptions);
                messengerInterface.at(*i);
                newline = true;
            }
        }

        if (newline)
        {
            write("\n");
        }
    }

    void printExtra()
    {
        if (!importSet.empty())
        {
            write("\n");
        }
        for (std::set<const Node*>::iterator i = importSet.begin();
             i != importSet.end();
             ++i)
        {
            std::string name = createFileName((*i)->getPrefixedName(), objectTypeName);
            writeln("#include <%s>", name.c_str());
        }
    }
};

class MessengerVisitor : public Visitor
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;
    std::string indent;

public:
    MessengerVisitor(const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        indent(indent)
    {
    }

    virtual void at(const Node* node)
    {
        if (1 < node->getRank())
        {
            return;
        }
        visitChildren(node);
    }

    virtual void at(const ExceptDcl* node)
    {
        if (1 < node->getRank() || node->isLeaf())
        {
            return;
        }

        std::string prefixedName = node->getPrefixedName();
        FILE* file = createFile(prefixedName, objectTypeName);
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esaidl (r%s).\n\n", SVN_REVISION);

        std::string included = Messenger::getIncludedName(createFileName(prefixedName, objectTypeName), indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        // body
        MessengerNameSpace ns(file, indent);
        ns.enter(prefixedName);

        MessengerInterface messengerInterface(file, stringTypeName, objectTypeName, useExceptions, indent);
        messengerInterface.at(node);

        ns.closeAll();

        // postable
        fprintf(file, "\n#endif  // %s\n", included.c_str());
        fclose(file);
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() ||
            (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

        std::string prefixedName = node->getPrefixedName();
        FILE* file = createFile(prefixedName, objectTypeName);
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esidl (r%s).\n\n", SVN_REVISION);
        std::string included = Messenger::getIncludedName(createFileName(prefixedName, objectTypeName), indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        if (node->getAttr() & OpDcl::HasCovariant)
        {
            fprintf(file, "#include <type_traits>\n");
        }

        // body

        // The base classes and exception classes need to be defined.
        MessengerInclude include(file, stringTypeName, objectTypeName, useExceptions, indent);
        include.at(node);
        include.print();

        MessengerNameSpace ns(file, indent);

        MessengerImport import(file, stringTypeName, objectTypeName, useExceptions, indent, &ns);
        import.at(node);
        import.print();
        fprintf(file, "\n");

        ns.enter(prefixedName);

        MessengerInterface messengerInterface(file, stringTypeName, objectTypeName, useExceptions, indent);
        messengerInterface.at(node);

        fprintf(file, "\n");

        ns.closeAll();

        // postable
        fprintf(file, "\n#endif  // %s\n", included.c_str());
        fclose(file);
    }
};

int printMessenger(const char* stringTypeName, const char* objectTypeName,
                   bool useExceptions, bool useVirtualBase, const char* indent)
{
    Messenger::useVirtualBase = useVirtualBase;
    MessengerVisitor visitor(stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);
    return 0;
}

class MessengerSrcVisitor : public Visitor
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;
    std::string indent;

public:
    MessengerSrcVisitor(const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        indent(indent)
    {
    }

    virtual void at(const Node* node)
    {
        if (1 < node->getRank())
        {
            return;
        }
        visitChildren(node);
    }

    virtual void at(const ExceptDcl* node)
    {
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() ||
            (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

        std::string prefixedName = node->getPrefixedName();
        FILE* file = createFile(prefixedName, objectTypeName, ".cpp");
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esidl (r%s).\n\n", SVN_REVISION);
        fprintf(file, "#ifdef HAVE_CONFIG_H\n");
        fprintf(file, "#include \"config.h\"\n");
        fprintf(file, "#endif\n\n");
        if (!node->isConstructor())
        {
            fprintf(file, "#include <%s>\n", createFileName(prefixedName, objectTypeName).c_str());
        }
        else
        {
            fprintf(file, "#include <%s>\n", createFileName(node->getParent()->getPrefixedName(), objectTypeName).c_str());
        }

        // body
        MessengerNameSpace ns(file, indent);

        MessengerImport import(file, stringTypeName, objectTypeName, useExceptions, indent, &ns);
        import.at(node);
        // Include imported, i.e., forward declared, class definitions here
        // so that template functions can be used without explicitly including
        // the other generated files.
        import.printExtra();
        fprintf(file, "\n");

        ns.enter(prefixedName);

        MessengerInvoke messengerInvoke(file, stringTypeName, objectTypeName, useExceptions, indent);
        messengerInvoke.at(node);

#ifdef USE_CONSTRUCTOR
        if (Interface* constructor = node->getConstructor())
        {
            MessengerInvoke messengerInvoke(file, stringTypeName, objectTypeName, useExceptions, indent);
            messengerInvoke.setConstructorMode(true);
            messengerInvoke.at(constructor);
        }
#endif
        ns.closeAll();

        // postable
        fclose(file);
    }
};

int printMessengerSrc(const char* stringTypeName, const char* objectTypeName,
                      bool useExceptions, bool useVirtualBase, const char* indent)
{
    Messenger::useVirtualBase = useVirtualBase;
    MessengerSrcVisitor visitor(stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);
    return 0;
}

class MessengerImp : public Messenger
{
public:
    MessengerImp(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        Messenger(file, stringTypeName, objectTypeName, useExceptions, indent)
    {
        currentNode = 0;
        targetModuleName = "::org::w3c::dom::bootstrap";
    }

    MessengerImp(const Formatter* formatter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        Messenger(formatter, stringTypeName, objectTypeName, useExceptions)
    {
        currentNode = 0;
        targetModuleName = "::org::w3c::dom::bootstrap";
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
            prefixedModuleName = currentNode->getPrefixedModuleName();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

        writetab();
        write("class %sImp : public ObjectMixin<%sImp",
              getEscapedName(getClassName(node)).c_str(),
              getEscapedName(getClassName(node)).c_str());

        if (node->getExtends())
        {
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                if (!(*i)->isBaseObject())
                {
                    Node* resolved = static_cast<ScopedName*>(*i)->searchCplusplus(node);
                    write(", %sImp", getEscapedName(resolved->getName()).c_str());
                }
            }
        }
        write("> {\n");
        unindent();
        writeln("public:");
        indent();

        // Expand supplementals
        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            writeln("// %s", (*i)->getName().c_str());
            const Node* saved = currentNode;
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                currentNode = *i;
                visitInterfaceElement(*i, *j);
            }
            currentNode = saved;
        }

        // Object
        writeln("// Object");
        writeln("virtual Any message_(uint32_t selector, const char* id, int argc, Any* argv) {");
            writeln("return %s::dispatch(this, selector, id, argc, argv);",
                    getScopedName(targetModuleName, getInterfaceName(node->getPrefixedName())).c_str());
        writeln("}");


        writeln("static const char* const getMetaData() {");
            writeln("return %s::getMetaData();",
                    getScopedName(targetModuleName, getInterfaceName(node->getPrefixedName())).c_str());
        writeln("}");

        writeln("};");
    }

    virtual void at(const Member* node)
    {
    }

    virtual void at(const Attribute* node)
    {
        writetab();

        // getter
        Messenger::getter(node);
        write(" __attribute__((weak));\n");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            writetab();
            Messenger::setter(node);
            write(" __attribute__((weak));\n");
        }
    }

    virtual void at(const OpDcl* node)
    {
        writetab();
        Messenger::at(node);
        write(" __attribute__((weak));\n");
    }
};

class MessengerImpVisitor : public Visitor
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;
    std::string indent;

public:
    MessengerImpVisitor(const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        indent(indent)
    {
    }

    virtual void at(const Node* node)
    {
        if (1 < node->getRank())
        {
            return;
        }
        visitChildren(node);
    }

    virtual void at(const ExceptDcl* node)
    {
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() ||
            (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

        std::string prefixedName("::org::w3c::dom::bootstrap::");
        prefixedName += node->getName() + "Imp";
        FILE* file = createFile(prefixedName, objectTypeName, ".h");
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esidl (r%s).\n", SVN_REVISION);
        fprintf(file, "// This file is expected to be modified for the Web IDL interface\n");
        fprintf(file, "// implementation.  Permission to use, copy, modify and distribute\n");
        fprintf(file, "// this file in any software license is hereby granted.\n\n");

        std::string included = Messenger::getIncludedName(createFileName(prefixedName, objectTypeName), indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        fprintf(file, "#ifdef HAVE_CONFIG_H\n");
        fprintf(file, "#include \"config.h\"\n");
        fprintf(file, "#endif\n\n");
        if (!node->isConstructor())
        {
            fprintf(file, "#include <%s>\n", createFileName(node->getPrefixedName(), objectTypeName).c_str());
        }
        else
        {
            fprintf(file, "#include <%s>\n", createFileName(node->getParent()->getPrefixedName(), objectTypeName).c_str());
        }

        if (node->getExtends())
        {
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                if (!(*i)->isBaseObject())
                {
                    Node* resolved = static_cast<ScopedName*>(*i)->searchCplusplus(node);
                    fprintf(file, "#include \"%sImp.h\"\n", resolved->getName().c_str());
                }
            }
        }

        // body
        MessengerNameSpace ns(file, indent);

        MessengerImport import(file, stringTypeName, objectTypeName, useExceptions, indent, &ns);
        import.at(node);
        // Include imported, i.e., forward declared, class definitions here
        // so that template functions can be used without explicitly including
        // the other generated files.
        import.printExtra();
        fprintf(file, "\n");

        ns.enter(prefixedName);

        MessengerImp messengerImp(file, stringTypeName, objectTypeName, useExceptions, indent);
        messengerImp.at(node);

        ns.closeAll();

        // postable
        fprintf(file, "\n#endif  // %s\n", included.c_str());
        fclose(file);
    }
};

int printMessengerImp(const char* stringTypeName, const char* objectTypeName,
                      bool useExceptions, bool useVirtualBase, const char* indent)
{
    Messenger::useVirtualBase = useVirtualBase;
    MessengerImpVisitor visitor(stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);
    return 0;
}

class MessengerImpSrc : public Messenger
{
    std::string className;

    void writeInvoke(const Node* node, Node* spec)
    {
        if (spec->isVoid(node))
        {
            return;
        }

        if (spec->isSequence(node))
        {
            writetab();
            write("return ");
            spec->accept(this);
            write("();\n");
        }
        else if (spec->isInterface(node))
        {
            writeln("return static_cast<Object*>(0);");
        }
        else if (spec->isString(node))
        {
            writeln("return u\"\";");
        }
        else
        {
            writeln("return 0;");
        }
    }

public:
    MessengerImpSrc(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        Messenger(file, stringTypeName, objectTypeName, useExceptions, indent)
    {
        currentNode = 0;
        targetModuleName = "::org::w3c::dom::bootstrap";
    }

    virtual void at(const Member* node)
    {
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
            prefixedModuleName = currentNode->getPrefixedModuleName();
            className = node->getName() + "Imp";
            if (constructorMode && (node->getAttr() & Interface::Constructor))
            {
                className = currentNode->getName();
            }
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());

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
        Node* spec = node->getSpec();
        if (node->isReplaceable())
        {
            spec = &replaceable;
        }

        writeln("");
        writetab();
        getter(node, className);
        writeln("{");
            writeln("// TODO: implement me!");
            writeInvoke(node, spec);
        writeln("}");

        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return;
        }

        // setter
        writeln("");
        writetab();
        setter(node, className);
        writeln("{");
            writeln("// TODO: implement me!");
        writeln("}");
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writeln("");
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
            writeln("// TODO: implement me!");
            writeInvoke(node, node->getSpec());
        writeln("}");
    }
};

class MessengerImpSrcVisitor : public Visitor
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;
    std::string indent;

public:
    MessengerImpSrcVisitor(const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        stringTypeName(stringTypeName),
        objectTypeName(objectTypeName),
        useExceptions(useExceptions),
        indent(indent)
    {
    }

    virtual void at(const Node* node)
    {
        if (1 < node->getRank())
        {
            return;
        }
        visitChildren(node);
    }

    virtual void at(const ExceptDcl* node)
    {
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() ||
            (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

        std::string prefixedName("::org::w3c::dom::bootstrap::");
        prefixedName += node->getName() + "Imp";
        FILE* file = createFile(prefixedName, objectTypeName, ".cpp");
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esidl (r%s).\n", SVN_REVISION);
        fprintf(file, "// This file is expected to be modified for the Web IDL interface\n");
        fprintf(file, "// implementation.  Permission to use, copy, modify and distribute\n");
        fprintf(file, "// this file in any software license is hereby granted.\n\n");
        fprintf(file, "#include \"%s.h\"\n\n", (node->getName() + "Imp").c_str());

        // body
        MessengerNameSpace ns(file, indent);

        ns.enter(prefixedName);

        MessengerImpSrc messengerImpSrc(file, stringTypeName, objectTypeName, useExceptions, indent);
        messengerImpSrc.at(node);

        ns.closeAll();

        // postable
        fclose(file);
    }
};

int printMessengerImpSrc(const char* stringTypeName, const char* objectTypeName,
                         bool useExceptions, bool useVirtualBase, const char* indent)
{
    Messenger::useVirtualBase = useVirtualBase;
    MessengerImpSrcVisitor visitor(stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);
    return 0;
}
