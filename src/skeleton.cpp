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
#include <map>
#include <set>

namespace
{

std::string getFileName(const std::string& name)
{
    std::string lowered(name);

    for (int i = 0; i < lowered.size(); ++i)
    {
        char c = lowered[i];
        lowered[i] = tolower(c);
        if (c == '.' || c == '/' || c == '\\')
        {
            lowered[i] = '_';
        }
    }
    return lowered;
}

}  // namespace

class SkeletonImpl : public CPlusPlus
{
public:
    SkeletonImpl(FILE* file) :
        CPlusPlus(file)
    {
    }

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf() || node->isBaseObject())
        {
            return;
        }

        std::list<const Interface*> interfaceList;
        node->collectMixins(&interfaceList);

        write("class %s_Impl : ", node->getName().c_str());
        for (std::list<const Interface*>::const_iterator i = interfaceList.begin();
             i != interfaceList.end();
             ++i)
        {
            if (i != interfaceList.begin())
            {
                write(", ");
            }
            write("public %s", getScopedName(moduleName, (*i)->getQualifiedName()).c_str());
        }
        write("\n");

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
        writeln("%s_Impl();", node->getName().c_str());

        // Destructor
        writeln("~%s_Impl();", node->getName().c_str());
        writeln("");

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
#ifdef USE_FUNCTION_CALLBACK
                            callbackStage = 0;
                            do
                            {
                                optionalCount = 0;
                                callbackCount = 0;
                                op->accept(this);
                                ++callbackStage;
                            } while (callbackStage < (1u << callbackCount));
#else  // USE_FUNCTION_CALLBACK
                            optionalCount = 0;
                            op->accept(this);
#endif  // USE_FUNCTION_CALLBACK
                            ++optionalStage;
                        } while (optionalStage <= optionalCount);
                    }
                }
            }
        }
        currentNode = node;

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
        write(";\n");

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
        write(";\n");
    }

    virtual void at(const OpDcl* node)
    {
        Interface* interface = dynamic_cast<Interface*>(node->getParent());
        assert(interface);

        writetab();
        CPlusPlus::at(node);
        write(";\n");
    }

    virtual void at(const StructType* node)
    {
    }

    virtual void at(const Member* node)
    {
    }
};

class SkeletonVisitor : public Visitor
{
    const char* idlFilename;
    bool isystem;

    std::string moduleName;

public:
    SkeletonVisitor(const char* idlFilename, bool isystem) :
        idlFilename(idlFilename),
        isystem(isystem)
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

    virtual void at(const Interface* node)
    {
        if (1 < node->getRank() || node->isLeaf())
        {
            return;
        }

        if (!node->getMixins()->empty())
        {
            // An interface with ImplementedOn attribute won't need its own implementation.
            return;
        }

        std::string filename = idlFilename;
        int begin = filename.rfind(".");
        filename.insert(begin, getFileName(node->getName()));
        filename.insert(begin, "_");
        filename = getOutputFilename(filename.c_str(), "skeleton.h");
        printf("# %s in %s\n", filename.c_str(), moduleName.c_str());

        FILE* file = fopen(filename.c_str(), "w");
        if (!file)
        {
            return;
        }

        std::string included = getIncludedName(filename);
        fprintf(file, "// Generated by esidl %s.\n\n", VERSION);
        fprintf(file, "#ifndef %s\n", included.c_str());
        fprintf(file, "#define %s\n\n", included.c_str());

        if (isystem)
        {
            fprintf(file, "#include <%s>\n\n", getOutputFilename(idlFilename, "h").c_str());
        }
        else
        {
            fprintf(file, "#include \"%s\"\n\n", getOutputFilename(idlFilename, "h").c_str());
        }

        size_t pos = 0;
        while ((pos = moduleName.find("::", pos)) != std::string::npos)
        {
            size_t next = moduleName.find("::", pos + 2);
            fprintf(file, "namespace %s\n{\n\n",
                    moduleName.substr(pos + 2, (next == std::string::npos) ? std::string::npos : next - pos).c_str());
            pos += 2;
        }

        SkeletonImpl SkeletonImpl(file);
        SkeletonImpl.at(node);
        fprintf(file, ";\n");

        pos = std::string::npos;
        while (pos && (pos = moduleName.rfind("::", pos)) != std::string::npos)
        {
            size_t next = moduleName.find("::", pos + 2);
            fprintf(file, "\n}  // namespace %s\n",
                    moduleName.substr(pos + 2, (next == std::string::npos) ? std::string::npos : next - pos).c_str());
        }

        fprintf(file, "\n#endif  // %s\n", included.c_str());

        fclose(file);
    }
};

void printSkeleton(const char* idlFilename, bool isystem)
{
    SkeletonVisitor visitor(idlFilename, isystem);
    getSpecification()->accept(&visitor);
}
