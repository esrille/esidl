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

class Cxx : public CPlusPlus
{
public:
    Cxx(FILE* file) :
        CPlusPlus(file)
    {
    }

    virtual void at(const Module* node)
    {
        if (0 < node->getName().size())
        {
            if (node->getJavadoc().size())
            {
                write("%s\n%s", node->getJavadoc().c_str(), indentString.c_str());
            }
            write("namespace %s\n", node->getName().c_str());
            write("%s{\n", indentString.c_str());
            indent();
            printChildren(node);
            unindent();
            write("%s}", indentString.c_str());
        }
        else
        {
            printChildren(node);
        }
    }

    virtual void at(const EnumType* node)
    {
        write("enum %s", node->getName().c_str());
        write("\n%s{\n", indentString.c_str());
        indent();
        write("%s", indentString.c_str());
        printChildren(node);
        unindent();
        write("\n%s}", indentString.c_str());
    }

    virtual void at(const StructType* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n%s", node->getJavadoc().c_str(), indentString.c_str());
        }
        write("struct %s", node->getName().c_str());
        if (!node->isLeaf())
        {
            write("\n%s{\n", indentString.c_str());
            indent();
            printChildren(node);
            unindent();
            write("%s}", indentString.c_str());
        }
    }

    virtual void at(const ExceptDcl* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n%s", node->getJavadoc().c_str(), indentString.c_str());
        }
        write("struct %s", node->getName().c_str());
        write("\n%s{\n", indentString.c_str());
        indent();
        printChildren(node);
        unindent();
        write("%s}", indentString.c_str());
    }

    virtual void at(const Interface* node)
    {
        if (node->getJavadoc().size())
        {
            write("%s\n%s", node->getJavadoc().c_str(), indentString.c_str());
        }
        write("class I%s", node->getName().c_str());
        if (node->getExtends())
        {
            write(" : ");
            prefix = "public ";
            interfaceMode = true;
            node->getExtends()->accept(this);
            interfaceMode = false;
            prefix = "";
        }
        if (!node->isLeaf())
        {
            write("\n%s{\n%spublic:\n", indentString.c_str(), indentString.c_str());
            indent();

            // printChildren(node);
            int count = 0;
            for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
            {
                if (dynamic_cast<PragmaID*>(*i) || (*i)->isSequence(node) || (*i)->isNative(node))
                {
                    continue;
                }

                if (0 < count)
                {
                    write(";\n");
                }
                write("%s", indentString.c_str());

                ++count;
                (*i)->accept(this);
            }
            if (0 < count)
            {
                write(";\n");
            }

            write("%sstatic const char* iid()\n", indentString.c_str());
            write("%s{\n", indentString.c_str());
            write("%s    static const char* name = \"%s\";\n", indentString.c_str(), node->getQualifiedName().c_str());
            write("%s    return name;\n", indentString.c_str());
            write("%s}\n", indentString.c_str());

            if (Interface* constructor = node->getConstructor())
            {
                // Process constructors.
                constructorMode = true;
                for (NodeList::iterator i = constructor->begin();
                     i != constructor->end();
                     ++i)
                {
                    write("%s", indentString.c_str());
                    (*i)->accept(this);
                }
                constructorMode = false;
                write("%sstatic IConstructor* getConstructor()\n", indentString.c_str());
                write("%s{\n", indentString.c_str());
                write("%s    return constructor;\n", indentString.c_str());
                write("%s}\n", indentString.c_str());
                write("%sstatic void setConstructor(IConstructor* ctor)\n", indentString.c_str());
                write("%s{\n", indentString.c_str());
                write("%s    constructor = ctor;\n", indentString.c_str());
                write("%s}\n", indentString.c_str());
                write("private:\n");
                write("%sstatic IConstructor* constructor;\n", indentString.c_str());
            }

            unindent();
            write("%s}", indentString.c_str());
        }

        if (node->getConstructor())
        {
            write(";\n\n");
            write("%sI%s::IConstructor* I%s::constructor __attribute__((weak))",
                    indentString.c_str(), node->getName().c_str(), node->getName().c_str());
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

    virtual void at(const PragmaID* node)
    {
    }

    virtual void at(const Member* node)
    {
        if (node->isTypedef())
        {
            write("typedef ");
        }
        if (node->isInterface(node->getParent()))
        {
            interfaceMode = true;
            node->getSpec()->accept(this);
            write(" I%s", node->getName().c_str());
            interfaceMode = false;
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
            write("%s\n%s", node->getJavadoc().c_str(), indentString.c_str());
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

        if (!node->isReadonly())
        {
            write(";\n");

            // setter
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
            write("\n%s{\n", indentString.c_str());
            write("%s    if (constructor)\n", indentString.c_str());
            write("%s        constructor->createInstance(", indentString.c_str());
            for (NodeList::iterator i = node->begin(); i != node->end(); ++i)
            {
                if (i != node->begin())
                {
                    write(", ");
                }
                write("%s", (*i)->getName().c_str());
            }
            write(");\n");
            write("%s}\n", indentString.c_str());
        }
    }
};

class Import : public Visitor
{
    FILE* file;

public:
    Import(FILE* file) :
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

    virtual void at(const Include* node)
    {
        if (1 < node->getRank())
        {
            return;
        }
        fprintf(file, "#include <%s>\n", getOutputFilename(node->getName().c_str(), "h").c_str());
    }
};

void printCxx(const std::string& filename)
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

    Cxx cxx(file);
    getSpecification()->accept(&cxx);

    fprintf(file, "#endif  // %s\n", included.c_str());

    fclose(file);
}
