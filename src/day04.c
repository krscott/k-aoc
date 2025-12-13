#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>

typedef struct
{
    strbuf buf;
    usize width;
} grid;

static void grid_deinit(grid *const g)
{
    strbuf_deinit(&g->buf);
}

static usize grid_height(grid const g)
{
    return g.buf.len / g.width;
}

static bool
grid_at(grid const g, i64 const row, i64 const col, char **const out)
{
    bool const in_bounds =
        0 <= row && row < (i64)grid_height(g) && 0 <= col && col < (i64)g.width;

    if (in_bounds)
    {
        i64 const idx = row * (i64)g.width + col;
        *out = &g.buf.ptr[idx];
    }

    return in_bounds;
}

static bool
grid_get(grid const g, i64 const row, i64 const col, char *const out)
{
    char *ch;
    bool const in_bounds = grid_at(g, row, col, &ch);
    if (in_bounds)
    {
        *out = *ch;
    }
    return in_bounds;
}

static bool
grid_set(grid const g, i64 const row, i64 const col, char const value)
{
    char *ch;
    bool const in_bounds = grid_at(g, row, col, &ch);
    if (in_bounds)
    {
        *ch = value;
    }
    return in_bounds;
}

static grid grid_from_stream(FILE *const input)
{
    strbuf buf = strbuf_init();
    (void)get_line(&buf, input);
    usize const width = buf.len;
    while (append_stream_until(&buf, input, '\n'))
    {
    }
    return (grid){
        .buf = buf,
        .width = width,
    };
}

static usize grid_count_surrounding(
    grid const g, i64 const row, i64 const col, char const match
)
{
    usize count = 0;

    for (isize dr = row > 0 ? -1 : 0; dr <= 1; ++dr)
    {
        for (isize dc = col > 0 ? -1 : 0; dc <= 1; ++dc)
        {
            if (dr != 0 || dc != 0)
            {
                char ch;
                if (grid_get(g, row + dr, col + dc, &ch) && ch == match)
                {
                    ++count;
                }
            }
        }
    }

    return count;
}

static i64 take_rolls(grid *const g)
{
    i64 total = 0;
    i64 const height = (i64)grid_height(*g);

    for (i64 row = 0; row < height; ++row)
    {
        for (i64 col = 0; col < (i64)g->width; ++col)
        {
            char ch = '?';

            if (grid_get(*g, row, col, &ch))
            {
                if (ch == '@')
                {
                    usize count = grid_count_surrounding(*g, row, col, '@');

                    if (count < 4)
                    {
                        ++total;
                        (void)grid_set(*g, row, col, '.');

                        ch = 'x';
                    }
                }
            }

            infof("%c", ch);
        }
        infof("\n");
    }
    infof("taken: %ld\n", total);

    return total;
}

i64 day04(FILE *const input, bool const b)
{
    (void)b;

    grid g = grid_from_stream(input);

    i64 taken = 0;
    i64 acc = 0;

    do
    {
        taken = take_rolls(&g);
        acc += taken;
    } while (b && taken > 0);

    grid_deinit(&g);

    return acc;
}
