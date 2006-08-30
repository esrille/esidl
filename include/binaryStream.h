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

#include <assert.h>
#include <es/uuid.h>

class BinaryStream
{
    std::ostream& out;

    unsigned offset;

public:
    BinaryStream(std::ostream& out) :
        out(out), offset(0)
    {

    }

    void resetOffset(void)
    {
        offset = 0;
    }

    unsigned getOffset(void)
    {
        return offset;
    }

    void write(const void* value, size_t len)
    {
        out.write(static_cast<const char*>(value), len);
        offset += len;
    }

    void write(u16 value)
    {
        write(&value, sizeof(u16));
    }

    void write(u32 value)
    {
        write(&value, sizeof(u32));
    }

    void write(u64 value)
    {
        write(&value, sizeof(u64));
    }

    void write(s16 value)
    {
        write(static_cast<u16>(value));
    }

    void write(s32 value)
    {
        write(static_cast<u32>(value));
    }

    void write(s64 value)
    {
        write(static_cast<long long>(value));
    }

    void write(float value)
    {
        write(&value, sizeof(float));
    }

    void write(double value)
    {
        write(&value, sizeof(double));
    }

    void write(const std::string& value)
    {
        assert(value.c_str()[value.size()] == 0);
        write(value.c_str(), value.size() + 1);
    }

    void write(const GUID* guid)
    {
        write(guid->Data1);
        write(guid->Data2);
        write(guid->Data3);
        write(guid->Data4, sizeof(guid->Data4));
    }
};
