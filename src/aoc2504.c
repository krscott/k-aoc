#include "aoc.h"

#include <assert.h>

static usize chargrid_count_surrounding(
    chargrid const *const g, usize const row, usize const col, char const match
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
                if (chargrid_get_checked(
                        g,
                        (usize)((isize)row + dr),
                        (usize)((isize)col + dc),
                        &ch
                    ) &&
                    ch == match)
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
    usize const height = chargrid_height(g);

    // Mark
    for (usize row = 0; row < height; ++row)
    {
        for (usize col = 0; col < g->width; ++col)
        {
            char ch = '?';

            if (chargrid_get_checked(g, row, col, &ch))
            {
                if (ch == '@')
                {
                    usize count = chargrid_count_surrounding(g, row, col, '@') +
                                  chargrid_count_surrounding(g, row, col, 'x');

                    if (count < 4)
                    {
                        ++total;
                        chargrid_set(g, row, col, 'x');
                    }
                }
            }
        }
    }

    // Sweep
    for (usize row = 0; row < height; ++row)
    {
        for (usize col = 0; col < g->width; ++col)
        {
            char ch = '?';
            if (chargrid_get_checked(g, row, col, &ch) && ch == 'x')
            {
                chargrid_set(g, row, col, '.');
            }
            infof("%c", ch);
        }
        infof("\n");
    }

    infof("taken: %ld\n", total);
    return total;
}

int main(void)
{
    defer(chargrid_deinit) chargrid g = chargrid_init();
    chargrid_read_stream(&g, stdin);

    i64 taken = 0;
    i64 acc = 0;

    do
    {
        taken = take_rolls(&g);
        acc += taken;
    } while (PART_2 && taken > 0);

    output(acc);
}
