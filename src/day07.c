#include "aoc.h"

#include <assert.h>
#include <stdio.h>

i64 day07(FILE *const input, bool const b)
{
    (void)b;

    defer(chargrid_deinit) chargrid chars = chargrid_init();
    chargrid_read_stream(&chars, input);
    usize const height = chargrid_height(chars);

    defer(intgrid_deinit) intgrid timelines =
        intgrid_init_height_width(height, chars.width, 0);
    chargrid_read_stream(&chars, input);

    i64 acc = 0;

    expect(height > 0);
    for (i64 row = 1; row < (i64)height; ++row)
    {
        for (i64 col = 0; col < (i64)chars.width; ++col)
        {
            char above;
            expect(chargrid_get(chars, row - 1, col, &above));
            if (above == '|' || above == 'S')
            {
                if (above == 'S')
                {
                    expect(intgrid_set(timelines, row - 1, col, 1));
                }

                i64 above_timelines;
                expect(intgrid_get(timelines, row - 1, col, &above_timelines));

                char ch;
                expect(chargrid_get(chars, row, col, &ch));
                if (ch == '^')
                {
                    expect(chargrid_set(chars, row, col - 1, '|'));
                    expect(chargrid_set(chars, row, col + 1, '|'));

                    expect(
                        intgrid_add(timelines, row, col - 1, above_timelines)
                    );
                    expect(
                        intgrid_add(timelines, row, col + 1, above_timelines)
                    );

                    if (!b)
                    {
                        ++acc;
                    }
                }
                else
                {
                    expect(chargrid_set(chars, row, col, '|'));
                    expect(intgrid_add(timelines, row, col, above_timelines));
                }
            }
        }
    }

    if (b)
    {
        for (i64 col = 0; col < (i64)chars.width; ++col)
        {
            i64 amt;
            expect(intgrid_get(timelines, (i64)height - 1, col, &amt));
            acc += amt;
        }
    }

    chargrid_print_info(chars);
    // if (b)
    // {
    //     intgrid_print_info(timelines, 2);
    // }

    return acc;
}
