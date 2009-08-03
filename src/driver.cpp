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

#include "esidl.h"

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

    bool skeleton = false;
    bool generic = false;
    bool isystem = false;
    bool useExceptions = true;
    const char* stringTypeName = "char*";   // C++ string type name to be used
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
            else if (strcmp(argv[i], "-fexceptions") == 0)
            {
                useExceptions = true;
            }
            else if (strcmp(argv[i], "-fno-exceptions") == 0)
            {
                useExceptions = false;
            }
            else if (strcmp(argv[i], "-include") == 0)
            {
                argCpp[optCpp++] = argv[i];
                ++i;
                argCpp[optCpp++] = argv[i];
            }
            else if (strcmp(argv[i], "-isystem") == 0)
            {
                argCpp[optCpp++] = argv[i];
                ++i;
                argCpp[optCpp++] = argv[i];
                setIncludePath(argv[i]);
                isystem = true;
            }
            else if (strcmp(argv[i], "-namespace") == 0)
            {
                ++i;
                Node::setFlatNamespace(argv[i]);
            }
            else if (strcmp(argv[i], "-object") == 0)
            {
                ++i;
                Node::setBaseObjectName(argv[i]);
            }
            else if (strcmp(argv[i], "-template") == 0)
            {
                generic = true;
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
            else if (strcmp(argv[i], "-indent") == 0)
            {
                ++i;
                indent = argv[i];
            }
        }
    }
    argCpp[optCpp] = 0;

    // Set up the global module
    Module* node = new Module("");
    setSpecification(node);
    setCurrent(node);

    if (strcmp(Node::getBaseObjectName(), "::Object") == 0)
    {
        // Manually install 'Object' interface forward declaration.
        Interface* object = new Interface("Object", 0, true);
        object->setRank(2);
        getCurrent()->add(object);
    }

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
                        strcmp(argv[i], "-isystem") == 0 ||
                        strcmp(argv[i], "-namespace") == 0 ||
                        strcmp(argv[i], "-object") == 0 ||
                        strcmp(argv[i], "-string") == 0 ||
                        strcmp(argv[i], "-indent") == 0)
                    {
                        ++i;
                    }
                    continue;
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

    ProcessExtendedAttributes processExtendedAttributes;
    getSpecification()->accept(&processExtendedAttributes);

    AdjustMethodCount adjustMethodCount;
    getSpecification()->accept(&adjustMethodCount);

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-I") == 0 ||
                strcmp(argv[i], "-include") == 0 ||
                strcmp(argv[i], "-isystem") == 0 ||
                strcmp(argv[i], "-namespace") == 0 ||
                strcmp(argv[i], "-object") == 0 ||
                strcmp(argv[i], "-string") == 0)
            {
                ++i;
            }
            continue;
        }
        result = output(argv[i], isystem, useExceptions, stringTypeName, indent,
                        skeleton, generic);
    }

    return result;
}
