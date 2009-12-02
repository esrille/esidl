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

    filename += "/" + node->getName() + ".java";

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
    JavaInterface(const char* source, FILE* file, const char* indent = "es") :
        Java(source, file, indent)
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
        write("public class %s extends RuntimeException {\n", node->getName().c_str());
            // Constructor
            // TODO: should check exception members
            writeln("public %s(short code, String message) {", node->getName().c_str());
                writeln("super(message);");
                writeln("this.code = code;");
            writeln("}");
            printChildren(node);
        writeln("}");
    }

    virtual void at(const Interface* node)
    {
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

        std::string name = node->getQualifiedName();
        name = getInterfaceName(name);
        write("public interface %s", name.substr(name.rfind(':') + 1).c_str());
        if (node->getExtends())
        {
            const char* separator = " extends ";
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

        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            visitInterfaceElement(node, *i);
        }

        // Expand mixins
        std::list<const Interface*> interfaceList;
        node->collectMixins(&interfaceList, node);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
                i != interfaceList.end();
                ++i)
        {
            if (*i == node)
            {
                continue;
            }
            writeln("// %s", (*i)->getName().c_str());
            const Node* saved = currentNode;
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                currentNode = *i;
                visitInterfaceElement(*i, *j);
            }
            currentNode = saved;
        }

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
            write(" %s;\n", node->getName().c_str());
        }
    }

    virtual void at(const ArrayDcl* node)
    {
        assert(!node->isLeaf());
        if (node->isTypedef(node->getParent()))
        {
            return;
        }

        writetab();
        node->getSpec()->accept(this);
        write(" %s", node->getName().c_str());
        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            write("[");
            (*i)->accept(this);
            write("]");
        }
        if (node->isTypedef(node->getParent()))
        {
            write(";\n");
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
        write(" %s = ", node->getName().c_str());
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
        write(";\n");
    }
};

class JavaImport : public Visitor, public Formatter
{
    std::string package;
    const Interface* current;
    bool printed;
    std::string prefixedName;
    std::set<std::string> importSet;

public:
    JavaImport(std::string package, FILE* file, const char* indent) :
        Formatter(file, indent),
        package(package),
        current(0)
    {
    }

    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const ScopedName* node)
    {
        assert(current);

        Node* resolved = node->search(current);
        node->check(resolved, "could not resolved %s.", node->getName().c_str());
        if (dynamic_cast<Interface*>(resolved) || dynamic_cast<ExceptDcl*>(resolved))
        {
            if (resolved->isBaseObject())
            {
                return;
            }
            if (Module* module = dynamic_cast<Module*>(resolved->getParent()))
            {
                if (prefixedName != module->getPrefixedName())
                {
                    importSet.insert(Java::getPackageName(module->getPrefixedName()) + "." + resolved->getName());
                }
            }
        }
    }

    virtual void at(const Interface* node)
    {
        if (current)
        {
            return;
        }
        current = node;
        if (Module* module = dynamic_cast<Module*>(node->getParent()))
        {
            prefixedName = module->getPrefixedName();
        }
        visitChildren(node->getExtends());
        visitChildren(node);
        // Expand mixins
        std::list<const Interface*> interfaceList;
        node->collectMixins(&interfaceList, node);
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            if (*i == node)
            {
                continue;
            }
            current = *i;
            visitChildren(*i);
        }
        current = node;
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
    const char* source;
    const char* indent;

    std::string prefixedName;

public:
    JavaVisitor(const char* source, const char* indent = "es") :
        source(source),
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
        if (1 < node->getRank() || node->isLeaf() || !node->isDefinedIn(source))
        {
            return;
        }

        FILE* file = createFile(Java::getPackageName(prefixedName), node);
        if (!file)
        {
            return;
        }

        fprintf(file, "// Generated by esidl %s.\n\n", VERSION);
        fprintf(file, "package %s;\n\n", Java::getPackageName(prefixedName).c_str());

        JavaInterface javaInterface(source, file, indent);
        javaInterface.at(node);

        fclose(file);
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() || !node->isDefinedIn(source) ||
            (node->getAttr() & Interface::Supplemental))
        {
            return;
        }

        FILE* file = createFile(Java::getPackageName(prefixedName), node);
        if (!file)
        {
            return;
        }

        fprintf(file, "// Generated by esidl %s.\n\n", VERSION);
        fprintf(file, "package %s;\n\n", Java::getPackageName(prefixedName).c_str());

        JavaImport import(Java::getPackageName(prefixedName), file, indent);
        import.at(node);
        import.print();

        JavaInterface javaInterface(source, file, indent);
        javaInterface.at(node);

        fclose(file);
    }
};

void printJava(const char* source, const char* indent)
{
    JavaVisitor visitor(source, indent);
    getSpecification()->accept(&visitor);
}
