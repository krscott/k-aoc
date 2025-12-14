#include "aoc.h"

#include <assert.h>
#include <stdio.h>

static usize chargrid_count_surrounding(
    chargrid const g, i64 const row, i64 const col, char const match
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
                if (chargrid_get(g, row + dr, col + dc, &ch) && ch == match)
                {
                    ++count;
                }
            }
        }
    }

    return count;
}

static i64 take_rolls(chargrid *const g)
{
    i64 total = 0;
    i64 const height = (i64)chargrid_height(*g);

    // Mark
    for (i64 row = 0; row < height; ++row)
    {
        for (i64 col = 0; col < (i64)g->width; ++col)
        {
            char ch = '?';

            if (chargrid_get(*g, row, col, &ch))
            {
                if (ch == '@')
                {
                    usize count =
                        chargrid_count_surrounding(*g, row, col, '@') +
                        chargrid_count_surrounding(*g, row, col, 'x');

                    if (count < 4)
                    {
                        ++total;
                        (void)chargrid_set(*g, row, col, 'x');
                    }
                }
            }
        }
    }

    // Sweep
    for (i64 row = 0; row < height; ++row)
    {
        for (i64 col = 0; col < (i64)g->width; ++col)
        {
            char ch = '?';
            if (chargrid_get(*g, row, col, &ch) && ch == 'x')
            {
                (void)chargrid_set(*g, row, col, '.');
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
    defer(chargrid_deinit) chargrid g = chargrid_init();
    chargrid_read_stream(&g, input);

    i64 taken = 0;
    i64 acc = 0;

    do
    {
        taken = take_rolls(&g);
        acc += taken;
    } while (b && taken > 0);

    return acc;
}
