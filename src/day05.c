#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>

typedef struct
{
    i64 start;
    i64 end;
} range;

static inline bool range_in(range r, i64 value)
{
    return (r.start <= value && value <= r.end);
}

static range range_from_str(str s)
{
    str start_s, end_s;
    if (!str_split(s, '-', &start_s, &end_s))
    {
        panicf("Expected range format: 'X-Y' Got: %.*s", (int)s.len, s.ptr);
    }

    i64 start;
    if (!str2int(start_s, &start))
    {
        panicf("Expected int. Got: %.*s", (int)start_s.len, start_s.ptr);
    }

    i64 end;
    if (!str2int(end_s, &end))
    {
        panicf("Expected int. Got: %.*s", (int)end_s.len, end_s.ptr);
    }

    return (range){
        .start = start,
        .end = end,
    };
}

#define ranges__type range
#define ranges__infallible_allocator true
#define ktl_vec ranges
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#undef ktl_vec

i64 day05(FILE *const input, bool const b)
{
    (void)b;

    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(ranges_deinit) ranges fresh_ranges = ranges_init();

    while (get_line(&line, input) && line.len > 0)
    {
        range fresh_range = range_from_str(strbuf_as_str(line));

        infof("%ld-%ld\n", fresh_range.start, fresh_range.end);
        ranges_push(&fresh_ranges, fresh_range);
    }

    i64 acc = 0;

    while (get_line(&line, input) && line.len > 0)
    {
        i64 ingredient;
        if (!str2int(strbuf_as_str(line), &ingredient))
        {
            panicf("Could not parse ID: %s", line.ptr);
        }

        bool is_fresh = false;
        for (usize i = 0; i < fresh_ranges.len; ++i)
        {
            range const r = fresh_ranges.ptr[i];
            if (range_in(r, ingredient))
            {
                infof(
                    "Ingredient %ld is in range (%ld-%ld)\n",
                    ingredient,
                    r.start,
                    r.end
                );
                is_fresh = true;
                break;
            }
        }

        if (is_fresh)
        {
            ++acc;
        }
        else
        {
            infof("Ingredient %ld is spoiled\n", ingredient);
        }
    }

    return acc;
}
