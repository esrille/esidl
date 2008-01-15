/*
 * Copyright 2008 Google Inc.
 * Copyright 2006, 2007 Nintendo Co., Ltd.
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

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <es/endian.h>
#include "guid.h"

namespace
{
    const int GuidStringLength = 37;        // Including terminating zero
}

bool parseGuid(const char* str, Guid* u)
{
    int x;
    int b;
    u8* p = (u8*) u;
    int i;
    int j;

    // 0         1         2         3
    // 012345678901234567890123456789012345
    // 2772cebc-0e69-4582-ae3c-c151bf5a9a55
    for (i = 0, j = 0; i < GuidStringLength - 1; ++i)
    {
        switch (i)
        {
          case 8: case 13: case 18: case 23:
            if (str[i] != '-')
            {
                return false;
            }
            continue;
            break;
          default:
            break;
        }

        x = str[i];
        if (!isxdigit(x))
        {
            return false;
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

        if (j & 1)
        {
            b = (b << 4) | x;
            *p++ = (u8) b;
        }
        else
        {
            b = x;
        }
        ++j;
        assert(b < 256);
    }
    assert(j == 32);

    u->Data1 = ntohl(u->Data1);
    u->Data2 = ntohs(u->Data2);
    u->Data3 = ntohs(u->Data3);

    return true;
}

void printGuid(const Guid& guid)
{
    printf("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
           guid.Data1, guid.Data2, guid.Data3,
           guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
           guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
}
