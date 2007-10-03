/*
 * Copyright (c) 2007
 * Nintendo Co., Ltd.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Nintendo makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
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

    int opt = 1;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            argCpp[opt] = argIdl[opt] = argv[i];
            ++opt;
            if (strcmp(argv[i], "-I") == 0)
            {
                argCpp[opt] = argIdl[opt] = argv[++i];
                ++opt;
            }
        }
    }

    if (argc == opt)
    {
        return EXIT_FAILURE;
    }

    argCpp[opt + 1] = 0;
    argIdl[opt] = 0;

    int result = EXIT_SUCCESS;

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            if (strcmp(argv[i], "-I") == 0)
            {
                ++i;
            }
        }
        else
        {
            argCpp[opt] = argv[i];
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
    }

    return result;
}
