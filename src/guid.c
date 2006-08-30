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

#include <ctype.h>
#include <stdio.h>
#include <es/uuid.h>

#define UUID_STR_LEN    37  // Including terminating zero

char* UuidToAscii(const Guid* u, char* str)
{
    int i;
    char* ptr = str;

    ptr += sprintf(ptr, "%8.8x-%4.4x-%4.4x-%2.2x%2.2x-",
                   u->Data1,
                   u->Data2,
                   u->Data3,
                   u->Data4[0],
                   u->Data4[1]);
    for (i = 2; i < 8; i++)
    {
        ptr += sprintf(ptr, "%2.2x", u->Data4[i]);
    }
    return str;
}

int ScanUuid(const char* str, Guid* u)
{
    const char* p = str;
    u32 x;
    u32 d;
    int i;
    int j;

    // 0         1         2         3
    // 012345678901234567890123456789012345
    // 83ba18aa-3e2e-425f-b6ee-0a2e46ea1d9a
    for (i = 0, j = 0, d = 0; i < UUID_STR_LEN; ++i, ++p)
    {
        x = *p;

        switch (i)
        {
          case 8:
            if (x != '-')
            {
                return 0;
            }
            u->Data1 = d;
            d = 0;
            continue;
          case 13:
            if (x != '-')
            {
                return 0;
            }
            u->Data2 = (u16) d;
            d = 0;
            continue;
          case 18:
            if (x != '-')
            {
                return 0;
            }
            u->Data3 = (u16) d;
            d = 0;
            continue;
          case 23:
            if (x != '-')
            {
                return 0;
            }
            u->Data4[j++] = (u8) d;
            d = 0;
            continue;
          case 21: case 26: case 28: case 30: case 32: case 34:
            u->Data4[j++] = (u8) d;
            d = 0;
            break;
          case 36:
            u->Data4[j++] = (u8) d;
            d = 0;
            continue;
          default:
            break;
        }

        if (!isxdigit(x))
        {
            return 0;
        }
        if (isdigit(x))
        {
            x -= '0';
        }
        else
        {
            x = tolower(x) - 'a' + 10;
        }
        d = (d << 4) | x;
    }
    return 1;
}
