// No header guard - repeatable include

#include "aoc.h"
#include "ktl/lib/integers.h"
#include "ktl/macros.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

// Defaults (dev-only)

#ifndef grid
#include "ktl/lib/strings.h"
#define dev_grid__list strbuf
#define grid dev_grid
#endif

// Macros

#undef grid_m
#define grid_m(x) KTL_TEMPLATE(grid, x)

#undef grid_list
#define grid_list grid_m(_list)

#undef grid_T
#define grid_T KTL_TEMPLATE(grid_list, _type)

#ifndef KTL_INC

// Type

/**
 * Generic grid type
 *
 * Fields:
 * - ptr: Pointer to array
 * - len: Length of array
 * - cap: Allocated capacity
 * - allocator (optional): Allocator handle
 */
typedef struct grid
{
    grid_list buf;
    usize width;
} grid;

// Methods

nodiscard grid grid_m(init)(void);
void grid_m(deinit)(grid *grid);
void grid_m(push)(grid *g, grid_T value);
nodiscard usize grid_m(height)(grid g);
nodiscard bool grid_m(at)(grid g, i64 row, i64 col, grid_T **out);
nodiscard bool grid_m(get)(grid g, i64 row, i64 col, grid_T *out);
nodiscard bool grid_m(set)(grid g, i64 row, i64 col, grid_T value);

#endif // KTL_INC
