#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>

static bool is_repeating_n(char const *const s, int const n, int const div)
{
    if (n % div != 0)
    {
        return false;
    }

    int const section_size = n / div;

    for (int i = 0; i < section_size; ++i)
    {
        char const c = s[i];
        for (int j = 1; j < div; ++j)
        {
            int const offset = i + j * section_size;
            if (offset >= n)
            {
                break;
            }
            if (c != s[offset])
            {
                return false;
            }
        }
    }

    return true;
}

static bool is_repeating(i64 const x)
{
    char s[30] = {0};

    int const n = snprintf(s, sizeof(s), "%ld", x);

    bool out = false;

    if (PART_1)
    {
        out = is_repeating_n(s, n, 2);
    }
    else
    {
        for (int i = 2; i <= n; ++i)
        {
            if (is_repeating_n(s, n, i))
            {
                infof(" %ld(%d)", x, i);
                out = true;
                break;
            }
        }
    }

    return out;
}

static i64 day02_sum_invalid(i64 const start, i64 const end)
{
    i64 acc = 0;

    for (i64 x = start; x <= end; ++x)
    {
        if (is_repeating(x))
        {
            acc += x;
        }
    }

    return acc;
}

int main(void)
{
    i64 acc = 0;

    strbuf range = strbuf_init();

    while (get_split(&range, stdin, ','))
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

        infof("%ld-%ld:", start, end);

        acc += day02_sum_invalid(start, end);

        infof("\n");
    }

    strbuf_deinit(&range);

    output(acc);
}
