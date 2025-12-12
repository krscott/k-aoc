#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>

static bool is_repeating_pair(i64 const x)
{
    char s[30] = {0};

    int const n = snprintf(s, sizeof(s), "%ld", x);

    // printf("\n%ld - ", x);
    if (n % 2 != 0)
    {
        return false;
    }

    int const half = n / 2;
    for (int i = 0; i < half; ++i)
    {
        // printf("%c=%c ", s[i], s[i + half]);
        if (s[i] != s[i + half])
        {
            return false;
        }
    }

    return true;
}

i64 day02_sum_invalid(i64 const start, i64 const end)
{
    i64 acc = 0;

    for (i64 x = start; x <= end; ++x)
    {
        if (is_repeating_pair(x))
        {
            // printf(" %ld", x);
            acc += x;
        }
    }

    return acc;
}

i64 day02(FILE *input, bool b)
{
    (void)b;
    i64 acc = 0;

    strbuf range = strbuf_init();

    while (get_split(&range, input, ','))
    {

        str start_s, end_s;
        if (!str_split(strbuf_as_str(range), '-', &start_s, &end_s))
        {
            break;
        }

        i64 start, end;
        if (!str2int(start_s, &start) || !str2int(end_s, &end))
        {
            break;
        }

        // printf("%ld-%ld:", start, end);

        acc += day02_sum_invalid(start, end);

        // printf("\n");
    }

    strbuf_deinit(&range);
    return acc;
}
