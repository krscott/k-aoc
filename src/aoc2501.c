#include "aoc.h"
#include "ktl/lib/strings.h"

#include <assert.h>
#include <stdlib.h>

#define DIAL_SIZE 100

static i64 day01_count_hits(i64 pos, i64 move)
{
    i64 hits = 0;
    i64 tmp = pos + move;
    if (tmp >= DIAL_SIZE)
    {
        hits = tmp / DIAL_SIZE;
    }
    else if (tmp <= 0)
    {
        hits = -tmp / DIAL_SIZE + (pos != 0 ? 1 : 0);
    }
    return hits;
}

int main(void)
{
    strbuf line = strbuf_init();

    i64 position = 50;
    i64 hits = 0;

    while (get_line(&line, stdin))
    {
        infof("> %s\n", line.ptr);

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

        i64 const mod_position = modulo(position + move, DIAL_SIZE);

        if (PART_1)
        {
            if (mod_position == 0)
            {
                ++hits;
            }
        }
        else
        {
            hits += day01_count_hits(position, move);
        }

        position = mod_position;

        infof(" pos: %2ld hits: %ld\n", position, hits);
    }

    strbuf_deinit(&line);

    output(hits);
}
