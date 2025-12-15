#include "ktl/lib/integers.h"
#include "ktl/macros.h"
#include "macros.h"
#include <assert.h>

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

nodiscard grid grid_m(init_height_width)(
    usize const height, usize const width, grid_T const fill
)
{
    usize const len = height * width;

    grid g = grid_m(init)();
    KTL_TEMPLATE(grid_list, reserve)(&g.buf, len);

    for (usize i = 0; i < len; ++i)
    {
        KTL_TEMPLATE(grid_list, push)(&g.buf, fill);
    }

    g.width = width;
    return g;
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
    assert(g.width > 0);
    return g.buf.len / g.width;
}

nodiscard bool
grid_m(at)(grid const g, usize const row, usize const col, grid_T **const out)
{
    bool const in_bounds = row < grid_m(height)(g) && col < g.width;

    if (in_bounds)
    {
        usize const idx = row * g.width + col;
        *out = &g.buf.ptr[idx];
    }

    return in_bounds;
}

nodiscard bool grid_m(get_checked)(
    grid const g, usize const row, usize const col, grid_T *const out
)
{
    grid_T *ch;
    bool const in_bounds = grid_m(at)(g, row, col, &ch);
    if (in_bounds)
    {
        *out = *ch;
    }
    return in_bounds;
}

nodiscard grid_T grid_m(get)(grid const g, usize const row, usize const col)
{
    grid_T out;
    expect(grid_m(get_checked)(g, row, col, &out));
    return out;
}

nodiscard bool grid_m(set_checked)(
    grid const g, usize const row, usize const col, grid_T const value
)
{
    grid_T *ch;
    bool const in_bounds = grid_m(at)(g, row, col, &ch);
    if (in_bounds)
    {
        *ch = value;
    }
    return in_bounds;
}

void grid_m(set)(
    grid const g, usize const row, usize const col, grid_T const value
)
{
    expect(grid_m(set_checked)(g, row, col, value));
}

#ifdef grid_T_eq
nodiscard bool grid_m(find)(
    grid const g, grid_T const value, usize *const out_row, usize *const out_col
)
{
    assert(g.width > 0);

    usize idx;
    bool const found = KTL_TEMPLATE(grid_list, find_index)(g.buf, value, &idx);
    if (found)
    {
        if (out_row)
        {
            *out_row = idx / g.width;
        }
        if (out_col)
        {
            *out_col = idx % g.width;
        }
    }
    return found;
}
#endif
