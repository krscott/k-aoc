// No header guard - repeatable include

#include "ktl/lib/integers.h"
#include "ktl/macros.h"
#include "macros.h"

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

KTL_DIAG_PUSH
KTL_DIAG_IGNORE("-Wundef")

#undef grid_m
#define grid_m(x) KTL_TEMPLATE(grid, x)

#undef grid_list
#define grid_list grid_m(_list)

#undef grid_T
#define grid_T KTL_TEMPLATE(grid_list, _type)

#undef grid_T_eq
#if KTL_TEMPLATE(grid_T, _eq)
#define grid_T_eq(a, b) KTL_TEMPLATE(grid_T, eq)((a), (b))
#elif KTL_TEMPLATE(grid_T, _ord)
#define grid_T_eq(a, b) (0 == KTL_TEMPLATE(grid_T, cmp)((a), (b)))
#endif

KTL_DIAG_POP

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
nodiscard
    grid grid_m(init_height_width)(usize height, usize width, grid_T fill);
void grid_m(deinit)(grid *grid);
void grid_m(push)(grid *g, grid_T value);
nodiscard usize grid_m(height)(grid const *g);
nodiscard grid_T *grid_m(at)(grid const *g, usize row, usize col);
nodiscard bool
    grid_m(at_checked)(grid const *g, usize row, usize col, grid_T **out);
nodiscard bool
    grid_m(get_checked)(grid const *g, usize row, usize col, grid_T *out);
nodiscard grid_T grid_m(get)(grid const *g, usize row, usize col);
nodiscard bool
    grid_m(set_checked)(grid const *g, usize row, usize col, grid_T value);
void grid_m(set)(grid const *g, usize row, usize col, grid_T value);

#ifdef grid_T_eq
nodiscard bool
    grid_m(find)(grid const *g, grid_T value, usize *out_row, usize *out_col);
#endif

#endif // KTL_INC
