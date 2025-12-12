#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define DIAL_SIZE 100

i64 day01(FILE *input)
{
    strbuf line = strbuf_init();
    strbuf_reserve(&line, 256);

    i64 position = 50;
    i64 hits = 0;

    while (get_line(&line, input))
    {
        // fprintf(stderr, "> %s\n", line.ptr);

        int move = atoi(&line.ptr[1]);

        switch (line.ptr[0])
        {
            case 'L':
                move = -move;
                break;
            case 'R':
                break;
            default:
                assert(false);
                break;
        }

        position = (position + DIAL_SIZE + move) % DIAL_SIZE;
        // fprintf(stderr, "acc: %d\n", hits);

        if (position == 0)
        {
            ++hits;
        }
    }

    strbuf_deinit(&line);

    return hits;
}
