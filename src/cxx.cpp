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

#include "cxx.h"
#include "info.h"

class CxxInterface : public Cxx
{
    bool useVirtualBase;

    // TODO: Move to Cxx
    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        optionalStage = 0;
        do
        {
            optionalCount = 0;
            element->accept(this);
            ++optionalStage;
        } while (optionalStage <= optionalCount);
    }

public:
    CxxInterface(const char* source, FILE* file, const char* stringTypeName = "char*", const char* objectTypeName = "object",
        bool useExceptions = true, bool useVirtualBase = false, const char* indent = "es") :
        Cxx(source, file, stringTypeName, objectTypeName, useExceptions, indent),
        useVirtualBase(useVirtualBase)
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
        write("struct %s {\n", node->getName().c_str());
            printChildren(node);
        writeln("};");
    }

    virtual void at(const Interface* node)
    {
        if (node->getAttr() & Interface::Supplemental)
        {
            return;
        }
        writetab();
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

        std::string name = node->getQualifiedName();
        name = getInterfaceName(name);
        write("class %s", name.substr(name.rfind(':') + 1).c_str());
        if (node->isLeaf())
        {
            write(";\n");
        }
        else
        {
            if (node->getExtends())
            {
                write(" : ");
                for (NodeList::iterator i = node->getExtends()->begin();
                     i != node->getExtends()->end();
                     ++i)
                {
                    if (i != node->getExtends()->begin())
                    {
                        write(", ");
                    }
                    write("public ");
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

            writeln("static const char* iid() {");
                writeln("static const char* const name = \"%s\";",
                        name.c_str());
                writeln("return name;");
            writeln("}");

            writeln("static const char* info() {");
                writetab();
                write("static const char* const info =");
                flush();
                indent();
                Info info(this, moduleName, objectTypeName);
                const_cast<Interface*>(node)->accept(&info);
                info.flush();
                write(";\n");
                unindent();
                writeln("return info;");
            writeln("}");

            if (Interface* constructor = node->getConstructor())
            {
                // Process constructors.
                constructorMode = true;
                for (NodeList::iterator i = constructor->begin();
                     i != constructor->end();
                     ++i)
                {
                    (*i)->accept(this);
                }
                constructorMode = false;
                writeln("static Constructor* getConstructor() {");
                    writeln("return constructor;");
                writeln("}");
                writeln("static void setConstructor(Constructor* ctor) {");
                    writeln("constructor = ctor;");
                writeln("}");
                unindent();
                writeln("private:");
                indent();
                writeln("static Constructor* constructor;");
            }

            writeln("};");

            if (node->getConstructor())
            {
                // TODO: Control the use of GCC extensions.
                writeln("%s::Constructor* %s::constructor __attribute__((weak));",
                        node->getName().c_str(), node->getName().c_str());
            }
        }
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
        writetab();
        if (node->isTypedef(node->getParent()))
        {
            write("typedef ");
        }
        node->getSpec()->accept(this);
        write(" %s;\n", node->getName().c_str());
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
        Cxx::getter(node);
        write(" = 0;\n");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            writetab();
            Cxx::setter(node);
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

        Cxx::at(node);

        if (!constructorMode)
        {
            write(" = 0;\n");
        }
        else
        {
            write("{");
            writeln("");
                writeln("if (constructor)");
                indent();
                    writetab();
                    write("return constructor->createInstance(");
                    for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
                    {
                        if (i != node->begin())
                        {
                            write(", ");
                        }
                        write("%s", (*i)->getName().c_str());
                    }
                    write(");");
                    writeln("");
                unindent();
                writeln("else");
                indent();
                    writeln("return 0;");
                unindent();
            writeln("}");
        }
    }
};

class Import : public Visitor
{
    const char* source;
    FILE* file;
    bool printed;

public:
    Import(const char* source, FILE* file) :
        source(source),
        file(file),
        printed(false)
    {
    }

    virtual void at(const Node* node)
    {
        visitChildren(node);
    }

    virtual void at(const Include* node)
    {
        if (!node->isDefinedIn(source))
        {
            return;
        }
        if (node->isSystem())
        {
            fprintf(file, "#include <%s>\n", getOutputFilename(node->getName().c_str(), "h").c_str());
        }
        else
        {
            fprintf(file, "#include \"%s\"\n", getOutputFilename(node->getName().c_str(), "h").c_str());
        }
        printed = true;
    }

    bool hasPrinted() const
    {
        return printed;
    }
};

class Predeclaration : public Visitor, public Formatter
{
    const char* source;

public:
    Predeclaration(const char* source, FILE* file, const char* indent) :
        Formatter(file, indent),
        source(source)
    {
    }

    virtual void at(const Node* node)
    {
        if (!node->isDefinedIn(source))
        {
            return;
        }
        visitChildren(node);
    }

    virtual void at(const Module* node)
    {
        if (!node->hasPredeclarations())
        {
            return;
        }
        if (0 < node->getName().size())
        {
            write("namespace %s {\n", node->getName().c_str());
                visitChildren(node);
            writeln("}");
            writeln("");
        }
        else
        {
            visitChildren(node);
        }
    }

    virtual void at(const Interface* node)
    {
        if (!node->isDefinedIn(source) || !node->isLeaf())
        {
            return;
        }
        writeln("class %s;", node->getName().c_str());
    }
};

void printCxx(const char* source, const char* stringTypeName, const char* objectTypeName,
              bool useExceptions, bool useVirtualBase, const char* indent)
{
    const std::string filename = getOutputFilename(source, "h");
    printf("# %s\n", filename.c_str());

    FILE* file = fopen(filename.c_str(), "w");
    if (!file)
    {
        return;
    }

    std::string included = Cxx::getIncludedName(filename, indent);
    fprintf(file, "// Generated by esidl %s.\n\n", VERSION);
    fprintf(file, "#ifndef %s\n", included.c_str());
    fprintf(file, "#define %s\n\n", included.c_str());

    Import import(source, file);
    getSpecification()->accept(&import);
    if (import.hasPrinted())
    {
        fprintf(file, "\n");
    }

    if (!Node::getFlatNamespace())
    {
        Predeclaration predeclaration(source, file, indent);
        getSpecification()->accept(&predeclaration);
    }

    CxxInterface cxx(source, file, stringTypeName, objectTypeName, useExceptions, useVirtualBase, indent);
    getSpecification()->accept(&cxx);

    fprintf(file, "#endif  // %s\n", included.c_str());

    fclose(file);
}
