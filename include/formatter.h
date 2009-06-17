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

#ifndef NINTENDO_ESIDL_FORMATTER_H_INCLUDED
#define NINTENDO_ESIDL_FORMATTER_H_INCLUDED

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

class Formatter
{
    FILE*       file;
    int         tabWidth;
    std::string indentString;

public:
    Formatter(FILE* file, int tabWidth = 4) :
        file(file),
        tabWidth(tabWidth)
    {
    }

    void indent()
    {
        indentString += std::string(tabWidth, ' ');
    }

    void unindent()
    {
        indentString.erase(indentString.length() - tabWidth);
    }

    void write(const char* format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vfprintf(file, format, ap);
        va_end(ap);
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
        vfprintf(file, format, ap);
        va_end(ap);
        write("\n");
    }
};

#endif  // NINTENDO_ESIDL_FORMATTER_H_INCLUDED
