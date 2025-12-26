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
    u16 lights;
    u8 switches[MAX_LEN + 1][MAX_LEN + 1];
    u8 joltage[MAX_LEN + 1];
} machine;

static void machine_print_info(machine const *const m)
{
    infof("[");
    for (usize i = 0; i < m->num_lights; ++i)
    {
        if ((m->lights >> i) & 1)
        {
            infof("#");
        }
        else
        {
            infof(".");
        }
    }
    infof("] ");

    for (usize i = 0; i < MAX_LEN && m->switches[i][0] != 0xff; ++i)
    {
        infof("(");
        for (usize j = 0; j < MAX_LEN && m->switches[i][j] != 0xff; ++j)
        {
            infof("%u,", m->switches[i][j]);
        }
        infof(") ");
    }

    infof("\n");
}

static void machine_parse(machine *m, strview sv)
{
    (void)memset(m, 0xff, sizeof(*m));
    m->num_lights = 0;
    m->lights = 0;

    char ch;
    expect(strview_split_first(sv, &ch, &sv) && ch == '[');

    while (strview_split_first(sv, &ch, &sv) && ch != ']')
    {

        switch (ch)
        {
            case '#':
                m->lights |= 1 << m->num_lights;
                break;
            case '.':
                break;

            default:
                panicf("Unexpected char: %c", ch);
                break;
        }

        ++(m->num_lights);
    }

    for (usize i = 0;
         strview_split_first(strview_trim_start(sv), &ch, &sv) && ch == '(';
         ++i)
    {
        infof("%.*s\n", (int)sv.len, sv.ptr);

        i64 id;
        for (usize j = 0; parse_int(sv, &id, &sv); ++j)
        {
            infof("%ld\n", id);
            expect(j < MAX_LEN);
            expect(0 <= id && id < 0xff);
            m->switches[i][j] = (u8)id;
            m->switches[i][j + 1] = 0xff;

            // TODO check for comma
        }

        expect(i < MAX_LEN);
        m->switches[i + 1][0] = 0xff;

        // TODO check for end paren
    }
}

i64 day10(FILE *const input, bool const part2)
{
    (void)part2;

    defer(strbuf_deinit) strbuf line = strbuf_init();

    i64 acc = 0;

    machine m;
    while (get_line(&line, input) && line.len > 0)
    {
        machine_parse(&m, strbuf_as_strview(line));
        machine_print_info(&m);
    }

    return acc;
}
