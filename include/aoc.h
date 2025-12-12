#ifndef AOC_H_
#define AOC_H_

#include <stdio.h>

#include "ktl/lib/strings.h"

#if defined(__GNUC__) || defined(__clang__)
#define nodiscard __attribute__((warn_unused_result))
#else
#define nodiscard
#endif

nodiscard bool get_line(strbuf *buf, FILE *stream);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline i64 modulo(i64 x, i64 y)
{
    i64 const res = x % y;
    return res >= 0 ? res : res + y;
}

i64 day01_count_hits(i64 pos, i64 move);
i64 day01(FILE *input, bool b);

#endif
