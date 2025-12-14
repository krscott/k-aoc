#include "aoc.h"

#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"
#include "string.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

bool aoc_is_verbose = false;

nodiscard bool
append_stream_until(strbuf *const buf, FILE *const stream, char const delim)
{
    int c = fgetc(stream);
    bool const ok = c != EOF;

    for (; c != EOF && c != delim; c = fgetc(stream))
    {
        strbuf_push(buf, (char)c);
    }

    return ok;
}

nodiscard bool
get_split(strbuf *const buf, FILE *const stream, char const delim)
{
    assert(stream);

    strbuf_clear(buf);
    strbuf_reserve(buf, 256);
    buf->ptr[0] = '\0';

    return append_stream_until(buf, stream, delim);
}

nodiscard bool get_line(strbuf *const buf, FILE *const stream)
{
    return get_split(buf, stream, '\n');
}

nodiscard bool str2int(str s, i64 *out)
{
    s = str_trim(s);

    bool const neg = (s.ptr[0] == '-');
    if (neg)
    {
        str_split_at(s, 1, NULL, &s);
    }

    bool ok = false;
    i64 acc = 0;

    for (usize i = 0; i < s.len; ++i)
    {
        char const c = s.ptr[i];
        ok = '0' <= c && c <= '9';
        if (ok)
        {
            if (neg)
            {
                acc = acc * 10 - (i64)(c - '0');
            }
            else
            {
                acc = acc * 10 + (i64)(c - '0');
            }
        }
        else
        {
            break;
        }
    }

    if (ok)
    {
        *out = acc;
    }
    return ok;
}
