#include "aoc.h"

#include "ktl/lib/strings.h"
#include "ktl/lib/strings.inc"
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
    s = str_trim(s);

    bool const neg = (s.ptr[0] == '-');
    if (neg)
    {
        str_split_at(s, 1, NULL, &s);
    }

    bool ok = false;
    i64 acc = 0;

    for (usize i = 0; i < s.len; ++i)
    {
        char const c = s.ptr[i];
        ok = '0' <= c && c <= '9';
        if (ok)
        {
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

    if (ok)
    {
        *out = acc;
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

void chargrid_print_info(chargrid const g)
{
    assert(g.width > 0);
    usize const height = chargrid_height(g);
    for (usize row = 0; row < height; ++row)
    {
        char *ptr;
        expect(chargrid_at(g, row, 0, &ptr));
        infof("%.*s\n", (int)g.width, ptr);
    }
}

#define ktl_vec intlist
#include "ktl/struct/vec.inc"
#undef ktl_vec

#define grid intgrid
#include "gen/grid.inc"
#undef grid

void intgrid_print_info(intgrid const g, int const cell_padding)
{
    assert(g.width > 0);
    usize const height = intgrid_height(g);
    for (usize row = 0; row < height; ++row)
    {
        for (usize col = 0; col < g.width; ++col)
        {
            i64 val;
            expect(intgrid_get(g, row, col, &val));
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

bool intgrid_add(
    intgrid const g, usize const row, usize const col, i64 const addition
)
{
    i64 prev;
    bool const ok = intgrid_get(g, row, col, &prev);
    if (ok)
    {
        expect(intgrid_set(g, row, col, prev + addition));
    }
    return ok;
}
