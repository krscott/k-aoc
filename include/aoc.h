#ifndef AOC_H_
#define AOC_H_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // IWYU pragma: export

#include "ktl/lib/strings.h"
#include "ktl/prelude.h" // IWYU pragma: export
#include "macros.h"      // IWYU pragma: export

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

#define chargrid__list strbuf
#define grid chargrid
#include "gen/grid.h"
#undef grid
void chargrid_read_stream(chargrid *g, FILE *input);
void chargrid_print_info(chargrid g);

#define intlist__type i64
#define intlist__infallible_allocator true
#define ktl_vec intlist
#include "ktl/struct/vec.h"
#undef ktl_vec

#define intgrid__list intlist
#define grid intgrid
#include "gen/grid.h"
#undef grid
void intgrid_print_info(intgrid g, int cell_padding);
void intgrid_add(intgrid g, usize row, usize col, i64 addition);

// Globals

extern bool aoc_is_verbose;

#endif
