#include "ktl/lib/integers.h"

#ifndef aoc_pathfind // dev-only
#include "aoc_pathfind.h"
#endif

#define KTL_INC
#include "aoc_pathfind.h"
#undef KTL_INC

#define ktl_hashmap pathfindgraph
#include "ktl/struct/hashmap.inc"
#undef ktl_hashmap

#define ktl_hashmap pathfindscore
#include "ktl/struct/hashmap.inc"
#undef ktl_hashmap

bool aoc_pathfind_m(dijkstra)(
    pathfindgraph *const g, u16 const start, u16 const goal
)
{
    pathfindgraph_clear(g);

    return false;
}
