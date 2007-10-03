/*
 * Copyright (c) 2006, 2007
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
