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

nodiscard bool get_split(strbuf *buf, FILE *stream, char delim);
nodiscard bool get_line(strbuf *buf, FILE *stream);

nodiscard bool str2int(str s, i64 *out);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline i64 modulo(i64 x, i64 y)
{
    i64 const res = x % y;
    return res >= 0 ? res : res + y;
}

i64 day01_count_hits(i64 pos, i64 move);
i64 day01(FILE *input, bool b);
i64 day02_sum_invalid(i64 start, i64 end, bool b);
i64 day02(FILE *input, bool b);

i64 dayX(FILE *input, bool b);

#endif
