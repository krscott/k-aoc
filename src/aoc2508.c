#include "aoc.h"
#include "ktl/lib/strings.h"
#include "macros.h"

#include <assert.h>

typedef struct
{
    i64 x;
    i64 y;
    i64 z;
} vec3;
#define vec3_fmts "%ld,%ld,%ld"
#define vec3_fmtv(v) (v).x, (v).y, (v).z

static nodiscard bool vec3_from_str(str s, vec3 *const out)
{

    str sx, sy;
    return str_split(s, ',', &sx, &s) && //
           str_split(s, ',', &sy, &s) && //
           str2int(sx, &out->x) &&       //
           str2int(sy, &out->y) &&       //
           str2int(s, &out->z);
}

static inline nodiscard i64 vec3_mag_sq(vec3 const v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline nodiscard vec3 vec3_sub(vec3 const a, vec3 const b)
{
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline nodiscard i64 vec3_dist_sq(vec3 const a, vec3 const b)
{
    return vec3_mag_sq(vec3_sub(a, b));
}

#define vec3list__type vec3
#define vec3list__infallible_allocator true
#define ktl_vec vec3list
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#undef ktl_vec

typedef struct
{
    u32 a;
    u32 b;
    i64 dist_sq;
} connection;

static int connection_cmp_shortest(void const *a, void const *b)
{
    i64 const dist_a = ((connection const *)a)->dist_sq;
    i64 const dist_b = ((connection const *)b)->dist_sq;
    if (dist_a < dist_b)
    {
        return -1;
    }
    if (dist_a > dist_b)
    {
        return 1;
    }
    return 0;
}

#define cxnlist__type connection
#define cxnlist__mut true
#define cxnlist__infallible_allocator true
#define ktl_vec cxnlist
#include "ktl/struct/vec.h"
#include "ktl/struct/vec.inc"
#undef ktl_vec

typedef struct
{
    usize len;
    vec3 *breaker_positions;
    i64 *breaker_circuits;
    i64 *circuit_sizes;
    cxnlist *connections;

} state;

static void state_print_info(state const s)
{
    for (usize i = 0; i < s.len; ++i)
    {
        vec3 const pos = s.breaker_positions[i];
        i64 const cir = s.breaker_circuits[i];
        infof("%5ld,%5ld,%5ld  cir:%5ld\n", pos.x, pos.y, pos.z, cir);
    }
    infof("\n");
    for (usize i = 0; i < s.len; ++i)
    {
        i64 const size = s.circuit_sizes[i];
        infof("cir:%5zu  size:%5ld\n", i, size);
    }
    infof("\n");
}

static i64 state_mult_3_largest(state const s)
{
    defer(intlist_deinit) intlist tmp = intlist_init();
    intlist_append(&tmp, s.circuit_sizes, s.len);
    intlist_sort(tmp);
    expect(tmp.len > 3);
    return tmp.ptr[tmp.len - 1] * tmp.ptr[tmp.len - 2] * tmp.ptr[tmp.len - 3];
}

static usize make_connections(state const s, usize n)
{
    assert(n <= s.connections->len);

    usize i = 0;
    for (; i < n; ++i)
    {
        usize const brk_a = (usize)s.connections->ptr[i].a;
        usize const brk_b = (usize)s.connections->ptr[i].b;

        infof("Connected " vec3_fmts, vec3_fmtv(s.breaker_positions[brk_a]));
        infof(" <-> " vec3_fmts, vec3_fmtv(s.breaker_positions[brk_b]));

        i64 const dist_sq = vec3_dist_sq(
            s.breaker_positions[brk_a],
            s.breaker_positions[brk_b]
        );
        infof(" dist=%ld", dist_sq);

        i64 const cir_a = s.breaker_circuits[brk_a];
        i64 const cir_b = s.breaker_circuits[brk_b];

        if (cir_a != cir_b)
        {
            // Move breakers on circuit B to circuit A
            for (usize j = 0; j < s.len; ++j)
            {
                if (s.breaker_circuits[j] == cir_b)
                {
                    s.breaker_circuits[j] = cir_a;
                }
            }

            // Update circuit sizes
            s.circuit_sizes[cir_a] += s.circuit_sizes[cir_b];
            s.circuit_sizes[cir_b] = 0;

            infof(" (%ld)\n", s.circuit_sizes[cir_a]);

            if (s.circuit_sizes[cir_a] >= (i64)s.len)
            {
                break;
            }
        }
        else
        {
            infof(" (redundant)\n");
        }
    }

    return i;
}

int main(void)
{
    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(vec3list_deinit) vec3list breaker_positions = vec3list_init();

    while (get_line(&line, stdin) && line.len > 0)
    {
        vec3 position;
        expect(vec3_from_str(strbuf_as_str(line), &position));
        vec3list_push(&breaker_positions, position);
    }

    defer(intlist_deinit) intlist breaker_circuits = intlist_init();
    defer(intlist_deinit) intlist circuit_sizes = intlist_init();
    intlist_reserve(&breaker_circuits, breaker_positions.len);
    intlist_reserve(&circuit_sizes, breaker_positions.len);
    for (usize i = 0; i < breaker_positions.len; ++i)
    {
        intlist_push(&breaker_circuits, (i64)i);
        intlist_push(&circuit_sizes, 1);
    }

    defer(cxnlist_deinit) cxnlist connections = cxnlist_init();
    cxnlist_reserve(
        &connections,
        breaker_positions.len * breaker_positions.len
    );
    for (u32 i = 0; i < breaker_positions.len - 1; ++i)
    {
        for (u32 j = i + 1; j < breaker_positions.len; ++j)
        {
            cxnlist_push(
                &connections,
                (connection){
                    .a = i,
                    .b = j,
                    .dist_sq = vec3_dist_sq(
                        breaker_positions.ptr[i],
                        breaker_positions.ptr[j]
                    ),
                }
            );
        }
    }

    cxnlist_sort_by(connections, connection_cmp_shortest);
    for (usize i = 1; i < connections.len; ++i)
    {
        if (connections.ptr[i - 1].dist_sq > connections.ptr[i].dist_sq)
        {
            panicf(
                "sort failed at %zu: %ld > %ld cmp=%d",
                i,
                connections.ptr[i - 1].dist_sq,
                connections.ptr[i].dist_sq,
                connection_cmp_shortest(
                    &connections.ptr[i - 1],
                    &connections.ptr[i]
                )
            );
        }
    }

    assert(breaker_positions.len == breaker_circuits.len);
    assert(breaker_positions.len == circuit_sizes.len);

    state s = {
        .len = breaker_positions.len,
        .breaker_positions = breaker_positions.ptr,
        .breaker_circuits = breaker_circuits.ptr,
        .circuit_sizes = circuit_sizes.ptr,
        .connections = &connections,
    };

    i64 acc;

    if (PART_1)
    {
        // Example uses fewer connections
        usize const connection_count = s.len < 1000 ? 10 : 1000;

        (void)make_connections(s, connection_count);

        acc = state_mult_3_largest(s);
    }
    else
    {
        usize const last = make_connections(s, s.connections->len);

        usize const last_a = connections.ptr[last].a;
        usize const last_b = connections.ptr[last].b;

        acc = breaker_positions.ptr[last_a].x * breaker_positions.ptr[last_b].x;
    }

    state_print_info(s);

    output(acc);
}
