/*
 * Copyright 2010 Esrille Inc.
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
#include "cplusplus.h"
#include "cplusplusCall.h"
#include "cplusplusInvoke.h"
#include "cplusplusSrc.h"
#include "cplusplusTemplate.h"
#include "cplusplusMeta.h"

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

std::string CPlusPlus::getEscapedName(std::string name)
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

class CPlusPlusInterface : public CPlusPlus
{
public:
    CPlusPlusInterface(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        CPlusPlus(file, stringTypeName, objectTypeName, useExceptions, indent)
    {
        currentNode = 0;
    }

    CPlusPlusInterface(const Formatter* formattter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        CPlusPlus(formattter, stringTypeName, objectTypeName, useExceptions)
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
#ifdef USE_VIRTUAL_BASE
                if ((*i)->isBaseObject())
                {
                    write("virtual ");
                }
#endif
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

        writeln("static const char* const getQualifiedName() {");
            writeln("static const char* qualifiedName = \"%s\";", getInterfaceName(node->getQualifiedName()).c_str());
            writeln("return qualifiedName;");
        writeln("}");

        writeln("virtual void* queryInterface(const char* qualifiedName) {");
            writeln("if (qualifiedName == getQualifiedName()) {");
                writeln("return static_cast<%s*>(this);", getEscapedName(getClassName(node)).c_str());
            writeln("}");
            if (node->getExtends())
            {
                for (NodeList::iterator i = node->getExtends()->begin();
                    i != node->getExtends()->end();
                    ++i)
                {
                    writetab();
                    write("if (void* object = ");
                    (*i)->accept(this);
                    write("::queryInterface(qualifiedName)) {\n");
                        writeln("return object;");
                    writeln("}");
                }
            }
            writeln("return 0;");
        writeln("}");

        CPlusPlusMeta meta(this, stringTypeName, objectTypeName, useExceptions);
        const_cast<Interface*>(node)->accept(&meta);

#if 0
#ifdef USE_CONSTRUCTOR
        if (constructor)
        {
            constructorMode = true;
            writeln("");
            writeln("// Constructor");
            writeln("static %s_Constructor* factory;", getEscapedName(node->getName()).c_str());
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(constructor, *i);
            }
            constructorMode = false;
        }
#endif
#endif

        writeln("");
        CPlusPlusCall call(this, stringTypeName, objectTypeName, useExceptions);
        call.at(node);

        writeln("");
        CPlusPlusInvoke invoke(this, stringTypeName, objectTypeName, useExceptions);
        invoke.at(node);

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
        if (!currentNode)
        {
            currentNode = node->getParent();
            prefixedModuleName = currentNode->getPrefixedModuleName();
        }
        writetab();
        if (node->isTypedef(node->getParent()))
        {
            write("typedef ");
        }  // else this node is an exception class member.
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
        CPlusPlus::getter(node);
        write(" = 0;\n");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            writetab();
            CPlusPlus::setter(node);
            write(" = 0;\n");
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
        CPlusPlus::at(node);

#ifdef USE_CONSTRUCTOR
        if (constructorMode)
        {
            writeln("{");
                writetab();
                write("return factory->createInstance(");
                NodeList::iterator it = node->begin();
                for (int i = 0; i < paramCount; ++i, ++it)
                {
                    if (i != 0)
                    {
                        write(", ");
                    }
                    ParamDcl* param = static_cast<ParamDcl*>(*it);
                    write("%s", getEscapedName(param->getName()).c_str());
                    if (param->isVariadic() /* || param->getSpec()->isSequence(node) */)
                    {
                        write(", %sLength", getEscapedName(param->getName()).c_str());
                    }
                }
                write(");\n");
            writeln("}");
        }
        else
        {
            write(" = 0;\n");
        }
#else
        write(" = 0;\n");
#endif
    }
};

class CPlusPlusInclude: public CPlusPlus
{
    std::set<const Node*> includeSet;

public:
    CPlusPlusInclude(FILE* file,const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
        CPlusPlus(file, stringTypeName, objectTypeName, useExceptions, indent)
    {
        currentNode = 0;
    }

    CPlusPlusInclude(const Formatter* formattter, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions) :
        CPlusPlus(formattter, stringTypeName, objectTypeName, useExceptions)
    {
        currentNode = 0;
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
        if (dynamic_cast<ExceptDcl*>(resolved))
        {
            resolved->accept(this);
        }
        currentNode = saved;
    }

    virtual void at(const ExceptDcl* node)
    {
        includeSet.insert(node);
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

#ifdef USE_CONSTRUCTOR
        if (constructor)
        {
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(constructor, *i);
            }
        }
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
        if (useExceptions && node->getRaises())
        {
            visitChildren(node->getRaises());
        }
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

class CPlusPlusNameSpace : public Formatter
{
    std::vector<std::string> packageName;

public:
    CPlusPlusNameSpace(FILE* file, const std::string& indent) :
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
            writeln("namespace %s {", CPlusPlus::getEscapedName(packageName[j]).c_str());
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
class CPlusPlusImport : public Visitor, public Formatter
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

    CPlusPlusNameSpace* ns;

public:
    CPlusPlusImport(FILE* file, const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent, CPlusPlusNameSpace* ns) :
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
                CPlusPlusInterface cplusplusInterface(this, stringTypeName, objectTypeName, useExceptions);
                cplusplusInterface.at(*i);
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

class CPlusPlusVisitor : public Visitor
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;
    std::string indent;

public:
    CPlusPlusVisitor(const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
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

        std::string included = CPlusPlus::getIncludedName(createFileName(prefixedName, objectTypeName), indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        // body
        CPlusPlusNameSpace ns(file, indent);
        ns.enter(prefixedName);

        CPlusPlusInterface cplusplusInterface(file, stringTypeName, objectTypeName, useExceptions, indent);
        cplusplusInterface.at(node);

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
        std::string included = CPlusPlus::getIncludedName(createFileName(prefixedName, objectTypeName), indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        if (node->getAttr() & OpDcl::HasCovariant)
        {
            fprintf(file, "#include <type_traits>\n");
        }
        if (Interface* constructor = node->getConstructor())
        {
            fprintf(file, "#include <%s>\n", createFileName(constructor->getPrefixedName(), objectTypeName).c_str());
        }

        // body

        // The base classes and exception classes need to be defined.
        CPlusPlusInclude include(file, stringTypeName, objectTypeName, useExceptions, indent);
        include.at(node);
        include.print();

        CPlusPlusNameSpace ns(file, indent);

        CPlusPlusImport import(file, stringTypeName, objectTypeName, useExceptions, indent, &ns);
        import.at(node);
        import.print();

        ns.enter(prefixedName);

        CPlusPlusInterface cplusplusInterface(file, stringTypeName, objectTypeName, useExceptions, indent);
        cplusplusInterface.at(node);

        fprintf(file, "\n");

        CPlusPlusTemplate cplusplusTemplate(file, stringTypeName, objectTypeName, useExceptions, indent);
        cplusplusTemplate.at(node);

        ns.closeAll();

        // postable
        fprintf(file, "\n#endif  // %s\n", included.c_str());
        fclose(file);

#ifdef USE_CONSTRUCTOR
        if (Interface* constructor = node->getConstructor())
        {
            at(constructor);
        }
#endif
    }
};

int printCPlusPlus(const char* stringTypeName, const char* objectTypeName,
                   bool useExceptions, bool useVirtualBase, const char* indent)
{
    CPlusPlusVisitor visitor(stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);
    return 0;
}

class CPlusPlusSrcVisitor : public Visitor
{
    std::string stringTypeName;
    std::string objectTypeName;
    bool useExceptions;
    std::string indent;

public:
    CPlusPlusSrcVisitor(const std::string& stringTypeName, const std::string& objectTypeName, bool useExceptions, const std::string& indent) :
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
        CPlusPlusNameSpace ns(file, indent);

        CPlusPlusImport import(file, stringTypeName, objectTypeName, useExceptions, indent, &ns);
        import.at(node);
        // Include imported, i.e., forward declared, class definitions here
        // so that template functions can be used without explicitly including
        // the other generated files.
        import.printExtra();
        fprintf(file, "\n");

        ns.enter(prefixedName);

        CPlusPlusSrc cplusplusSrc(file, stringTypeName, objectTypeName, useExceptions, indent);
        cplusplusSrc.at(node);

        ns.closeAll();

        // postable
        fclose(file);

#ifdef USE_CONSTRUCTOR
        if (Interface* constructor = node->getConstructor())
        {
            at(constructor);
        }
#endif
    }
};

int printCPlusPlusSrc(const char* stringTypeName, const char* objectTypeName,
                      bool useExceptions, bool useVirtualBase, const char* indent)
{
    CPlusPlusSrcVisitor visitor(stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);
    return 0;
}
