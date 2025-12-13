#ifndef AOC_H_
#define AOC_H_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "ktl/lib/strings.h"

#if defined(__GNUC__) || defined(__clang__)
#define nodiscard __attribute__((warn_unused_result))
#else
#define nodiscard
#endif

i64 day01_count_hits(i64 pos, i64 move);
i64 day01(FILE *input, bool b);
i64 day02_sum_invalid(i64 start, i64 end, bool b);
i64 day02(FILE *input, bool b);
i64 day03(FILE *input, bool b);
i64 day04(FILE *input, bool b);
i64 day05(FILE *input, bool b);
i64 day06(FILE *input, bool b);
i64 day07(FILE *input, bool b);
i64 day08(FILE *input, bool b);
i64 day09(FILE *input, bool b);
i64 day10(FILE *input, bool b);
i64 day11(FILE *input, bool b);
i64 day12(FILE *input, bool b);
i64 day13(FILE *input, bool b);
i64 day14(FILE *input, bool b);
i64 day15(FILE *input, bool b);
i64 day16(FILE *input, bool b);
i64 day17(FILE *input, bool b);
i64 day18(FILE *input, bool b);
i64 day19(FILE *input, bool b);
i64 day20(FILE *input, bool b);
i64 day21(FILE *input, bool b);
i64 day22(FILE *input, bool b);
i64 day23(FILE *input, bool b);
i64 day24(FILE *input, bool b);
i64 day25(FILE *input, bool b);

i64 dayX(FILE *input, bool b);

nodiscard bool
append_stream_until(strbuf *const buf, FILE *const stream, char const delim);
nodiscard bool get_split(strbuf *buf, FILE *stream, char delim);
nodiscard bool get_line(strbuf *buf, FILE *stream);

nodiscard bool str2int(str s, i64 *out);

static inline i64 modulo(i64 x, i64 y)
{
    i64 const res = x % y;
    return res >= 0 ? res : res + y;
}

#define infof(...)                                                             \
    do                                                                         \
    {                                                                          \
        if (aoc_is_verbose)                                                    \
        {                                                                      \
            fprintf(stderr, __VA_ARGS__);                                      \
        }                                                                      \
    } while (0)

extern bool aoc_is_verbose;

#endif
