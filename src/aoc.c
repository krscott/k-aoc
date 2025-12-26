#include "aoc.h"

#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"
#include "macros.h"
#include "string.h"
#include <assert.h>
#include <stdio.h>

bool aoc_is_verbose = false;

nodiscard bool
append_stream_until(strbuf *const buf, FILE *const stream, char const delim)
{
    int c = fgetc(stream);
    bool const ok = c != EOF;

    for (; c != EOF && c != delim; c = fgetc(stream))
    {
        strbuf_push(buf, (char)c);
    }

    return ok;
}

nodiscard bool
get_split(strbuf *const buf, FILE *const stream, char const delim)
{
    assert(stream);

    strbuf_clear(buf);
    strbuf_reserve(buf, 256);
    buf->ptr[0] = '\0';

    return append_stream_until(buf, stream, delim);
}

nodiscard bool get_line(strbuf *const buf, FILE *const stream)
{
    return get_split(buf, stream, '\n');
}

nodiscard bool str2int(str s, i64 *out)
{
    strview sv = strview_trim(strview_from_str(s));
    i64 tmp;
    bool const ok = parse_int(sv, &tmp, &sv) && sv.len == 0;
    if (ok)
    {
        *out = tmp;
    }
    return ok;
}

nodiscard bool parse_int(strview s, i64 *out, strview *tail)
{
    bool ok = false;
    i64 acc = 0;

    s = strview_trim_start(s);

    if (s.len > 0)
    {
        bool const neg = (s.ptr[0] == '-');
        if (neg)
        {
            strview_split_at(s, 1, NULL, &s);
        }

        char c;
        strview tmp;
        while (strview_split_first(s, &c, &tmp))
        {
            if ('0' <= c && c <= '9')
            {
                // Found any correct digit
                ok = true;
                s = tmp;

                if (neg)
                {
                    acc = acc * 10 - (i64)(c - '0');
                }
                else
                {
                    acc = acc * 10 + (i64)(c - '0');
                }
            }
            else
            {
                break;
            }
        }
    }

    if (ok)
    {
        if (out)
        {
            *out = acc;
        }
        if (tail)
        {
            *tail = s;
        }
    }
    return ok;
}

nodiscard bool
parse_expect(strview input, char const *const match, strview *const output)
{
    strview const match_sv = strview_from_terminated(match);
    bool const ok = strview_starts_with(input, match_sv);
    if (ok)
    {
        strview_split_at(input, match_sv.len, NULL, output);
    }
    return ok;
}

#define chargrid__list strbuf
#define grid chargrid
#include "gen/grid.inc"
#undef grid

void chargrid_read_stream(chargrid *const g, FILE *const input)
{
    for (;;)
    {
        int c = getc(input);
        if (c == EOF)
        {
            break;
        }
        if (c == '\n' || c == '\r')
        {
            if (g->width == 0)
            {
                g->width = g->buf.len;
            }
        }
        else
        {
            chargrid_push(g, (char)c);
        }
    }
}

void chargrid_print_info(chargrid const *const g)
{
    assert(g->width > 0);
    usize const height = chargrid_height(g);
    for (usize row = 0; row < height; ++row)
    {
        infof("%.*s\n", (int)g->width, chargrid_at(g, row, 0));
    }
}

#define ktl_vec intlist
#include "ktl/struct/vec.inc"
#undef ktl_vec

#define grid intgrid
#include "gen/grid.inc"
#undef grid

void intgrid_print_info(intgrid const *const g, int const cell_padding)
{
    assert(g->width > 0);
    usize const height = intgrid_height(g);
    for (usize row = 0; row < height; ++row)
    {
        for (usize col = 0; col < g->width; ++col)
        {
            i64 val = intgrid_get(g, row, col);
            if (val == 0)
            {
                infof("%*c ", cell_padding, ' ');
            }
            else
            {
                infof("%*ld ", cell_padding, val);
            }
        }
        infof("\n");
    }
}

void intgrid_add(
    intgrid const *const g, usize const row, usize const col, i64 const addition
)
{
    i64 const prev = intgrid_get(g, row, col);
    intgrid_set(g, row, col, prev + addition);
}
