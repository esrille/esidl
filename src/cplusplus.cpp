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

#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <set>
#include <vector>
#include "cplusplus.h"
#include "cplusplusCall.h"
#include "cplusplusInvoke.h"
#include "cplusplusTemplate.h"
#include "cplusplusMeta.h"

namespace
{

std::string createFileName(const std::string package, const Node* node)
{
    std::string filename = package;

    size_t pos = 0;
    for (;;)
    {
        pos = filename.find('.', pos);
        if (pos == std::string::npos)
        {
            break;
        }
        filename[pos] = '/';
    }

    filename += "/" + node->getName() + ".h";

    return filename;
}

FILE* createFile(const std::string package, const Node* node)
{
    std::string filename = createFileName(package, node);
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

    printf("# %s in %s\n", node->getName().c_str(), filename.c_str());
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
    CPlusPlusInterface(FILE* file, const char* indent = "es") :
        CPlusPlus(file, "std::string", "Object", true, indent)
    {
        currentNode = 0;
    }

    CPlusPlusInterface(const Formatter* formattter) :
        CPlusPlus(formattter, "std::string", "Object", true)
    {
        currentNode = 0;
    }

    virtual void at(const ExceptDcl* node)
    {
        if (!currentNode)
        {
            currentNode = node->getParent();
            qualifiedModuleName = currentNode->getQualifiedModuleName();
        }

        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("struct %s {\n", getEscapedName(node->getName()).c_str());
            printChildren(node);
        writeln("};");
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
            qualifiedModuleName = currentNode->getQualifiedModuleName();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

        write("class ");
        write("%s", getEscapedName(node->getName()).c_str());

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

#ifdef USE_CONSTRUCTOR
        if (constructor)
        {
            writeln("// [Constructor]");
            at(constructor);
            writeln("");
        }
#endif

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
            writeln("static const char* qualifiedName = \"%s\";", node->getQualifiedName().c_str());
            writeln("return qualifiedName;");
        writeln("}");

        writeln("virtual void* queryInterface(const char* qualifiedName) {");
            writeln("if (qualifiedName == getQualifiedName()) {");
                writeln("return static_cast<%s*>(this);", getEscapedName(node->getName()).c_str());
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

        CPlusPlusMeta meta(this);
        const_cast<Interface*>(node)->accept(&meta);

#ifdef USE_CONSTRUCTOR
        if (constructor)
        {
            constructorMode = true;
            writeln("");
            writeln("// Constructor");
            writeln("static Constructor* factory;");
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(constructor, *i);
            }
            constructorMode = false;
        }
#endif

        writeln("");
        CPlusPlusCall call(this);
        call.at(node);

        writeln("");
        CPlusPlusInvoke invoke(this);
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
            qualifiedModuleName = currentNode->getQualifiedModuleName();
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
    CPlusPlusInclude(FILE* file, const char* indent = "es") :
        CPlusPlus(file, "std::string", "Object", true, indent)
    {
        currentNode = 0;
    }

    CPlusPlusInclude(const Formatter* formattter) :
        CPlusPlus(formattter, "std::string", "Object", true)
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
            qualifiedModuleName = currentNode->getQualifiedModuleName();
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
        Interface* interface = static_cast<Interface*>(node->getParent());
        assert(interface);
        if (resolveInBase(interface, node->getName()))
        {
            // This node possibly overrides a base class operation.
            // It might need to have the interface definition of the return type.
            // cf. namedItem() in HTMLPropertiesCollection
            if (ScopedName* name = dynamic_cast<ScopedName*>(node->getSpec()))
            {
                Node* spec = name->search(currentNode);
                node->check(spec, "could not resolved %s.", name->getName().c_str());
                if (dynamic_cast<Interface*>(spec) && !spec->isBaseObject())
                {
                    includeSet.insert(spec);
                }
            }
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
            std::string name = createFileName(getPackageName(static_cast<Module*>((*i)->getParent())->getPrefixedName()), *i);
            writeln("#include <%s>", name.c_str());
        }
        write("\n");
    }
};

class CPlusPlusNameSpace : public Formatter
{
    std::vector<std::string> packageName;

public:
    CPlusPlusNameSpace(FILE* file, const char* indent) :
        Formatter(file, indent)
    {
    }

    size_t enter(std::string name)
    {
        std::vector<std::string> prevName = packageName;
        packageName.clear();

        size_t pos = name.rfind('.');
        if (pos == std::string::npos)
        {
            pos = 0;
        }
        else
        {
            size_t start = 0;
            size_t dot;
            while ((dot = name.find('.', start)) != std::string::npos)
            {
                packageName.push_back(name.substr(start, dot - start));
                start = dot + 1;
            }
            ++pos;
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

        return pos;
    }

    void closeAll()
    {
        write("\n");
        for (int i = 0; i < packageName.size(); ++i)
        {
            writeln("}");
        }
        packageName.clear();
    }
};

// Print the required class forward declarations and typedef statments.
class CPlusPlusImport : public Visitor, public Formatter
{
    std::string package;
    const Node* currentNode;
    bool printed;
    std::set<std::string> importSet;
    std::list<Member*> typedefList;
    std::set<Member*> typedefSet;
    bool importObjectArray;

    CPlusPlusNameSpace* ns;

public:
    CPlusPlusImport(std::string package,
                    FILE* file, const char* indent,
                    CPlusPlusNameSpace* ns) :
        Formatter(file, indent),
        package(package),
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
            if (Module* module = dynamic_cast<Module*>(resolved->getParent()))
            {
                importSet.insert(CPlusPlus::getPackageName(module->getPrefixedName()) + "." + resolved->getName());
            }
        }
        currentNode = saved;

        if (Member* type = node->isTypedef(currentNode))
        {
            // The order of definitions is important here.
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

    void print()
    {
        bool newline = false;

        if (importObjectArray)
        {
            writeln("#include <org/w3c/dom/ObjectArray.h>");
            writeln("");
            newline = true;
        }

        for (std::set<std::string>::iterator i = importSet.begin();
             i != importSet.end();
             ++i)
        {
            size_t pos = ns->enter(*i);
            writeln("class %s;", (*i).substr(pos).c_str());
            newline = true;
        }

        for (std::list<Member*>::iterator i = typedefList.begin();
             i != typedefList.end();
             ++i)
        {
            if (Module* module = dynamic_cast<Module*>((*i)->getParent()))
            {
                std::string name = CPlusPlus::getPackageName(module->getPrefixedName()) + "." + (*i)->getName();
                size_t pos = ns->enter(name);
                CPlusPlusInterface cplusplusInterface(this);
                cplusplusInterface.at(*i);
                newline = true;
            }
        }

        if (newline)
        {
            write("\n");
        }
    }
};

class CPlusPlusVisitor : public Visitor
{
    const char* indent;

    std::string prefixedName;

public:
    CPlusPlusVisitor(const char* indent = "es") :
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

    virtual void at(const Module* node)
    {
        std::string enclosed = prefixedName;
        prefixedName = node->getPrefixedName();
        at(static_cast<const Node*>(node));
        prefixedName = enclosed;
    }

    virtual void at(const ExceptDcl* node)
    {
        if (1 < node->getRank() || node->isLeaf())
        {
            return;
        }

        FILE* file = createFile(CPlusPlus::getPackageName(prefixedName), node);
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esaidl %s.\n\n", VERSION);

        Module* module = dynamic_cast<Module*>(node->getParent());
        assert(module);
        std::string name = CPlusPlus::getPackageName(module->getPrefixedName()) + "." + node->getName();

        std::string included = CPlusPlus::getIncludedName(name, indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        // body
        CPlusPlusNameSpace ns(file, indent);
        ns.enter(name);

        CPlusPlusInterface cplusplusInterface(file, indent);
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

        FILE* file = createFile(CPlusPlus::getPackageName(prefixedName), node);
        if (!file)
        {
            return;
        }

        // preamble
        fprintf(file, "// Generated by esidl %s.\n\n", VERSION);

        Module* module = dynamic_cast<Module*>(node->getParent());
        assert(module);
        std::string name = CPlusPlus::getPackageName(module->getPrefixedName()) + "." + node->getName();

        std::string included = CPlusPlus::getIncludedName(name, indent);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        // body

        // The base classes and exception classes need to be defined.
        CPlusPlusInclude include(file, indent);
        include.at(node);
        include.print();

        CPlusPlusNameSpace ns(file, indent);

        CPlusPlusImport import(CPlusPlus::getPackageName(prefixedName), file, indent, &ns);
        import.at(node);
        import.print();

        ns.enter(name);

        CPlusPlusInterface cplusplusInterface(file, indent);
        cplusplusInterface.at(node);

        fprintf(file, "\n");

        CPlusPlusTemplate cplusplusTemplate(file, indent);
        cplusplusTemplate.at(node);

        ns.closeAll();

        // postable
        fprintf(file, "\n#endif  // %s\n", included.c_str());
        fclose(file);
    }
};

int printCPlusPlus(const char* stringTypeName, const char* objectTypeName,
                   bool useExceptions, bool useVirtualBase, const char* indent)
{
    CPlusPlusVisitor visitor(indent);
    getSpecification()->accept(&visitor);
    return 0;
}
