#include "aoc.h"
#include "ktl/lib/strings.h"
#include "ktl/macros.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 10

static i64 lightsnode_heuristic(u16 const node, u16 const goal)
{
    u16 const diff = node ^ goal;
    i64 dist = 0;
    for (usize i = 0; i < MAX_LEN; ++i)
    {
        dist += (diff >> i) & 1;
    }
    return dist;
}

#define lightsnode__node u16
#define lightsnode__h lightsnode_heuristic
#define lightsnode__infallible_allocator true
#define aoc_pathfind lightsnode
#include "gen/aoc_pathfind.h"
#include "gen/aoc_pathfind.inc"
#undef aoc_pathfind

typedef struct
{
    u8 num_lights;
    u16 lights_goal;
    u16 lights_state;
    u16 switch_options[MAX_LEN + 1];
    u8 joltage[MAX_LEN + 1];
} machine;

static void machine_print_info(machine const *const m)
{
    infof("[");
    for (usize i = 0; i < m->num_lights; ++i)
    {
        if ((m->lights_goal >> i) & 1)
        {
            if ((m->lights_state >> i) & 1)
            {
                infof("#");
            }
            else
            {
                infof("x");
            }
        }
        else
        {
            if ((m->lights_state >> i) & 1)
            {
                infof("X");
            }
            else
            {
                infof(".");
            }
        }
    }
    infof("] ");

    for (usize i = 0; m->switch_options[i]; ++i)
    {
        infof("(");
        u16 x = m->switch_options[i];
        for (usize j = 0; x; ++j)
        {
            if (x & 1)
            {
                infof("%zu", j);

                if ((x >> 1) != 0)
                {
                    infof(",");
                }
            }

            x >>= 1;
        }
        infof(") ");
    }

    infof("{");
    for (usize j = 0; j < MAX_LEN && m->joltage[j] != 0xff; ++j)
    {
        infof("%u", m->joltage[j]);
        if (j < MAX_LEN - 1 && m->joltage[j + 1] != 0xff)
        {
            infof(",");
        }
    }
    infof("} ");

    infof("\n");
}

static nodiscard bool
parse_token(strview sv, char const *const token, strview *const tail)
{
    sv = strview_trim_start(sv);
    strview const token_sv = strview_from_terminated(token);
    bool const ok = strview_starts_with(sv, token_sv);
    if (ok)
    {
        strview_split_at(sv, token_sv.len, NULL, tail);
        // infof("Token: %s\n", token);
    }
    return ok;
}

static nodiscard bool
parse_lights(strview sv, machine *const m, strview *const tail)
{
    m->num_lights = 0;
    m->lights_goal = 0;

    bool ok = parse_token(sv, "[", &sv);

    while (ok && !parse_token(sv, "]", &sv))
    {
        if (parse_token(sv, "#", &sv))
        {
            m->lights_goal |= 1 << m->num_lights;
        }
        else if (parse_token(sv, ".", &sv))
        {
            // do nothing
        }
        else
        {
            ok = false;
            // infof(
            //     "parse_lights: unexpected token " strview_fmts,
            //     strview_fmtv(sv)
            // );
        }
        ++(m->num_lights);
    }

    if (ok && tail)
    {
        *tail = sv;
    }

    return ok;
}

static nodiscard bool parse_switches_single(
    strview sv, machine *const m, usize i, strview *const tail
)
{
    bool ok = parse_token(sv, "(", &sv);
    if (ok)
    {
        usize j = 0;
        do
        {
            i64 x;
            if ((ok = parse_int(sv, &x, &sv)))
            {
                // infof("int: %ld\n", x);
                m->switch_options[i] |= 1 << x;
            }
        } while (ok && j < MAX_LEN && parse_token(sv, ",", &sv));

        ok = parse_token(sv, ")", &sv);
    }

    // if (!ok)
    // {
    //     infof(
    //         "parse_switches_single: unexpected token " strview_fmts,
    //         strview_fmtv(sv)
    //     );
    // }

    if (ok && tail)
    {
        *tail = sv;
    }

    return ok;
}

static nodiscard bool
parse_switches(strview sv, machine *const m, strview *const tail)
{
    bool ok = false;
    for (usize i = 0; i < MAX_LEN && parse_switches_single(sv, m, i, &sv); ++i)
    {
        ok = true;
    }

    if (ok && tail)
    {
        *tail = sv;
    }

    return ok;
}

static nodiscard bool
parse_joltage(strview sv, machine *const m, strview *const tail)
{
    bool ok = parse_token(sv, "{", &sv);
    if (ok)
    {
        usize j = 0;
        do
        {
            i64 x;
            if ((ok = parse_int(sv, &x, &sv)))
            {
                // infof("int: %ld\n", x);
                m->joltage[j++] = (u8)x;
                m->joltage[j] = 0xff;
            }
        } while (ok && j < MAX_LEN && parse_token(sv, ",", &sv));

        ok = parse_token(sv, "}", &sv);
    }

    if (ok && tail)
    {
        *tail = sv;
    }

    return ok;
}

static nodiscard bool machine_parse(machine *m, strview sv)
{
    memset(m, 0, sizeof(*m));
    return parse_lights(sv, m, &sv)      //
           && parse_switches(sv, m, &sv) //
           && parse_joltage(sv, m, &sv);
}

static u8 bits_diff(u16 a, u16 b)
{
    u8 count = 0;
    for (usize i = 0; i < 16; ++i)
    {
        if (((a ^ b) & (1 << i)) == 0)
        {
            ++count;
        }
    }
    return count;
}

static bool machine_step(machine *m)
{
    usize best_idx = 0;
    u8 best_score = 0xff;
    for (usize i = 0; i < MAX_LEN && m->switch_options[i]; ++i)
    {
        u8 const score =
            bits_diff(m->lights_state, m->lights_state ^ m->switch_options[i]);

        if (best_score > score)
        {
            best_score = score;
            best_idx = i;
        }
    }
    expect(best_score < 0xff);

    infof("Apply option %zu\n", best_idx);
    m->lights_state ^= m->switch_options[best_idx];

    return m->lights_state != m->lights_goal;
}

i64 day10(FILE *const input, bool const part2)
{
    (void)part2;

    defer(strbuf_deinit) strbuf line = strbuf_init();

    i64 acc = 0;

    machine m;
    while (get_line(&line, input) && line.len > 0)
    {
        expect(machine_parse(&m, strbuf_as_strview(line)));

        while (machine_step(&m))
        {
            machine_print_info(&m);
        }
    }

    return acc;
}
