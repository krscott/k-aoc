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
#include "gen/grid.h"
#include "gen/grid.inc"
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

static i64 day06a(FILE *const input)
{
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

static i64 cephalopod_math(chargrid chars, i64 op_row, i64 op_col, bool mult)
{
    i64 acc = mult ? 1 : 0;

    char ch;
    for (i64 col = op_col;; ++col)
    {
        bool any_digit = false;
        i64 num = 0;

        for (i64 row = 0; row < op_row; ++row)
        {
            if (chargrid_get(chars, row, col, &ch))
            {
                if ('0' <= ch && ch <= '9')
                {
                    num = num * 10 + (ch - '0');
                    any_digit = true;
                }
            }
        }

        if (!any_digit)
        {
            break;
        }

        infof(" %ld", num);

        if (mult)
        {
            acc *= num;
        }
        else
        {
            acc += num;
        }
    }

    infof(" -> %ld\n", acc);

    return acc;
}

static i64 day06b(FILE *const input)
{
    defer(chargrid_deinit) chargrid chars = chargrid_init();
    chargrid_read_stream(&chars, input);

    i64 acc = 0;

    i64 const op_row = (i64)chargrid_height(chars) - 1;
    char op;
    for (i64 col = 0; chargrid_get(chars, op_row, col, &op); ++col)
    {
        if (op == '*' || op == '+')
        {
            infof("%c", op);
            acc += cephalopod_math(chars, op_row, col, op == '*');
        }
    }
    infof("\n");

    return acc;
}

i64 day06(FILE *const input, bool const b)
{
    return b ? day06b(input) : day06a(input);
}
