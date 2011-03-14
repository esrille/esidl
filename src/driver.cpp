/*
 * Copyright 2010, 2011 Esrille Inc.
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

#include "esidl.h"
#include "meta.h"
#include "sheet.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>

namespace
{
    std::string baseFilename;
    std::string filename;
}

const std::string getBaseFilename()
{
    return baseFilename;
}

void setBaseFilename(const char* name)
{
    baseFilename = name;
    if (baseFilename[0] == '"')
    {
        baseFilename = baseFilename.substr(1, baseFilename.length() - 2);
    }
    setFilename(name);
}

const std::string getFilename()
{
    return filename;
}

void setFilename(const char* name)
{
    filename = name;
    if (filename == "\"<stdin>\"")
    {
        filename = getBaseFilename();
    }
    else if (filename[0] == '"')
    {
        filename = filename.substr(1, filename.length() - 2);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        return EXIT_FAILURE;
    }

    // Construct cpp options
    const char** argCpp = static_cast<const char**>(malloc(sizeof(char*) * (argc + 2)));
    if (!argCpp)
    {
        return EXIT_FAILURE;
    }

    int optCpp = 0;
    argCpp[optCpp++] = "cpp";
    argCpp[optCpp++] = "-C";  // Use -C for cpp by default.

    bool version = false;
    bool skeleton = false;
    bool generic = false;
    bool isystem = false;
    bool useExceptions = true;
    bool useVirtualBase = false;
    bool useMultipleInheritance = true;
    bool messenger = false;
    bool messengerSrc = false;
    bool java = false;
    bool cplusplus = false;
    bool cplusplusSrc = false;
    bool sheet = false;
    const char* stringTypeName = "char*";   // C++ string type name to be used
    const char* objectTypeName = "Object";  // C++ object type name to be used
    const char* indent = "es";

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'I')
            {
                argCpp[optCpp++] = argv[i];
                if (argv[i][2] == '\0')
                {
                    ++i;
                    argCpp[optCpp++] = argv[i];
                    setIncludePath(argv[i]);
                }
                else
                {
                    setIncludePath(&argv[i][2]);
                }
            }
            else if (strcmp(argv[i], "-messenger") == 0)
            {
                messenger = true;
            }
            else if (strcmp(argv[i], "-messenger-src") == 0)
            {
                messengerSrc = true;
            }
            else if (strcmp(argv[i], "-cplusplus") == 0)
            {
                cplusplus = true;
            }
            else if (strcmp(argv[i], "-cplusplus-src") == 0)
            {
                cplusplusSrc = true;
            }
            else if (strcmp(argv[i], "-fexceptions") == 0)
            {
                useExceptions = true;
            }
            else if (strcmp(argv[i], "-fno-exceptions") == 0)
            {
                useExceptions = false;
            }
            else if (strcmp(argv[i], "-fvirtualbase") == 0)
            {
                useVirtualBase = true;
            }
            else if (strcmp(argv[i], "-fno-virtualbase") == 0)
            {
                useVirtualBase = false;
            }
            else if (strcmp(argv[i], "-fmultipleinheritance") == 0)
            {
                useMultipleInheritance = true;
            }
            else if (strcmp(argv[i], "-fno-multipleinheritance") == 0)
            {
                useMultipleInheritance = false;
            }
            else if (strcmp(argv[i], "-include") == 0)
            {
                argCpp[optCpp++] = argv[i];
                ++i;
                argCpp[optCpp++] = argv[i];
            }
            else if (strcmp(argv[i], "-indent") == 0)
            {
                ++i;
                indent = argv[i];
            }
            else if (strcmp(argv[i], "-isystem") == 0)
            {
                argCpp[optCpp++] = argv[i];
                ++i;
                argCpp[optCpp++] = argv[i];
                setIncludePath(argv[i]);
                isystem = true;
            }
            else if (strcmp(argv[i], "-java") == 0)
            {
                java = true;
            }
            else if (strcmp(argv[i], "-namespace") == 0)
            {
                ++i;
                Node::setFlatNamespace(argv[i]);
            }
            else if (strcmp(argv[i], "-object") == 0)
            {
                ++i;
                objectTypeName = argv[i];
            }
            else if (strcmp(argv[i], "-prefix") == 0)
            {
                ++i;
                Node::setDefaultPrefix(argv[i]);
            }
            else if (strcmp(argv[i], "-template") == 0)
            {
                generic = true;
            }
            else if (strcmp(argv[i], "-sheet") == 0)
            {
                sheet = true;
            }
            else if (strcmp(argv[i], "-skeleton") == 0)
            {
                skeleton = true;
            }
            else if (strcmp(argv[i], "-string") == 0)
            {
                ++i;
                stringTypeName = argv[i];
            }
            else if (strcmp(argv[i], "--version") == 0)
            {
                version = true;
                break;
            }
        }
    }

    if (version)
    {
        printf("%s (r%s)\n", argv[0], SVN_REVISION);
        return 0;
    }

    argCpp[optCpp] = 0;

    // Set up the global module
    Module* node = new Module("");
    setSpecification(node);
    setCurrent(node);

    // Manually install 'object' interface forward declaration.
    Interface* object = new Interface("object", 0, true);
    object->setRank(2);
    getCurrent()->add(object);

    if (Node::getFlatNamespace())
    {
        Module* module = new Module(Node::getFlatNamespace());
        getCurrent()->add(module);
        setCurrent(module);
    }

    // Load every IDL file at once
    int result = EXIT_SUCCESS;
    int cppStream[2];
    pipe(cppStream);
    pid_t id = fork();
    if (id == 0)
    {
        // Child process
        int catStream[2];
        pipe(catStream);
        pid_t id = fork();
        if (id == 0)
        {
            // cat every IDL file
            close(catStream[0]);
            FILE* out = fdopen(catStream[1], "w");
            for (int i = 1; i < argc; ++i)
            {
                if (argv[i][0] == '-')
                {
                    if (strcmp(argv[i], "-I") == 0 ||
                        strcmp(argv[i], "-include") == 0 ||
                        strcmp(argv[i], "-indent") == 0 ||
                        strcmp(argv[i], "-isystem") == 0 ||
                        strcmp(argv[i], "-namespace") == 0 ||
                        strcmp(argv[i], "-object") == 0 ||
                        strcmp(argv[i], "-prefix") == 0 ||
                        strcmp(argv[i], "-string") == 0)
                    {
                        ++i;
                    }
                    continue;
                }

                if (const char* dot = strrchr(argv[i], '.'))
                {
                    if (strcasecmp(dot + 1, "idl"))
                    {
                        continue;
                    }
                }

                FILE* in = fopen(argv[i], "r");
                if (!in)
                {
                    return EXIT_FAILURE;
                }

                fprintf(out, "#pragma source \"%s\"\n", argv[i]);
                int ch;
                while ((ch = fgetc(in)) != EOF)
                {
                    putc(ch, out);
                }
                fclose(in);
            }
            fclose(out);
            return EXIT_SUCCESS;
        }
        else if (0 < id)
        {
            // execute cpp
            close(0);
            dup(catStream[0]);
            close(catStream[0]);
            close(catStream[1]);
            close(1);
            dup(cppStream[1]);
            close(cppStream[0]);
            close(cppStream[1]);
            execvp(argCpp[0], const_cast<char**>(argCpp));
            return EXIT_FAILURE;
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
    else if (0 < id)
    {
        // Parent process - process an IDL file
        close(cppStream[1]);
        if (input(cppStream[0], isystem, useExceptions, stringTypeName) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }
        close(cppStream[0]);

        int status;
        while (wait(&status) != id)
        {
        }
        if (result == EXIT_SUCCESS)
        {
            if (!WIFEXITED(status))
            {
                result = EXIT_FAILURE;
            }
            else
            {
                result = WEXITSTATUS(status);
            }
        }
    }
    else
    {
        return EXIT_FAILURE;
    }

    setBaseFilename("");

    if (java || cplusplus || cplusplusSrc || sheet)
    {
        Node::setCtorScope("_");
    }

    Interface::useMultipleInheritance = useMultipleInheritance;

    ProcessExtendedAttributes processExtendedAttributes;
    getSpecification()->accept(&processExtendedAttributes);

    AdjustMethodCount adjustMethodCount(!useMultipleInheritance || Node::getFlatNamespace());
    getSpecification()->accept(&adjustMethodCount);

    Meta meta(objectTypeName);
    getSpecification()->accept(&meta);

    if (messenger)
    {
        result = printMessenger(stringTypeName, objectTypeName,
                                useExceptions, useVirtualBase, indent);
    }
    else if (messengerSrc)
    {
        result = printMessengerSrc(stringTypeName, objectTypeName,
                                    useExceptions, useVirtualBase, indent);
    }
    else if (java)
    {
        result = printJava(indent);
    }
    else if (cplusplus)
    {
        result = printCPlusPlus(stringTypeName, objectTypeName,
                                useExceptions, useVirtualBase, indent);
    }
    else if (cplusplusSrc)
    {
        result = printCPlusPlusSrc(stringTypeName, objectTypeName,
                                   useExceptions, useVirtualBase, indent);
    }
    else if (sheet)
    {
        Sheet visitor;
        getSpecification()->accept(&visitor);
        result = EXIT_SUCCESS;
    }
    else
    {
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i][0] == '-')
            {
                if (strcmp(argv[i], "-I") == 0 ||
                    strcmp(argv[i], "-include") == 0 ||
                    strcmp(argv[i], "-indent") == 0 ||
                    strcmp(argv[i], "-isystem") == 0 ||
                    strcmp(argv[i], "-namespace") == 0 ||
                    strcmp(argv[i], "-object") == 0 ||
                    strcmp(argv[i], "-prefix") == 0 ||
                    strcmp(argv[i], "-string") == 0)
                {
                    ++i;
                }
                continue;
            }
            result = output(argv[i], isystem, useExceptions, useVirtualBase,
                            stringTypeName, objectTypeName, indent,
                            skeleton, generic);
        }
    }
    return result;
}
