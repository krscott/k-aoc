#include "aoc.h"
#include "ktl/macros.h"
#include <assert.h>

// Dev-only
#ifndef aoc_pathfind
i64 dev_h(u16 const n);
#define dev_pathfind__node u16
#define dev_pathfind__h dev_h
#define aoc_pathfind dev_pathfind
#endif

// Macros

KTL_DIAG_PUSH
KTL_DIAG_IGNORE("-Wundef")

#undef aoc_pathfind_m
#define aoc_pathfind_m(x) KTL_TEMPLATE_lv2(aoc_pathfind, x)

#undef aoc_pathfind_T
#define aoc_pathfind_T aoc_pathfind_m(_node)

#undef aoc_pathfind_H
#define aoc_pathfind_H aoc_pathfind_m(_h)

#undef aoc_pathfind_infallible_allocator
#define aoc_pathfind_infallible_allocator aoc_pathfind_m(_infallible_allocator)

KTL_DIAG_POP

#ifndef KTL_INC

// Node Graph
#define pathfindgraph__key aoc_pathfind_T
#define pathfindgraph__value aoc_pathfind_T
#define pathfindgraph__infallible_allocator aoc_pathfind_infallible_allocator
#define ktl_hashmap pathfindgraph
#include "ktl/struct/hashmap.h"

#undef ktl_hashmap

// Node Score
#define pathfindscore__key aoc_pathfind_T
#define pathfindscore__value i64
#define pathfindscore__infallible_allocator aoc_pathfind_infallible_allocator
#define ktl_hashmap pathfindscore
#include "ktl/struct/hashmap.h"
#undef ktl_hashmap

bool aoc_pathfind_m(pathfind)(pathfindgraph *const g, u16 const start, u16 const goal);

#endif // KTL_INC
