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

#include "cplusplus.h"
#include "info.h"

class Cxx : public CPlusPlus
{
    void visitInterfaceElement(const Interface* interface, Node* element)
    {
        if (element->isSequence(interface) || element->isNative(interface))
        {
            return;
        }
        optionalStage = 0;
        do
        {
            optionalCount = 0;
            writetab();
            element->accept(this);
            write(";\n");
            ++optionalStage;
        } while (optionalStage <= optionalCount);
    }

public:
    Cxx(FILE* file, const char* stringTypeName = "char*", bool useExceptions = true) :
        CPlusPlus(file, stringTypeName, useExceptions)
    {
    }

    virtual void at(const StructType* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("struct %s", node->getName().c_str());
        if (!node->isLeaf())
        {
            writeln("");
            writeln("{");
            indent();
            printChildren(node);
            unindent();
            writetab();
            write("}");
        }
    }

    virtual void at(const ExceptDcl* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("struct %s", node->getName().c_str());
        writeln("");
        writeln("{");
        indent();
        printChildren(node);
        unindent();
        writetab();
        write("}");
    }

    virtual void at(const Interface* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("class %s", node->getName().c_str());
        if (node->getExtends())
        {
            write(" : ");
            prefix = "public ";
            node->getExtends()->accept(this);
            prefix = "";
        }
        if (!node->isLeaf())
        {
            writeln("");
            writeln("{");
            writeln("public:");
            indent();

            for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
            {
                visitInterfaceElement(node, *i);
            }

            // Expand mixins
            for (std::list<const Interface*>::const_iterator i = node->getMixins()->begin();
                 i != node->getMixins()->end();
                 ++i)
            {
                writeln("// %s", (*i)->getName().c_str());
                for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
                {
                    visitInterfaceElement(*i, *j);
                }
            }

            writeln("static const char* iid()");
            writeln("{");
            indent();
                writeln("static const char* const name = \"%s\";",
                        node->getQualifiedName().c_str());
                writeln("return name;");
            unindent();
            writeln("}");

            writeln("static const char* info()");
            writeln("{");
            indent();
                writetab();
                write("static const char* const info = ");
                indent();
                Info info(this, moduleName);
                const_cast<Interface*>(node)->accept(&info);
                write(";\n");
                unindent();
                writeln("return info;");
            unindent();
            writeln("}");

            if (Interface* constructor = node->getConstructor())
            {
                // Process constructors.
                constructorMode = true;
                for (NodeList::iterator i = constructor->begin();
                     i != constructor->end();
                     ++i)
                {
                    writetab();
                    (*i)->accept(this);
                }
                constructorMode = false;
                writeln("static Constructor* getConstructor()");
                writeln("{");
                indent();
                    writeln("return constructor;");
                unindent();
                writeln("}");
                writeln("static void setConstructor(Constructor* ctor)");
                writeln("{");
                indent();
                    writeln("constructor = ctor;");
                unindent();
                writeln("}");
                unindent();
                writeln("private:");
                indent();
                writeln("static Constructor* constructor;");
            }

            unindent();
            writetab();
            write("}");
        }

        if (node->getConstructor())
        {
            write(";\n\n");
            writetab();
            write("%s::Constructor* %s::constructor __attribute__((weak))",
                  node->getName().c_str(), node->getName().c_str());
        }
    }

    virtual void at(const NativeType* node)
    {
        if (node->getName() == "void_pointer")
        {
            write("void*");
        }
        else
        {
            write("%s", node->getName().c_str());
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
        if (node->getName() == "TRUE")
        {
            write("true");
        }
        else if (node->getName() == "FALSE")
        {
            write("false");
        }
        else
        {
            write("%s", node->getName().c_str());
        }
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef(node->getParent()))
        {
            write("typedef ");
        }
        if (node->isInterface(node->getParent()))
        {
            node->getSpec()->accept(this);
            write(" %s", node->getName().c_str());
        }
        else
        {
            node->getSpec()->accept(this);
            write(" %s", node->getName().c_str());
        }
    }

    virtual void at(const ArrayDcl* node)
    {
        assert(!node->isLeaf());

        at(static_cast<const Member*>(node));
        for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
        {
            write("[");
            (*i)->accept(this);
            write("]");
        }
    }

    virtual void at(const ConstDcl* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }
        write("static const ");
        at(static_cast<const Member*>(node));
        write(" = ");
        node->getExp()->accept(this);
    }

    virtual void at(const Attribute* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

        // getter
        CPlusPlus::getter(node);
        write(" = 0");

        if (!node->isReadonly() || node->isPutForwards() || node->isReplaceable())
        {
            // setter
            write(";\n");
            writetab();
            CPlusPlus::setter(node);
            write(" = 0");
        }
    }

    virtual void at(const OpDcl* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n", node->getJavadoc().c_str());
            writetab();
        }

        CPlusPlus::at(node);

        if (!constructorMode)
        {
            write(" = 0");
        }
        else
        {
            writeln("");
            writeln("{");
            indent();
                writeln("if (constructor)");
                indent();
                    writetab();
                    write("constructor->createInstance(");
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
            unindent();
            writeln("}");
        }
    }
};

class Import : public Visitor
{
    FILE* file;
    bool printed;

public:
    Import(FILE* file) :
        file(file),
        printed(false)
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

    virtual void at(const Include* node)
    {
        if (1 < node->getRank())
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

class Predeclaration : public Visitor
{
    FILE* file;

public:
    Predeclaration(FILE* file) :
        file(file)
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
        if (!node->hasPredeclarations())
        {
            return;
        }
        if (0 < node->getName().size())
        {
            fprintf(file, "namespace %s\n", node->getName().c_str());
            fprintf(file, "{\n");
                visitChildren(node);
            fprintf(file, "}\n\n");
        }
        else
        {
            visitChildren(node);
        }
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || !node->isLeaf())
        {
            return;
        }
        fprintf(file, "    class %s;\n", node->getName().c_str());
    }
};

void printCxx(const std::string& filename, const char* stringTypeName, bool useExceptions)
{
    printf("# %s\n", filename.c_str());

    FILE* file = fopen(filename.c_str(), "w");
    if (!file)
    {
        return;
    }

    std::string included = getIncludedName(filename);
    fprintf(file, "// Generated by esidl %s.\n\n", VERSION);
    fprintf(file, "#ifndef %s\n", included.c_str());
    fprintf(file, "#define %s\n\n", included.c_str());

    Import import(file);
    getSpecification()->accept(&import);
    if (import.hasPrinted())
    {
        fprintf(file, "\n");
    }

    if (!Node::getFlatNamespace())
    {
        Predeclaration predeclaration(file);
        getSpecification()->accept(&predeclaration);
    }

    Cxx cxx(file, stringTypeName, useExceptions);
    getSpecification()->accept(&cxx);

    fprintf(file, "#endif  // %s\n", included.c_str());

    fclose(file);
}
