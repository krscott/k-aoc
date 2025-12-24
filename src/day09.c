#include "aoc.h"
#include "ktl/lib/strings.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>

typedef struct
{
    i64 x;
    i64 y;
} vec2;
#define vec2_fmts "%ld,%ld"
#define vec2_fmtv(v) ((v).x), ((v).y)

static nodiscard bool vec2_from_str(str s, vec2 *const out)
{
    str sx;
    return str_split(s, ',', &sx, &s) && //
           str2int(sx, &out->x) &&       //
           str2int(s, &out->y);
}

static nodiscard i64 vec2_rect_area(vec2 const a, vec2 const b)
{
    i64 w = absi(a.x - b.x) + 1;
    i64 h = absi(a.y - b.y) + 1;
    return w * h;
}

#define vec2list__type vec2
#define vec2list__infallible_allocator true
#define ktl_vec vec2list
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#undef ktl_vec

typedef struct
{
    vec2 tl;
    vec2 br;
} aabb;
#define aabb_fmts vec2_fmts "~" vec2_fmts
#define aabb_fmtv(x) vec2_fmtv((x).tl), vec2_fmtv((x).br)

static inline nodiscard aabb aabb_from_corners(vec2 const a, vec2 const b)
{
    return (aabb){
        .tl = (vec2){mini(a.x, b.x), mini(a.y, b.y)},
        .br = (vec2){maxi(a.x, b.x), maxi(a.y, b.y)},
    };
}

static inline nodiscard i64 aabb_rect_area(aabb const b)
{
    return vec2_rect_area(b.tl, b.br);
}

#define aabblist__type aabb
#define aabblist__infallible_allocator true
#define ktl_vec aabblist
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#undef ktl_vec

static int aabb_cmp_area_rev(void const *a, void const *b)
{
    i64 const area_a = aabb_rect_area(*(aabb const *)a);
    i64 const area_b = aabb_rect_area(*(aabb const *)b);
    return -i64_cmp(&area_a, &area_b);
}

#define intmap__key i64
#define intmap__value usize
#define intmap__infallible_allocator true
#define ktl_hashmap intmap
#include "ktl/struct/hashmap.h"
#include "ktl/struct/hashmap.inc"
#undef ktl_hashmap

static void set_sorted_indices(intmap *m)
{
    defer(intlist_deinit) intlist indices = intlist_init();
    intlist_reserve(&indices, m->count);

    {
        i64 x;
        intmap_iter it = intmap_each(m);
        while (intmap_next(&it, &x, NULL))
        {
            intlist_push(&indices, x);
        }
    }

    intlist_sort(indices);

    for (usize i = 0; i < indices.len; ++i)
    {
        intmap_insert(m, indices.ptr[i], i);
        // infof("%4zu: %7ld\n", i, indices.ptr[i]);
    }
    // infof("\n");
}

i64 day09(FILE *input, bool b);

i64 day09(FILE *const input, bool const part2)
{
    (void)part2;

    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(vec2list_deinit) vec2list coords = vec2list_init();

    i64 acc = 0;

    while (get_line(&line, input) && line.len > 0)
    {
        vec2 v;
        expect(vec2_from_str(strbuf_as_str(line), &v));
        vec2list_push(&coords, v);
    }

    if (!part2)
    {
        for (usize i = 0; i < coords.len - 1; ++i)
        {
            for (usize j = i + 1; j < coords.len; ++j)
            {
                i64 const area = vec2_rect_area(coords.ptr[i], coords.ptr[j]);
                infof(
                    vec2_fmts " - " vec2_fmts " -> %ld\n",
                    vec2_fmtv(coords.ptr[i]),
                    vec2_fmtv(coords.ptr[j]),
                    area
                );
                if (area > acc)
                {
                    acc = area;
                }
            }
        }
    }
    else
    {
        defer(intmap_deinit) intmap x_coords = intmap_init();
        defer(intmap_deinit) intmap y_coords = intmap_init();

        for (usize i = 0; i < coords.len; ++i)
        {
            vec2 const v = coords.ptr[i];
            intmap_insert(&x_coords, v.x, 0);
            intmap_insert(&x_coords, v.x + 1, 0);
            intmap_insert(&y_coords, v.y, 0);
            intmap_insert(&y_coords, v.y + 1, 0);
        }

        set_sorted_indices(&x_coords);
        set_sorted_indices(&y_coords);

        defer(chargrid_deinit) chargrid regions =
            chargrid_init_height_width(y_coords.count, x_coords.count, '.');

        for (usize i = 0; i < coords.len; ++i)
        {
            vec2 const v0 = coords.ptr[i];
            usize r0, c0;
            expect(intmap_get(&x_coords, v0.x, &c0));
            expect(intmap_get(&y_coords, v0.y, &r0));

            vec2 const v1 = coords.ptr[(i + 1) % coords.len];
            usize r1, c1;
            expect(intmap_get(&x_coords, v1.x, &c1));
            expect(intmap_get(&y_coords, v1.y, &r1));

            for (usize r = min(r0, r1); r <= max(r0, r1); ++r)
            {
                for (usize c = min(c0, c1); c <= max(c0, c1); ++c)
                {
                    chargrid_set(&regions, r, c, '#');
                }
            }
        }

        {
            defer(vec2list_deinit) vec2list fillstack = vec2list_init();

            // Find a cell on the interior.
            // Top row is guaranteed to be top edge.
            for (usize c = 0; c < regions.width; ++c)
            {
                if (chargrid_get(&regions, 0, c) == '#')
                {
                    if (chargrid_get(&regions, 1, c) == '.')
                    {
                        vec2list_push(&fillstack, (vec2){(i64)c, 1});
                    }
                }
            }

            // Fill
            vec2 v;
            while (vec2list_pop(&fillstack, &v))
            {
                usize const row = (usize)v.y;
                usize const col = (usize)v.x;
                if (chargrid_get(&regions, row, col) == '.')
                {
                    chargrid_set(&regions, row, col, '#');
                    vec2list_push(&fillstack, (vec2){v.x, v.y - 1});
                    vec2list_push(&fillstack, (vec2){v.x, v.y + 1});
                    vec2list_push(&fillstack, (vec2){v.x - 1, v.y});
                    vec2list_push(&fillstack, (vec2){v.x + 1, v.y});
                }
            }
        }

        chargrid_print_info(&regions);

        defer(aabblist_deinit) aabblist rects = aabblist_init();
        aabblist_reserve(&rects, coords.len * coords.len);

        for (usize i = 0; i < coords.len - 1; ++i)
        {
            for (usize j = i + 1; j < coords.len; ++j)
            {
                aabblist_push(
                    &rects,
                    aabb_from_corners(coords.ptr[i], coords.ptr[j])
                );
            }
        }

        aabblist_sort_by(rects, aabb_cmp_area_rev);

        for (usize i = 0; i < rects.len; ++i)
        {
            vec2 const v0 = rects.ptr[i].tl;
            vec2 const v1 = rects.ptr[i].br;

            usize r0, c0, r1, c1;
            expect(intmap_get(&x_coords, v0.x, &c0));
            expect(intmap_get(&x_coords, v1.x, &c1));
            expect(intmap_get(&y_coords, v0.y, &r0));
            expect(intmap_get(&y_coords, v1.y, &r1));

            bool ok = true;

            for (usize c = c0; c <= c1; ++c)
            {
                if ((chargrid_get(&regions, r0, c) != '#') ||
                    (chargrid_get(&regions, r1, c) != '#'))
                {
                    ok = false;
                    goto endloop;
                }
            }

            for (usize r = r0; r <= r1; ++r)
            {
                if ((chargrid_get(&regions, r, c0) != '#') ||
                    (chargrid_get(&regions, r, c1) != '#'))
                {
                    ok = false;
                    goto endloop;
                }
            }

        endloop:
            if (ok)
            {
                acc = vec2_rect_area(v0, v1);
                break;
            }
        }
    }

    return acc;
}
