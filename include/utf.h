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

#ifndef ESIDL_UTF_H_INCLUDED
#define ESIDL_UTF_H_INCLUDED

#include <ctype.h>
#include <string>

inline char* utf8to32(const char* utf8, uint32_t* utf32)
{
    uint32_t u = 0;
    uint8_t  c;
    unsigned len;
    unsigned i;

    c = (uint8_t) *utf8;
    if (c != '\0')
    {
        ++utf8;
    }
    if ((c & 0x80u) == 0x00u)
    {
        u = c;
        len = 0;
    }
    else if ((c & 0xe0u) == 0xc0u)
    {
        u = c & 0x1fu;
        len = 1;
    }
    else if ((c & 0xf0u) == 0xe0u)
    {
        u = c & 0x0fu;
        len = 2;
    }
    else if ((c & 0xf8u) == 0xf0u)
    {
        u = c & 0x07u;
        len = 3;
    }
    else
    {
        return NULL;
    }

    for (i = 0; i < len; ++i)
    {
        u <<= 6;
        c = (uint8_t) *utf8++;
        if ((c & 0xc0u) != 0x80u)
        {
            return NULL;
        }
        u |= (c & 0x3fu);
    }

    if (u <= 0x7f)
    {
        if (len != 0)
        {
            return NULL;
        }
    }
    else if (u <= 0x7ff)
    {
        if (len != 1)
        {
            return NULL;
        }
    }
    else if (u <= 0xffff)
    {
        if (len != 2)
        {
            return NULL;
        }
    }

    if (0xD800 <= u && u <= 0xDFFF) // Surrogate Area
    {
        return NULL;
    }

    *utf32 = u;
    return (char*) utf8;
}

inline char* utf32to8(uint32_t utf32, char* utf8)
{
    int len;

    if (0xD800 <= utf32 && utf32 <= 0xDFFF) // Surrogate Area
    {
        return NULL;
    }

    if (utf32 < 0x80)
    {
        *utf8++ = (char) utf32;
        len = 0;
    }
    else if (utf32 < 0x800)
    {
        *utf8++ = (char) (0xc0 | (utf32 >> 6));
        len = 1;
    }
    else if (utf32 < 0x10000)
    {
        *utf8++ = (char) (0xe0 | (utf32 >> 12));
        len = 2;
    }
    else if (utf32 < 0x110000)
    {
        *utf8++ = (char) (0xf0 | (utf32 >> 18));
        len = 3;
    }
    else
    {
        return NULL;
    }

    while (0 < len--)
    {
        *utf8++ = (char) (0x80 | ((utf32 >> (6 * len)) & 0x3f));
    }
    return utf8;
}

inline size_t parseHex(const std::string& str, size_t pos, int limit, uint32_t& hex)
{
    hex = 0;
    for (int i = 0; i < limit; ++i, ++pos)
    {
        uint8_t x = str[pos];
        if (!isxdigit(x))
        {
            break;
        }
        if (isdigit(x))
        {
            x -= '0';
        }
        else
        {
            x = tolower(x);
            x -= 'a';
            x += 10;
        }
        hex <<= 4;
        hex |= x;
    }
    return pos;
}

inline std::string unescape(const std::string s, size_t pos, size_t n)
{
    std::string value;

    while (0 < n--)
    {
        char c = s[pos++];
        if (c != '\\')
        {
            value += c;
        }
        else
        {
            // Process escape sequence
            uint32_t ucode;
            char     utf8[5];
            char*    t;

            c = s[pos++];
            switch (c)
            {
            case '\'':
            case '"':
            case '\\':
                value += c;
                break;
            case 'b':
                value += '\b';
                break;
            case 'f':
                value += '\f';
                break;
            case 'n':
                value += '\n';
                break;
            case 'r':
                value += '\r';
                break;
            case 't':
                value += '\t';
                break;
            case 'v':
                value += '\v';
                break;
            case '0':
                value += '\0';
                break;
            case 'x':
                pos = parseHex(s, pos, 2, ucode);
                t = utf32to8(ucode, utf8);
                if (t)
                {
                    *t = 0;
                    value += utf8;
                }
                break;
            case 'u':
                pos = parseHex(s, pos, 4, ucode);
                t = utf32to8(ucode, utf8);
                if (t)
                {
                    *t = 0;
                    value += utf8;
                }
                break;
            default:
                value += '\\';
                value += c;
                break;
            }
        }
    }
    return value;
}

#endif  // ESIDL_UTF_H_INCLUDED
