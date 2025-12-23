#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>

i64 dayX(FILE *input, bool b);

i64 dayX(FILE *const input, bool const part2)
{
    (void)part2;

    defer(strbuf_deinit) strbuf line = strbuf_init();

    i64 acc = 0;

    while (get_line(&line, input) && line.len > 0)
    {
    }

    return acc;
}
