#include "aoc.h"

#include <assert.h>
#include <stdio.h>

i64 day07(FILE *const input, bool const b)
{
    defer(chargrid_deinit) chargrid chars = chargrid_init();
    chargrid_read_stream(&chars, input);
    usize const height = chargrid_height(&chars);

    defer(intgrid_deinit) intgrid timelines =
        intgrid_init_height_width(height, chars.width, 0);
    chargrid_read_stream(&chars, input);

    i64 acc = 0;

    expect(height > 0);
    for (usize row = 1; row < height; ++row)
    {
        for (usize col = 0; col < chars.width; ++col)
        {
            char above = chargrid_get(&chars, row - 1, col);
            if (above == '|' || above == 'S')
            {
                if (above == 'S')
                {
                    intgrid_set(&timelines, row - 1, col, 1);
                }

                i64 above_timelines = intgrid_get(&timelines, row - 1, col);

                char ch = chargrid_get(&chars, row, col);
                if (ch == '^')
                {
                    chargrid_set(&chars, row, col - 1, '|');
                    chargrid_set(&chars, row, col + 1, '|');

                    intgrid_add(&timelines, row, col - 1, above_timelines);
                    intgrid_add(&timelines, row, col + 1, above_timelines);

                    if (!b)
                    {
                        ++acc;
                    }
                }
                else
                {
                    chargrid_set(&chars, row, col, '|');
                    intgrid_add(&timelines, row, col, above_timelines);
                }
            }
        }
    }

    if (b)
    {
        for (usize col = 0; col < chars.width; ++col)
        {
            acc += intgrid_get(&timelines, height - 1, col);
        }
    }

    chargrid_print_info(&chars);
    // if (b)
    // {
    //     intgrid_print_info(timelines, 2);
    // }

    return acc;
}
