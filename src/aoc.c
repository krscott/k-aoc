#include "aoc.h"

#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

nodiscard bool
get_split(strbuf *const buf, FILE *const stream, char const delim)
{
    assert(stream);

    strbuf_clear(buf);
    strbuf_reserve(buf, 256);
    buf->ptr[0] = '\0';

    int c = fgetc(stream);
    bool const ok = c != EOF;

    for (; c != EOF && c != delim; c = fgetc(stream))
    {
        strbuf_push(buf, (char)c);
    }

    return ok;
}

nodiscard bool get_line(strbuf *const buf, FILE *const stream)
{
    return get_split(buf, stream, '\n');
}

nodiscard bool str2int(str s, i64 *out)
{
    assert(s.len > 0);
    errno = 0;
    i64 value = atoll(s.ptr);
    bool ok = (errno == 0);
    if (ok)
    {
        *out = value;
    }
    return ok;
}
