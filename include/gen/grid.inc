#include "aoc.h"
#include "ktl/macros.h"

#ifndef grid // dev-only
#include "gen/grid.h"
#endif

#define KTL_INC
#include "gen/grid.h"
#undef KTL_INC

nodiscard grid grid_m(init)(void)
{
    return (grid){
        .buf = KTL_TEMPLATE(grid_list, init)(),
    };
}

void grid_m(deinit)(grid *grid)
{
    KTL_TEMPLATE(grid_list, deinit)(&grid->buf);
}

void grid_m(push)(grid *const g, grid_T const value)
{
    KTL_TEMPLATE(grid_list, push)(&g->buf, value);
}

nodiscard usize grid_m(height)(grid const g)
{
    return g.buf.len / g.width;
}

nodiscard bool
grid_m(at)(grid const g, i64 const row, i64 const col, grid_T **const out)
{
    bool const in_bounds = 0 <= row && row < (i64)grid_m(height)(g) &&
                           0 <= col && col < (i64)g.width;

    if (in_bounds)
    {
        i64 const idx = row * (i64)g.width + col;
        *out = &g.buf.ptr[idx];
    }

    return in_bounds;
}

nodiscard bool
grid_m(get)(grid const g, i64 const row, i64 const col, grid_T *const out)
{
    grid_T *ch;
    bool const in_bounds = grid_m(at)(g, row, col, &ch);
    if (in_bounds)
    {
        *out = *ch;
    }
    return in_bounds;
}

nodiscard bool
grid_m(set)(grid const g, i64 const row, i64 const col, grid_T const value)
{
    grid_T *ch;
    bool const in_bounds = grid_m(at)(g, row, col, &ch);
    if (in_bounds)
    {
        *ch = value;
    }
    return in_bounds;
}
