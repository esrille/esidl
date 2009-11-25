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

#include "java.h"
#include "info.h"

namespace
{

std::string getFileName(const std::string& name)
{
    std::string filename(name);

    for (int i = 0; i < filename.size(); ++i)
    {
        char c = filename[i];
        if (c == '.' || c == '/' || c == '\\')
        {
            filename[i] = '_';
        }
    }
    return filename;
}

FILE* createFile(const char* source, const Node* node)
{
    std::string filename = source;
    size_t begin = filename.rfind("/");
    if (begin == std::string::npos)
    {
        begin = 0;
    }
    else
    {
        ++begin;
    }
    filename.replace(begin, filename.length() - begin, getFileName(node->getName()));
    filename += ".";
    filename = getOutputFilename(filename.c_str(), "java");
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
            // TODO(Shiki): Need a constructor.
            printChildren(node);
        writeln("}");
    }

    virtual void at(const Interface* node)
    {
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
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                visitInterfaceElement(*i, *j);
            }
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
            return;
        }

        writetab();
        write("public ");
        node->getSpec()->accept(this);
        write(" %s;\n", node->getName().c_str());
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

class JavaVisitor : public Visitor
{
    const char* source;
    const char* indent;

    std::string moduleName;

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
        if (0 < node->getName().size())
        {
            moduleName += "::";
            moduleName += node->getName();
            at(static_cast<const Node*>(node));
            moduleName.erase(moduleName.size() - node->getName().size() - 2);
        }
        else
        {
            at(static_cast<const Node*>(node));
        }
    }

    virtual void at(const ExceptDcl* node)
    {
        if (1 < node->getRank() || node->isLeaf() || !node->isDefinedIn(source))
        {
            return;
        }

        FILE* file = createFile(source, node);
        if (!file)
        {
            return;
        }

        fprintf(file, "// Generated by esidl %s.\n\n", VERSION);

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

        FILE* file = createFile(source, node);
        if (!file)
        {
            return;
        }

        fprintf(file, "// Generated by esidl %s.\n\n", VERSION);

#if 0
        size_t pos = 0;
        while ((pos = moduleName.find("::", pos)) != std::string::npos)
        {
            size_t next = moduleName.find("::", pos + 2);
            fprintf(file, "package %s\n{\n\n",
                    moduleName.substr(pos + 2, (next == std::string::npos) ? std::string::npos : next - pos).c_str());
            pos += 2;
        }
#endif

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
