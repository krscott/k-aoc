#include "aoc.h"

#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"
#include <stdio.h>

nodiscard bool get_line(strbuf *const buf, FILE *const stream)
{
    strbuf_clear(buf);

    int c = fgetc(stream);
    bool const ok = c != EOF;

    for (; c != EOF && c != '\n'; c = fgetc(stream))
    {
        strbuf_push(buf, (char)c);
    }

    return ok;
}
