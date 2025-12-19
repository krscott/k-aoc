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

static inline i64 range_count(range r)
{
    return r.end - r.start + 1;
}

static inline bool range_overlap(range a, range b)
{
    return a.start <= b.end && b.start <= a.end;
}

static inline range range_bound(range a, range b)
{
    return (range){
        .start = i64_min(a.start, b.start),
        .end = i64_max(a.end, b.end),
    };
}

#define range_fmt(r) "%ld-%ld", (r).start, (r).end

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

static i64 day05a(FILE *const input)
{
    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(ranges_deinit) ranges fresh_ranges = ranges_init();

    while (get_line(&line, input) && line.len > 0)
    {
        range fresh_range = range_from_str(strbuf_as_str(line));

        infof("Range ");
        infof(range_fmt(fresh_range));
        infof("\n");

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
                infof("Ingredient %ld is in range ", ingredient);
                infof(range_fmt(r));
                infof("\n");
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

static i64 day05b(FILE *const input)
{
    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(ranges_deinit) ranges distinct_ranges = ranges_init();

    while (get_line(&line, input) && line.len > 0)
    {
        range fresh_range = range_from_str(strbuf_as_str(line));

        infof("Range ");
        infof(range_fmt(fresh_range));

        usize combined_index = 0;
        bool is_distinct = true;
        for (usize i = 0; i < distinct_ranges.len; ++i)
        {
            range const d = distinct_ranges.ptr[i];
            if (range_overlap(d, fresh_range))
            {
                if (is_distinct)
                {
                    is_distinct = false;
                    combined_index = i;
                    infof(" overlaps ");
                    infof(range_fmt(d));
                    distinct_ranges.ptr[combined_index] = range_bound(
                        distinct_ranges.ptr[combined_index],
                        fresh_range
                    );
                }
                else
                {
                    infof(" and ");
                    infof(range_fmt(d));
                    distinct_ranges.ptr[combined_index] = range_bound(
                        distinct_ranges.ptr[combined_index],
                        distinct_ranges.ptr[i]
                    );
                    ranges_swap_remove(&distinct_ranges, i);
                    break;
                }
            }
        }
        if (is_distinct)
        {
            infof(" is distinct ");
            ranges_push(&distinct_ranges, fresh_range);
        }

        infof("\n");
    }

    i64 acc = 0;

    infof("Non-overlapping ranges:\n");
    for (usize i = 0; i < distinct_ranges.len; ++i)
    {
        range const r = distinct_ranges.ptr[i];
        infof("  ");
        infof(range_fmt(r));
        infof("\n");
        acc += range_count(r);
    }

    return acc;
}

int main(void)
{
    output(PART_2 ? day05b(stdin) : day05a(stdin));
}
