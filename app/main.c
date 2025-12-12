#include "aoc.h"

#include "kcli.inc"
#include <stdbool.h>
#include <stdio.h>

struct opts
{
    bool verbose;
    long day;
    char const *filename;
};

static struct opts opts_parse(int const argc, char const *const *const argv)
{
    struct opts opts = {0};

    KCLI_PARSE(
        argc,
        argv,
        {
            .pos_name = "day",
            .ptr_long = &opts.day,
            .help = "Advent Day",
        },
        {
            .pos_name = "file",
            .ptr_str = &opts.filename,
            .help = "Input File",
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

    if (opts.verbose)
    {
        fprintf(stderr, "Day %ld - Input: %s\n", opts.day, opts.filename);
    }

    FILE *input = fopen(opts.filename, "r");

    switch (opts.day)
    {
        case 1:
            day01(input);
            break;
        default:
            printf("Day '%ld' not implemented\n", opts.day);
            return 1;
    }

    fclose(input);

    return 0;
}
