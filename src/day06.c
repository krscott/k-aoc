#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>

#define numberlist__type i64
#define numberlist__infallible_allocator true
#define ktl_vec numberlist
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#undef ktl_vec

#define intgrid__list numberlist
#define grid intgrid
#include "grid.inc"
#undef grid

static i64 calculate_column(intgrid g, usize col, char op)
{
    i64 acc = op == '*' ? 1 : 0;
    for (usize row = 0; row < intgrid_height(g); ++row)
    {
        i64 value;
        if (intgrid_get(g, (i64)row, (i64)col, &value))
        {
            if (op == '*')
            {
                acc *= value;
            }
            else
            {
                acc += value;
            }
        }
    }
    return acc;
}

i64 day06(FILE *const input, bool const b)
{
    (void)b;

    defer(intgrid_deinit) intgrid numbers = intgrid_init();
    defer(strbuf_deinit) strbuf line = strbuf_init();

    i64 acc = 0;

    while (get_line(&line, input) && line.len > 0)
    {
        str tail = str_trim_end(strbuf_as_str(line));
        str head;
        for (usize col = 0; str_split(str_trim_start(tail), ' ', &head, &tail);
             ++col)
        {
            i64 num;
            if (str2int(head, &num))
            {
                // number
                infof("%4ld ", num);
                intgrid_push(&numbers, num);
            }
            else
            {
                // op
                infof("%4c ", head.ptr[0]);
                acc += calculate_column(numbers, col, head.ptr[0]);
            }
        }

        if (numbers.width == 0)
        {
            numbers.width = numbers.buf.len;
        }
        infof("\n");
    }

    return acc;
}
