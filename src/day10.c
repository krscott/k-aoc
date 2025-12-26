#include "aoc.h"
#include "ktl/lib/strings.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 10

typedef struct
{
    u8 num_lights;
    u16 lights_goal;
    u16 lights_state;
    u8 switches[MAX_LEN + 1][MAX_LEN + 1];
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

    for (usize i = 0; i < MAX_LEN && m->switches[i][0] != 0xff; ++i)
    {
        infof("(");
        for (usize j = 0; j < MAX_LEN && m->switches[i][j] != 0xff; ++j)
        {
            infof("%u", m->switches[i][j]);
            if (j < MAX_LEN - 1 && m->switches[i][j + 1] != 0xff)
            {
                infof(",");
            }
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
                m->switches[i][j++] = (u8)x;
                m->switches[i][j] = 0xff;
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
        if (i < MAX_LEN - 1)
        {
            m->switches[i + 1][0] = 0xff;
        }
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
    return parse_lights(sv, m, &sv)      //
           && parse_switches(sv, m, &sv) //
           && parse_joltage(sv, m, &sv);
}

static void machine_step(machine *m)
{
    usize best_idx = 0;
    u8 best_score = 0;
    for (usize i = 0; i < MAX_LEN && m->switches[i][0] != 0xff; ++i)
    {
        u16 new_state = m->lights_state;
        for (usize j = 0; j < MAX_LEN && m->switches[i][j] != 0xff; ++j)
        {
            new_state ^= 1 << (m->switches[i][j]);
        }

        u8 score = 0;
        for (usize k = 0; k < m->num_lights; ++k)
        {
            if ((m->lights_state >> k) == (new_state >> k))
            {
                ++score;
            }
        }

        if (best_score < score)
        {
            best_score = score;
            best_idx = i;
        }
    }
    expect(best_score > 0);
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
        machine_print_info(&m);
    }

    return acc;
}
