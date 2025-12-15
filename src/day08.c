#include "aoc.h"
#include "ktl/lib/strings.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>

#define DISCONNECTED '.'

typedef struct
{
    i64 x;
    i64 y;
    i64 z;
} vec3;
#define vec3_fmt(v) "%ld,%ld,%ld", (v).x, (v).y, (v).z

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
    usize len;
    vec3 *breaker_positions;
    i64 *breaker_circuits;
    i64 *circuit_sizes;
    chargrid *connections;
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

static i64 connect(state const s)
{
    // Find shortest length of unconnected breaker boxes
    i64 min_dist_sq = -1;
    usize brk_a = 0;
    usize brk_b = 0;
    for (usize i = 0; i < s.len - 1; ++i)
    {
        for (usize j = i + 1; j < s.len; ++j)
        {
            if (DISCONNECTED == chargrid_get(*s.connections, i, j))
            {
                i64 dist_sq = vec3_dist_sq(
                    s.breaker_positions[i],
                    s.breaker_positions[j]
                );

                if (min_dist_sq == -1 || dist_sq < min_dist_sq)
                {
                    min_dist_sq = dist_sq;
                    brk_a = i;
                    brk_b = j;
                }
            }
        }
    }
    expect(min_dist_sq > 0);

    chargrid_set(*s.connections, brk_a, brk_b, 'x');
    // chargrid_set(*s.connections, brk_b, brk_a, 'o');

    infof("Connected " vec3_fmt(s.breaker_positions[brk_a]));
    infof(" <-> " vec3_fmt(s.breaker_positions[brk_b]));

    i64 const cir_a = s.breaker_circuits[brk_a];
    i64 const cir_b = s.breaker_circuits[brk_b];

    if (cir_a != cir_b)
    {
        // Move breakers on circuit B to circuit A
        for (usize i = 0; i < s.len; ++i)
        {
            if (s.breaker_circuits[i] == cir_b)
            {
                s.breaker_circuits[i] = cir_a;
            }
        }

        // Update circuit sizes
        s.circuit_sizes[cir_a] += s.circuit_sizes[cir_b];
        s.circuit_sizes[cir_b] = 0;

        infof(" (%ld)", s.circuit_sizes[cir_a]);
    }
    else
    {
        infof(" (redundant)");
    }
    infof("\n");

    i64 out = 0;
    // HACK
    if (s.circuit_sizes[cir_a] == (i64)s.len)
    {
        out = s.breaker_positions[brk_a].x * s.breaker_positions[brk_b].x;
    }
    return out;
}

i64 day08(FILE *const input, bool const b)
{
    (void)b;

    defer(strbuf_deinit) strbuf line = strbuf_init();
    defer(vec3list_deinit) vec3list breaker_positions = vec3list_init();

    while (get_line(&line, input) && line.len > 0)
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

    defer(chargrid_deinit) chargrid connections = chargrid_init_height_width(
        breaker_positions.len,
        breaker_positions.len,
        DISCONNECTED
    );

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

    if (!b)
    {
        // Example uses fewer connections
        usize const connection_count = s.len < 1000 ? 10 : 1000;

        for (usize i = 0; i < connection_count; ++i)
        {
            expect(0 == connect(s));
            infof("ans = %ld\n", state_mult_3_largest(s));
        }
        // chargrid_print_info(connections);
        state_print_info(s);

        acc = state_mult_3_largest(s);
    }
    else
    {
        do
        {
            acc = connect(s);
        } while (acc == 0);
    }

    return acc;
}
