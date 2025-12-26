#include "aoc.h"

#include "kcli.inc"
#include "ktl/prelude.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct opts
{
    bool verbose;
    long day;
    char const *part;
    char const *filename;
    bool expect_flag;
    long expect;
};

static struct opts opts_parse(int const argc, char const *const *const argv)
{
    struct opts opts = {0};

    KCLI_PARSE(
        argc,
        argv,
        {
            .pos_name = "file",
            .ptr_str = &opts.filename,
            .help = "Input File",
        },
        {
            .pos_name = "day",
            .ptr_long = &opts.day,
            .help = "Advent Day",
        },
        {
            .pos_name = "part",
            .ptr_str = &opts.part,
            .help = "A or B",
        },
        {
            .short_name = 'e',
            .long_name = "expect",
            .ptr_flag = &opts.expect_flag,
            .ptr_long = &opts.expect,
            .help = "Expected value",
        },
        {
            .short_name = 'v',
            .long_name = "verbose",
            .ptr_flag = &opts.verbose,
            .help = "Enable extra logging",
        },
    );

    return opts;
}

int main(int const argc, char const *const *const argv)
{
    struct opts opts = opts_parse(argc, argv);

    aoc_is_verbose = opts.verbose;

    FILE *input = fopen(opts.filename, "r");
    assert(input && "Could not open file"); // TODO: Error handling

    bool const part_b = (opts.part[0] & 0x5F) == 'B';

    i64 answer = 0;

    infof(
        "Day %ld %s - Input: %s\n",
        opts.day,
        part_b ? "B" : "A",
        opts.filename
    );

    switch (opts.day)
    {
        case 1:
            answer = day01(input, part_b);
            break;
        case 2:
            answer = day02(input, part_b);
            break;
        case 3:
            answer = day03(input, part_b);
            break;
        case 4:
            answer = day04(input, part_b);
            break;
        case 5:
            answer = day05(input, part_b);
            break;
        case 6:
            answer = day06(input, part_b);
            break;
        case 7:
            answer = day07(input, part_b);
            break;
        case 8:
            answer = day08(input, part_b);
            break;
        case 9:
            answer = day09(input, part_b);
            break;
        case 10:
            answer = day10(input, part_b);
            break;
        // case 11:
        //     answer = day11(input, part_b);
        //     break;
        // case 12:
        //     answer = day12(input, part_b);
        //     break;
        default:
            printf(
                "Day %ld %s not implemented\n",
                opts.day,
                part_b ? "B" : "A"
            );
            return 1;
    }

    int status = 0;

    if (opts.expect_flag)
    {
        if (opts.expect != answer)
        {
            printf("Expected: %ld  Got: %ld\n", opts.expect, answer);
            status = 1;
        }
    }
    else
    {
        printf("%ld\n", answer);
    }

    fclose(input);

    return status;
}
