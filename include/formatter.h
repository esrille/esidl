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

#ifndef ESIDL_FORMATTER_H_INCLUDED
#define ESIDL_FORMATTER_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class Formatter
{
    FILE*       file;
    std::string indentString;

    // Format options
    const unsigned indentLevel;  // 4, 2, 4
    const int caseIndentation; // 0, 1
    const bool namespaceIndentation;  // true/false, false, false
    const bool bracesOnFuncDeclLine;  // false, true, false (precedes bracesOnItsOwnLine)
    const bool bracesOnItsOwnLine;  // flase, false, true

    static const int BufferSize = 4096;
    char buffer[BufferSize];
    char* bufferPointer;

    void initialize()
    {
        bufferPointer = buffer;
        *bufferPointer = '\0';
    }

    void breakLineAt(char c)
    {
        assert(buffer < bufferPointer);
        const char* p = bufferPointer - 1;
        while (buffer < p && *p != c)
        {
            --p;
        }
        if (*p == c)
        {
            do {
                if ((std::isspace)(*(p - 1)))
                {
                    --p;
                }
                else
                {
                    break;
                }
            } while (buffer < p);
        }
        fprintf(file, "%.*s\n", p - buffer, buffer);
        fprintf(file, "%s", indentString.c_str());
        fprintf(file, "%c", c);
        if (*(bufferPointer - 1) == '\n')
        {
            fprintf(file, "\n");
        }
    }

public:
    Formatter(FILE* file, unsigned indentLevel = 4) :
        file(file),
        indentLevel(indentLevel),
        caseIndentation(0),
        namespaceIndentation(false),
        bracesOnItsOwnLine(true),
        bracesOnFuncDeclLine(false)
    {
        initialize();
    }

    Formatter(const Formatter* f) :
        file(f->file),
        indentString(f->indentString),
        indentLevel(f->indentLevel),
        caseIndentation(f->caseIndentation),
        namespaceIndentation(f->namespaceIndentation),
        bracesOnItsOwnLine(f->bracesOnItsOwnLine),
        bracesOnFuncDeclLine(f->bracesOnFuncDeclLine)
    {
        initialize();
    }

    ~Formatter()
    {
    }

    void indent()
    {
        indentString += std::string(indentLevel, ' ');
    }

    void unindent()
    {
        indentString.erase(indentString.length() - indentLevel);
    }

    void vwrite(const char* format, va_list ap)
    {
        vsnprintf(bufferPointer, BufferSize - (bufferPointer - buffer), format, ap);
        bufferPointer = buffer + strlen(buffer);
        assert(bufferPointer - buffer <= BufferSize);
    }

    void write(const char* format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vwrite(format, ap);
        va_end(ap);
        if (buffer < bufferPointer && *(bufferPointer - 1) == '\n')
        {
            flush();
        }
    }

    void writetab()
    {
        write("%s", indentString.c_str());
    }

    void writeln(const char* format, ...)
    {
        if (*format)
        {
            writetab();
        }
        va_list ap;
        va_start(ap, format);
        vwrite(format, ap);
        va_end(ap);
        write("\n");
        flush();
    }

    void flush()
    {
        if (bufferPointer <= buffer)
        {
            return;
        }

        const char* tail = bufferPointer - 1;
        if (*tail == '\n' && (buffer + 1) < bufferPointer)
        {
            --tail;
        }
        const char* head;
        for (head = buffer; head < bufferPointer && std::isspace(*head); ++head)
        {
        }

        if (strncmp(head, "/*", 2) == 0 || strncmp(head, "//", 10) == 0)
        {
            fputs(buffer, file);
        }
        else if (*tail == ':' && buffer < tail && !std::isspace(*(tail - 1)) && 0 < caseIndentation)
        {
            fprintf(file, "%s", std::string(caseIndentation, ' ').c_str());
            fputs(buffer, file);
        }
        else if (*tail == '{')
        {
            if (strncmp(head, "namespace ", 10) == 0)
            {
                if (bracesOnItsOwnLine)
                {
                    breakLineAt('{');
                }
                else
                {
                    fputs(buffer, file);
                }
                // TODO: check indent
            }
            else if (strncmp(head, "class ", 6) == 0 || strncmp(head, "struct ", 7) == 0)
            {
                if (bracesOnItsOwnLine)
                {
                    breakLineAt('{');
                }
                else
                {
                    fputs(buffer, file);
                }
            }
            else if (!bracesOnFuncDeclLine)
            {
                breakLineAt('{');
            }
            else
            {
                fputs(buffer, file);
            }
        }
        else
        {
            fputs(buffer, file);
        }
        bufferPointer = buffer;
        *bufferPointer = 0;
    }
};

#endif  // ESIDL_FORMATTER_H_INCLUDED
