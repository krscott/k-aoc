#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static usize max_char_idx(str s)
{
    char max = 0;
    usize idx = 0;
    for (usize i = 0; i < s.len; ++i)
    {
        if (s.ptr[i] > max)
        {
            max = s.ptr[i];
            idx = i;
        }
    }
    return idx;
}

i64 day03(FILE *const input, bool const b)
{
    (void)b;

    strbuf line = strbuf_init();

    i64 acc = 0;

    while (get_line(&line, input) && line.len > 1)
    {
        str tens = strbuf_as_str(line);
        --(tens.len);
        usize const tens_idx = max_char_idx(tens);

        str ones = {
            .ptr = &line.ptr[tens_idx + 1],
            .len = line.len - tens_idx - 1
        };
        usize const ones_idx = max_char_idx(ones);

        i64 const joltage = (i64)(tens.ptr[tens_idx] - '0') * 10 +
                            (i64)(ones.ptr[ones_idx] - '0');

        printf("%s - %ld\n", line.ptr, joltage);

        acc += joltage;
    }

    strbuf_deinit(&line);

    return acc;
}
