/*
 * Copyright 2008 Google Inc.
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

#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        return EXIT_FAILURE;
    }

    char** argCpp = static_cast<char**>(malloc(sizeof(char*) * (argc + 1)));
    char** argIdl = static_cast<char**>(malloc(sizeof(char*) * (argc + 1)));

    argCpp[0] = "cpp";
    argIdl[0] = "esidl2";
    int optCpp = 1;
    int optIdl = 1;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] == 'I')
            {
                argCpp[optCpp++] = argv[i];
                argIdl[optIdl++] = argv[i];
                if (argv[i][2] == '\0')
                {
                    ++i;
                    argCpp[optCpp++] = argv[i];
                    argIdl[optIdl++] = argv[i];
                }
            }
            else if (strcmp(argv[i], "-debug") == 0)
            {
                argIdl[optIdl++] = argv[i];
            }
            else if (strcmp(argv[i], "-include") == 0)
            {
                argCpp[optCpp++] = argv[i];
                ++i;
                argCpp[optCpp++] = argv[i];
            }
            else if (strcmp(argv[i], "-object") == 0)
            {
                argIdl[optIdl++] = argv[i];
                ++i;
                argIdl[optIdl++] = argv[i];
            }
            else
            {
                argCpp[optCpp++] = argv[i];
            }
        }
    }

    argCpp[optCpp + 1] = 0;
    argIdl[optIdl] = 0;

    int result = EXIT_SUCCESS;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-I") == 0 ||
                strcmp(argv[i], "-include") == 0 ||
                strcmp(argv[i], "-object") == 0)
            {
                ++i;
            }
            continue;
        }

        argCpp[optCpp] = argv[i];
        pid_t id = fork();
        if (id == 0)
        {
            int stream[2];
            pipe(stream);

            if (fork() == 0)
            {
                // Child process - execute cpp
                close(1);
                dup(stream[1]);
                close(stream[0]);
                close(stream[1]);
                execvp(argCpp[0], argCpp);
                break;
            }
            else
            {
                // Parent process - execute esidl2
                close(0);
                dup(stream[0]);
                close(stream[0]);
                close(stream[1]);
                execvp(argIdl[0], argIdl);
                break;
            }
        }
        else if (0 < id)
        {
            int status;

            while (wait(&status) != id)
            {
            }
            if (result == EXIT_SUCCESS &&
                WIFEXITED(status) &&
                WEXITSTATUS(status) != EXIT_SUCCESS)
            {
                result = WEXITSTATUS(status);
            }
        }
        else
        {
            return EXIT_FAILURE;
        }
    }

    return result;
}
