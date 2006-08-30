/*
 * Copyright (c) 2006
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
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <es.h>
#include <es/reflect.h>
#include <es/types.h>

void printGuid(Guid* guid)
{
    printf("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
             guid->Data1, guid->Data2, guid->Data3,
             guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
             guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);
}

const char* Reflect::typeID[ReflectionFile::TAG_MAX] =
{
    "signed char",
    "short",
    "int",
    "long long",
    "unsigned char",
    "unsigned short",
    "unsigned int",
    "unsigned long long",
    "float",
    "double",
    "bool",
    "char",
    "wchar_t",
    "void",
    "Guid"
};

void printType(Reflect::Type type)
{
    if (type.isConst())
    {
        printf("const ");
    }

    printf("%s", type.getName());

    if (type.getPointer() == 1)
    {
        printf("*");
    }
    else if (type.getPointer() == 2)
    {
        printf("**");
    }

    if (type.isReference())
    {
        printf("&");
    }

    // We also need the byte count of each parameter.
    printf(" {%d}", type.getSize());
}

char* indent = "";

int main(int argc, char* argv[])
{
    int size;
    int i;
    char* buf = 0;

    for (i = 1; i < argc; ++i)
    {

        int fd = open(argv[i], O_RDONLY);
        if (fd < 0)
        {
            continue;
        }

        struct stat s;
        if (fstat(fd, &s) < 0)
        {
            continue;
        }
        void* buf = malloc(s.st_size);
        if (!buf)
        {
            continue;
        }
        read(fd, buf, s.st_size);
        close(fd);

        Reflect r(buf);
        for (int i = 0; i < r.getInterfaceCount(); ++i)
        {
            Reflect::Interface interface(r.getInterface(i));
            printf("interface %s\n", interface.getName());
            printf("    iid: ");
            printGuid(interface.getIid());
            printf("\n");

            if (interface.getType().isImported())
            {
                continue;
            }

            if (interface.getSuperIid())
            {
                printf("    piid: ");
                printGuid(interface.getSuperIid());
                printf("\n");
                printf("    method count: %d\n", interface.getTotalMethodCount());
            }

            indent = "    ";
            for (int i = 0; i < interface.getMethodCount(); ++i)
            {
                Reflect::Function method(interface.getMethod(i));

                printf("%sfunction %s : ", indent, method.getName());
                printType(method.getReturnType());
                if (method.getReturnType().isInterfacePointer())
                {
                    printf(" interface_pointer");
                    printf(": ");
                    printGuid(method.getReturnType().getInterface().getIid());
                }

                printf("\n");

                indent = "        ";
                for (int i = 0; i < method.getParameterCount(); ++i)
                {
                    Reflect::Identifier param(method.getParameter(i));
                    printf("%s%s : ", indent, param.getName());
                    printType(param.getType());
                    if (param.getType().isArray())
                    {
                    }
                    else
                    {
                        if (0 <= param.getIidIs())
                        {
                            printf(" iid_is(%s)", method.getParameter(param.getIidIs()).getName());
                        }
                        if (0 <= param.getSizeIs())
                        {
                            printf(" size_is(%s)", method.getParameter(param.getSizeIs()).getName());
                        }
                    }

                    if (param.isInterfacePointer())
                    {
                        printf(" interface_pointer");
                        // We also need to know the IID of the interface.
                        if (param.getType().isInterfacePointer())
                        {
                            printf(": ");
                            printGuid(param.getType().getInterface().getIid());
                        }
                    }

                    printf("\n");
                }
                indent = "    ";
            }
            printf("%smember count: %d\n", indent, interface.getMemberCount());
            indent = "";
        }
        for (int i = 0; i < r.getTypeCount(); ++i)
        {
            Reflect::Identifier type(r.getIdentifier(i));
            printf("%s : ", type.getName());
            printType(type.getType());
            printf("\n");
        }

        free(buf);
    }
}
