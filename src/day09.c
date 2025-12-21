#include "aoc.h"
#include "ktl/lib/strings.h"

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

i64 day09(FILE *input, bool b);

i64 day09(FILE *const input, bool const b)
{
    (void)b;

    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(vec2list_deinit) vec2list coords = vec2list_init();

    i64 acc = 0;

    while (get_line(&line, input) && line.len > 0)
    {
        vec2 v;
        expect(vec2_from_str(strbuf_as_str(line), &v));
        vec2list_push(&coords, v);
    }

    for (usize i = 0; i < coords.len - 1; ++i)
    {
        for (usize j = i + 1; j < coords.len; ++j)
        {
            i64 area = vec2_rect_area(coords.ptr[i], coords.ptr[j]);
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

    return acc;
}
