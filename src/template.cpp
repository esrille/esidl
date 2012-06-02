/*
 * Copyright 2012 Esrille Inc.
 * Copyright 2009, 2010 Google Inc.
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

#include <set>

class TemplateVisitor : public Cxx
{
    unsigned methodNumber;

    // TODO: Move to Cxx later
    void writeSpec(const Member* node)
    {
        Node* spec = node->getSpec();
        SequenceType* seq = const_cast<SequenceType*>(spec->isSequence(node->getParent()));
        if (seq)
        {
            write("int");
        }
        else if (spec->isString(node->getParent()))
        {
            write("const ");
            spec->accept(this);
        }
        else if (spec->isArray(node->getParent()))
        {
            write("void");
        }
        else if (spec->isInterface(node->getParent()))
        {
            spec->accept(this);
            write("*");
        }
        else if (NativeType* nativeType = spec->isNative(node->getParent()))
        {
            nativeType->accept(this);
        }
        else
        {
            spec->accept(this);
        }
    }

public:
    TemplateVisitor(const char* source, FILE* file, const char* stringTypeName = "char*", const char* objectTypeName = "object",
                    bool useExceptions = true, const char* indent = "es") :
        Cxx(source, file, stringTypeName, objectTypeName, useExceptions, indent)
    {
    }

    virtual void at(const Interface* node)
    {
        if (node->getAttr() & Interface::Supplemental)
        {
            return;
        }
        if (!node->isDefinedIn(source) || node->isLeaf() || node->isBaseObject())
        {
            return;
        }

        writeln("template<class P, Any (*invoke)(P, const char*, unsigned, unsigned, unsigned, Any*), class O = %s, unsigned B = 0>",
                node->getName().c_str());
        writetab();
        write("class %s_Proxy : ", node->getName().c_str());
        if (Node* extends = node->getExtends())
        {
            write("public ");
            int n = 0;
            for (NodeList::reverse_iterator i = extends->rbegin(); i != extends->rend(); ++i)
            {
                if (!(*i)->isBaseObject())
                {
                    ++n;
                    write("%s_Proxy<P, invoke, ", (*i)->getName().c_str());
                }
            }
            write("O");
            for (int i = 0; i < n; ++i)
            {
                write(", B + %d>", i);
            }
        }
        else
        {
            write("public O");
        }
        const std::list<const Interface*>* implementList = node->getImplements();
        if (!implementList->empty())
        {
            for (std::list<const Interface*>::const_iterator i = implementList->begin();
                 i != implementList->end();
                 ++i)
            {
                write(", public %s", (*i)->getName().c_str());
            }
        }
        write(" {\n");
        unindent();
        writeln("public:");
        indent();

        std::list<const Interface*> interfaceList;
        node->collectSupplementals(&interfaceList);
        methodNumber = 0;
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            if (!((*i)->getAttr() & Interface::ImplementedOn))
            {
                currentNode = *i;
            }
            writeln("// %s", (*i)->getName().c_str());
            for (NodeList::iterator j = (*i)->begin(); j != (*i)->end(); ++j)
            {
                if (Attribute* attr = dynamic_cast<Attribute*>(*j))
                {
                    attr->accept(this);
                }
                else if (OpDcl* op = dynamic_cast<OpDcl*>(*j))
                {
                    optionalStage = 0;
                    do
                    {
                        optionalCount = 0;
                        op->accept(this);
                        ++optionalStage;
                    } while (optionalStage <= optionalCount);
                }
            }
        }
        currentNode = node;
        writeln("};");
    }

    virtual void at(const Attribute* node)
    {
        static Type replaceable("any");
        Node* spec = node->getSpec();
        Node* scope = node->getParent();
        if (node->isReplaceable())
        {
            spec = &replaceable;
            scope = getSpecification();
        }

        // getter
        writetab();
        Cxx::getter(node);
        write(" {\n");
        {
            unsigned paramCount = 1;  // for 'this'
            bool hasBuffer = false;
            if (spec->isSequence(node) ||
                stringTypeName == "char*" && (spec->isString(node) ||
                                              spec->isAny(node)))
            {
                hasBuffer = true;
                paramCount += 2;  // for the pointer to the buffer and its length
            }
            else if (spec->isArray(node))
            {
                hasBuffer = true;
                ++paramCount;
            }
            writeln("Any param[%u];", paramCount);
            unsigned anyIndex = 1;
            if (hasBuffer)
            {
                std::string name = getBufferName(node);
                writeln("param[%u] = reinterpret_cast<intptr_t>(%s);", anyIndex, name.c_str());
                ++anyIndex;
                if (!spec->isArray(node))
                {
                    writeln("param[%u] = %sLength;", anyIndex, name.c_str());
                    ++anyIndex;
                }
            }

            // Invoke
            if (spec->isVoid(node) || spec->isArray(node))
            {
                writeln("invoke(this, %s::iid(), B, %u, %u, param);",
                        currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else if (spec->isAny(node))
            {
                writeln("return invoke(this, %s::iid(), B, %u, %u, param);",
                        currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else if (spec->isInterface(node))
            {
                writetab();
                write("return dynamic_cast<");
                writeSpec(node);
                write(">(static_cast<Object*>(invoke(this, %s::iid(), B, %u, %u, param)));\n",
                      currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else
            {
                writetab();
                write("return static_cast<");
                writeSpec(node);
                write(">(invoke(this, %s::iid(), B, %u, %u, param));\n",
                      currentNode->getName().c_str(), methodNumber, paramCount);
            }
        }
        writeln("}");
        ++methodNumber;

        // setter
        if (node->isReadonly() && !node->isPutForwards() && !node->isReplaceable())
        {
            return;
        }

        if (node->isPutForwards())
        {
            Interface* target = dynamic_cast<Interface*>(dynamic_cast<ScopedName*>(spec)->search(scope));
            assert(target);
            Attribute* forwards = dynamic_cast<Attribute*>(target->search(node->getPutForwards()));
            assert(forwards);
            spec = forwards->getSpec();
            scope = target;
        }
        writetab();
        Cxx::setter(node);
        write(" {\n");
        {
            unsigned paramCount = 2;  // +1 for 'this'
            if (spec->isSequence(node))
            {
                ++paramCount;  // for the length
            }
            writeln("Any param[%u];", paramCount);
            unsigned anyIndex = 1;
            std::string name = getBufferName(node);
            if (spec->isSequence(node))
            {
                writeln("param[%u] = reinterpret_cast<intptr_t>(%s);", anyIndex, name.c_str());
                ++anyIndex;
                writeln("param[%u] = %sLength;", anyIndex, name.c_str());
                ++anyIndex;
            }
            else
            {
                writeln("param[%u] = %s;", anyIndex, name.c_str());
                ++anyIndex;
            }

            // Invoke
            if (spec->isSequence(node))
            {
                writeln("return static_cast<int>(invoke(this, %s::iid(), B, %u, %u, param));",
                        currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else
            {
                writeln("invoke(this, %s::iid(), B, %u, %u, param);",
                        currentNode->getName().c_str(), methodNumber, paramCount);
            }
        }
        writeln("}");
        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writetab();
        Cxx::at(node);
        write(" {\n");
            bool hasBuffer = false;
            Node* spec = node->getSpec();
            unsigned paramIndex;

            // Convert parameters into the array of 'Any'
            unsigned paramCount = getParamCount();
            ++paramCount;  // for 'this'
            if (spec->isSequence(node) ||
                stringTypeName == "char*" && (spec->isString(node) ||
                                              spec->isAny(node)))
            {
                hasBuffer = true;
                paramCount += 2;  // for the pointer to the buffer and its length
            }
            else if (spec->isArray(node))
            {
                hasBuffer = true;
                ++paramCount;  // for the pointer to the array
            }
            paramIndex = 0;
            for (NodeList::iterator i = node->begin();
                 paramIndex < getParamCount();
                 ++i, ++paramIndex)
            {
                ParamDcl* param = dynamic_cast<ParamDcl*>(*i);
                assert(param);
                Node* paramSpec = param->getSpec();
                if (param->isVariadic() || paramSpec->isSequence(node))
                {
                    ++paramCount;   // for the size of sequence
                }
            }
            writeln("Any param[%u];", paramCount);
            unsigned anyIndex = 1;
            if (hasBuffer)
            {
                std::string name = getBufferName(spec);
                writeln("param[%u] = reinterpret_cast<intptr_t>(%s);", anyIndex, name.c_str());
                ++anyIndex;
                if (!spec->isArray(node))
                {
                    writeln("param[%u] = %sLength;", anyIndex, name.c_str());
                    ++anyIndex;
                }
            }
            paramIndex = 0;
            for (NodeList::iterator i = node->begin();
                 paramIndex < getParamCount();
                 ++i, ++paramIndex)
            {
                ParamDcl* param = dynamic_cast<ParamDcl*>(*i);
                assert(param);
                Node* paramSpec = param->getSpec();
                if (param->isVariadic() || paramSpec->isSequence(node))
                {
                    writeln("param[%u] = reinterpret_cast<intptr_t>(%s);", anyIndex, param->getName().c_str());
                    ++anyIndex;
                    writeln("param[%u] = %sLength;", anyIndex, param->getName().c_str());
                    ++anyIndex;
                }
                else
                {
                    writeln("param[%u] = %s;", anyIndex, param->getName().c_str());
                    ++anyIndex;
                }
            }
            assert(anyIndex == paramCount);

            // Invoke
            if (spec->isVoid(node) || spec->isArray(node))
            {
                writeln("invoke(this, %s::iid(), B, %u, %u, param);",
                        currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else if (spec->isAny(node))
            {
                writeln("return invoke(this, %s::iid(), B, %u, %u, param);",
                        currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else if (spec->isInterface(node))
            {
                writetab();
                write("return dynamic_cast<");
                writeSpec(node);
                write(">(static_cast<Object*>(invoke(this, %s::iid(), B, %u, %u, param)));\n",
                      currentNode->getName().c_str(), methodNumber, paramCount);
            }
            else
            {
                writetab();
                write("return static_cast<");
                writeSpec(node);
                write(">(invoke(this, %s::iid(), B, %u, %u, param));\n",
                      currentNode->getName().c_str(), methodNumber, paramCount);
            }
        writeln("}");
        ++methodNumber;
    }

    virtual void at(const Member* node)
    {
        // To avoid typedef
    }

    virtual void at(const ExceptDcl* node)
    {
        // To avoid typedef
    }
};

void printTemplate(const char* source, const char* stringTypeName, const char* objectTypeName,
                   bool useExceptions, bool isystem, const char* indent)
{
    std::string filename = getOutputFilename(source, "template.h");
    printf("# %s\n", filename.c_str());

    FILE* file = fopen(filename.c_str(), "w");
    if (!file)
    {
        return;
    }

    std::string included = Cxx::getIncludedName(filename, indent);
    fprintf(file, "// Generated by esidl %s.\n\n", PACKAGE_VERSION);
    fprintf(file, "#ifndef %s\n", included.c_str());
    fprintf(file, "#define %s\n\n", included.c_str());

    // TODO: include header files here
    std::string header = getOutputFilename(source, "h");
    if (isystem)
    {
        fprintf(file, "#include <%s>\n\n", header.c_str());
    }
    else
    {
        fprintf(file, "#include \"%s\"\n\n", header.c_str());
    }

    TemplateVisitor visitor(source, file, stringTypeName, objectTypeName, useExceptions, indent);
    getSpecification()->accept(&visitor);

    fprintf(file, "#endif  // %s\n", included.c_str());

    fclose(file);
}
