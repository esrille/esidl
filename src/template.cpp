/*
 * Copyright 2009 Google Inc.
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

#include <set>

class TemplateVisitor : public CPlusPlus
{
    unsigned methodNumber;

    // TODO: Move to CPlusPlus later
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
    TemplateVisitor(const char* source, FILE* file, const char* stringTypeName = "char*", bool useExceptions = true) :
        CPlusPlus(source, file, stringTypeName, useExceptions)
    {
    }

    virtual void at(const Interface* node)
    {
        if (!node->isDefinedIn(source) || node->isLeaf() || node->isBaseObject())
        {
            return;
        }

        std::list<const Interface*> interfaceList;
        node->collectMixins(&interfaceList);

        write("template<class O, Any invoke(O&, unsigned, unsigned, Any*)>\n");
        writetab();
        write("class %s_Proxy : public Proxy_Impl<O", node->getName().c_str());
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            write(", %s", getScopedName(moduleName, (*i)->getQualifiedName()).c_str());
        }
        write(">\n");

        writeln("{");
        indent();

        std::set<std::string> idSet;
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            for (const Interface* interface = *i;
                 interface && !interface->isBaseObject();
                 interface = interface->getSuper())
            {
                for (NodeList::iterator i = interface->begin(); i != interface->end(); ++i)
                {
                    if (Attribute* attr = dynamic_cast<Attribute*>(*i))
                    {
                        idSet.insert(attr->getName());
                    }
                    else if (OpDcl* op = dynamic_cast<OpDcl*>(*i))
                    {
                        if ((interface->getAttr() & Interface::NoIndexingOperations) &&
                            (op->getAttr() & (OpDcl::IndexMask | OpDcl::NameMask)))
                        {
                            continue;
                        }
                        idSet.insert(op->getName());
                    }
                }
            }
        }

        unindent();
        writeln("public:");
        indent();

        // Constructor
        writeln("%s_Proxy(O object) :", node->getName().c_str());
        indent();
            writetab();
            write("Proxy_Impl<O");
            for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
                i != interfaceList.end();
                ++i)
            {
                write(", %s", getScopedName(moduleName, (*i)->getQualifiedName()).c_str());
            }
            write(">(object)\n");
        unindent();
        writeln("{");
        writeln("}");

        // Destructor
        writeln("~%s_Proxy()", node->getName().c_str());
        writeln("{");
        writeln("}");

        methodNumber = 0;
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            for (const Interface* interface = *i;
                 interface && !interface->isLeaf();
                 interface = interface->getSuper())
            {
                currentNode = interface;
                for (NodeList::iterator i = interface->begin(); i != interface->end(); ++i)
                {
                    if (Attribute* attr = dynamic_cast<Attribute*>(*i))
                    {
                        attr->accept(this);
                    }
                    else if (OpDcl* op = dynamic_cast<OpDcl*>(*i))
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
        }
        currentNode = node;

        // createInstance
        writeln("static %s* createInstance(O object)", node->getName().c_str());
        writeln("{");
        indent();
            writeln("return new %s_Proxy(object);", node->getName().c_str());
        unindent();
        writeln("}");

        unindent();
        writetab();
        write("}");
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
        CPlusPlus::getter(node);
        writeln("");
        writeln("{");
        indent();
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
                writeln("param[%u] = %s;", anyIndex, name.c_str());
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
                writeln("invoke(this->object, %u, %u, param);",
                        methodNumber, paramCount);
            }
            else if (spec->isAny(node))
            {
                writeln("return invoke(this->object, %u, %u, param);",
                        methodNumber, paramCount);
            }
            else if (spec->isInterface(node))
            {
                writetab();
                write("return static_cast<");
                writeSpec(node);
                write(">(static_cast<Object*>(invoke(this->object, %u, %u, param)));\n",
                      methodNumber, paramCount);
            }
            else
            {
                writetab();
                write("return static_cast<");
                writeSpec(node);
                write(">(invoke(this->object, %u, %u, param));\n",
                      methodNumber, paramCount);
            }
        }
        unindent();
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
        CPlusPlus::setter(node);
        writeln("");
        writeln("{");
        indent();
        {
            unsigned paramCount = 2;  // +1 for 'this'
            if (spec->isSequence(node))
            {
                ++paramCount;  // for the length
            }
            writeln("Any param[%u];", paramCount);
            unsigned anyIndex = 1;
            std::string name = getBufferName(node);
            writeln("param[%u] = %s;", anyIndex, name.c_str());
            ++anyIndex;
            if (spec->isSequence(node))
            {
                writeln("param[%u] = %sLength;", anyIndex, name.c_str());
                ++anyIndex;
            }

            // Invoke
            if (spec->isSequence(node))
            {
                writeln("return static_cast<int>(invoke(this->object, %u, %u, param));",
                        methodNumber, paramCount);
            }
            else
            {
                writeln("invoke(this->object, %u, %u, param);",
                        methodNumber, paramCount);
            }
        }
        unindent();
        writeln("}");
        ++methodNumber;
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writetab();
        CPlusPlus::at(node);
        writeln("");
        writeln("{");
        indent();
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
                writeln("param[%u] = %s;", anyIndex, name.c_str());
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
                writeln("param[%u] = %s;", anyIndex, param->getName().c_str());
                ++anyIndex;
                if (param->isVariadic() || paramSpec->isSequence(node))
                {
                    writeln("param[%u] = %sLength;", anyIndex, param->getName().c_str());
                    ++anyIndex;
                }
            }
            assert(anyIndex == paramCount);

            // Invoke
            if (spec->isVoid(node) || spec->isArray(node))
            {
                writeln("invoke(this->object, %u, %u, param);",
                        methodNumber, paramCount);
            }
            else if (spec->isAny(node))
            {
                writeln("return invoke(this->object, %u, %u, param);",
                        methodNumber, paramCount);
            }
            else if (spec->isInterface(node))
            {
                writetab();
                write("return static_cast<");
                writeSpec(node);
                write(">(static_cast<Object*>(invoke(this->object, %u, %u, param)));\n",
                      methodNumber, paramCount);
            }
            else
            {
                writetab();
                write("return static_cast<");
                writeSpec(node);
                write(">(invoke(this->object, %u, %u, param));\n",
                      methodNumber, paramCount);
            }
        unindent();
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

void printTemplate(const char* source, const char* stringTypeName, bool useExceptions, bool isystem)
{
    std::string filename = getOutputFilename(source, "template.h");
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

    TemplateVisitor visitor(source, file, stringTypeName, useExceptions);
    getSpecification()->accept(&visitor);

    fprintf(file, "#endif  // %s\n", included.c_str());

    fclose(file);
}
