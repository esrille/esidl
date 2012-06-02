/*
 * Copyright 2012 Esrille Inc.
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
#include "java.h"

namespace
{

FILE* createFile(const std::string package, const Node* node)
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

#ifndef USE_CONSTRUCTOR
    filename += "/" + node->getName() + ".java";
#else
    filename += "/";
    if (node->getAttr() & Node::Constructor)
    {
        filename += node->getParent()->getName() + node->getCtorScope();
    }
    filename += node->getName() + ".java";
#endif

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

std::string Java::getEscapedName(std::string name)
{
    static const char* reservedWords[] =
    {
        "abstract",
        "assert",
        "boolean",
        "break",
        "byte",
        "case",
        "catch",
        "char",
        "class",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extends",
        "final",
        "finally",
        "float",
        "for",
        "goto",
        "if",
        "implements",
        "import",
        "instanceof",
        "int",
        "interface",
        "long",
        "native",
        "new",
        "package",
        "private",
        "protected",
        "public",
        "return",
        "short",
        "static",
        "strictfp",
        "super",
        "switch",
        "synchronized",
        "this",
        "throw",
        "throws",
        "transient",
        "try",
        "void",
        "volatile",
        "while",
    };
    const char** p = find(reservedWords, reservedWords + sizeof reservedWords / sizeof reservedWords[0], name);
    if (p == &reservedWords[sizeof reservedWords / sizeof reservedWords[0]])
    {
        return name;
    }
    return "_" + name;
}

class JavaInterface : public Java
{
    // TODO: Move to Java
    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (dynamic_cast<Interface*>(element))
        {
            // Do not process Constructor.
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
    JavaInterface(FILE* file, const char* indent = "es") :
        Java(file, indent)
    {
    }

    virtual void at(const ExceptDcl* node)
    {
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("public class %s extends RuntimeException {\n", getEscapedName(node->getName()).c_str());
            // Constructor
            // TODO: should check exception members
            writeln("public %s(short code, String message) {", getEscapedName(node->getName()).c_str());
                writeln("super(message);");
                writeln("this.code = code;");
            writeln("}");
            printChildren(node);
        writeln("}");
    }

    virtual void at(const Interface* node)
    {
        Interface* constructor = node->getConstructor();

        if (!currentNode)
        {
            currentNode = node->getParent();
        }
        assert(!(node->getAttr() & Interface::Supplemental) && !node->isLeaf());
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

#ifdef USE_ABSTRACT
        write("public %s ", constructor ? "abstract class" : "interface");
#else
        write("public interface ");
#endif

#ifdef USE_CONSTRUCTOR
        if (node->getAttr() & Interface::Constructor)
        {
            write("%s_%s", getEscapedName(node->getParent()->getName()).c_str(),
                           node->getName().c_str());
        }
        else
        {
            write("%s", getEscapedName(node->getName()).c_str());
        }
#else
        write("%s", getEscapedName(node->getName()).c_str());
#endif

        if (node->getExtends())
        {
#ifdef USE_ABSTRACT
            const char* separator = constructor ? " implements " : " extends ";
#else
            const char* separator = " extends ";
#endif
            for (NodeList::iterator i = node->getExtends()->begin();
                 i != node->getExtends()->end();
                 ++i)
            {
                if ((*i)->getName() == Node::getBaseObjectName())
                {
                    // Do not extend from 'Object'.
                    continue;
                }
                write(separator);
                separator = ", ";
                (*i)->accept(this);
            }
        }
        write(" {\n");

#ifdef USE_ABSTRACT
        if (constructor)
        {
            methodAccessLevel = "public abstract";
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

#ifdef USE_ABSTRACT
        if (constructor)
        {
            methodAccessLevel = "public static";
            writeln("");
            writeln("// Constructor");
            writeln("static public %s_Constructor factory;",
                    getEscapedName(node->getName()).c_str());
            for (NodeList::iterator i = constructor->begin();
                 i != constructor->end();
                 ++i)
            {
                visitInterfaceElement(constructor, *i);
            }
            methodAccessLevel = "public";
        }
#endif

        writeln("}");
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
            node->getSpec()->accept(this);
        }
        else
        {
            // This node is an exception class member.
            writetab();
            write("public ");
            node->getSpec()->accept(this);
            write(" %s;\n", getEscapedName(node->getName()).c_str());
        }
    }

    virtual void at(const ConstDcl* node)
    {
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("public static final ");
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
        Java::getter(node);
        write(";\n");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            writetab();
            Java::setter(node);
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
        Java::at(node);

#ifdef USE_ABSTRACT
        if (methodAccessLevel == "public static")
        {
            writeln("{");
                writetab();
                write("return factory.createInstance(");
                NodeList::iterator param = node->begin();
                for (int i = 0; i < paramCount; ++i, ++param)
                {
                    if (i != 0)
                    {
                        write(", ");
                    }
                    write("%s", getEscapedName((*param)->getName()).c_str());
                }
                write(");\n");
            writeln("}");
        }
        else
        {
            write(";\n");
        }
#else
        write(";\n");
#endif
    }
};

class JavaImport : public Visitor, public Formatter
{
    std::string package;
    const Node* currentNode;
    bool printed;
    std::string prefixedName;
    std::set<std::string> importSet;

public:
    JavaImport(std::string package, FILE* file, const char* indent) :
        Formatter(file, indent),
        package(package),
        currentNode(0)
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
                if (prefixedName != module->getPrefixedName())
                {
                    importSet.insert(Java::getPackageName(module->getPrefixedName()) + "." + resolved->getName());
                }
            }
        }
        currentNode = saved;
    }

    virtual void at(const SequenceType* node)
    {
        Node* spec = node->getSpec();
        spec->accept(this);
    }

    virtual void at(const ArrayType* node)
    {
        Node* spec = node->getSpec();
        if (ScopedName* name = dynamic_cast<ScopedName*>(spec))
        {
            spec = name->search(currentNode);
        }
        Type* type = dynamic_cast<Type*>(spec);
        if (type && !(type->getAttr() & Node::Nullable))
        {
            if (type->getName() == "boolean")
            {
                importSet.insert("org.w3c.dom.BooleanArray");
                return;
            }
            else if (type->getName() == "octet")
            {
                importSet.insert("org.w3c.dom.OctetArray");
                return;
            }
            else if (type->getName() == "byte")
            {
                importSet.insert("org.w3c.dom.ByteArray");
                return;
            }
            else if (type->getName() == "unsigned byte")
            {
                importSet.insert("org.w3c.dom.UnsignedByteArray");
                return;
            }
            else if (type->getName() == "short")
            {
                importSet.insert("org.w3c.dom.ShortArray");
                return;
            }
            else if (type->getName() == "unsigned short")
            {
                importSet.insert("org.w3c.dom.UnsignedShortArray");
                return;
            }
            else if (type->getName() == "long")
            {
                importSet.insert("org.w3c.dom.LongArray");
                return;
            }
            else if (type->getName() == "unsigned long")
            {
                importSet.insert("org.w3c.dom.UnsignedLongArray");
                return;
            }
            else if (type->getName() == "long long")
            {
                importSet.insert("org.w3c.dom.LongLongArray");
                return;
            }
            else if (type->getName() == "unsigned long long")
            {
                importSet.insert("org.w3c.dom.UnsignedLongLongArray");
                return;
            }
            else if (type->getName() == "float")
            {
                importSet.insert("org.w3c.dom.FloatArray");
                return;
            }
            else if (type->getName() == "double")
            {
                importSet.insert("org.w3c.dom.DoubleArray");
                return;
            }
        }
        importSet.insert("org.w3c.dom.ObjectArray");
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
        currentNode = node;
#ifdef USE_CONSTRUCTOR
        if (node->getAttr() & Interface::Constructor)
        {
            currentNode = node->getParent();
        }
#endif
        if (Module* module = dynamic_cast<Module*>(currentNode->getParent()))
        {
            prefixedName = module->getPrefixedName();
        }
        visitChildren(node->getExtends());
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
        currentNode = node;
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
        if (importSet.empty())
        {
            return;
        }
        for (std::set<std::string>::iterator i = importSet.begin();
             i != importSet.end();
             ++i)
        {
            write("import %s;\n", (*i).c_str());
        }
        write("\n");
    }
};

class JavaVisitor : public Visitor
{
    const char* indent;

    std::string prefixedName;

public:
    JavaVisitor(const char* indent = "es") :
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

        FILE* file = createFile(Java::getPackageName(prefixedName), node);
        if (!file)
        {
            return;
        }

        fprintf(file, "// Generated by esidl %s.\n\n", PACKAGE_VERSION);
        fprintf(file, "package %s;\n\n", Java::getPackageName(prefixedName).c_str());

        JavaInterface javaInterface(file, indent);
        javaInterface.at(node);

        fclose(file);
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() ||
            (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

        FILE* file = createFile(Java::getPackageName(prefixedName), node);
        if (!file)
        {
            return;
        }

        fprintf(file, "// Generated by esidl %s.\n\n", PACKAGE_VERSION);
        fprintf(file, "package %s;\n\n", Java::getPackageName(prefixedName).c_str());

        JavaImport import(Java::getPackageName(prefixedName), file, indent);
        import.at(node);
        import.print();

        JavaInterface javaInterface(file, indent);
        javaInterface.at(node);

        fclose(file);

#ifdef USE_CONSTRUCTOR
        if (Interface* constructor = node->getConstructor())
        {
            at(constructor);
        }
#endif
    }
};

int printJava(const char* indent)
{
    JavaVisitor visitor(indent);
    getSpecification()->accept(&visitor);
    return 0;
}
