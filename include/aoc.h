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

i64 day01(FILE *input);

#endif
