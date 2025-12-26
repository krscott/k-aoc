#ifndef AOC_MACROS_H_
#define AOC_MACROS_H_

#include <stdio.h> // IWYU pragma: export

#if defined(__GNUC__) || defined(__clang__)
#define nodiscard __attribute__((warn_unused_result))
#else
#define nodiscard
#endif

#define defer(fn) __attribute__((cleanup(fn)))

#define infof(...)                                                             \
    do                                                                         \
    {                                                                          \
        if (aoc_is_verbose)                                                    \
        {                                                                      \
            fprintf(stderr, __VA_ARGS__);                                      \
        }                                                                      \
    } while (0)

#define panicf(...)                                                            \
    do                                                                         \
    {                                                                          \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
        fflush(stderr);                                                        \
        assert(false);                                                         \
        exit(1);                                                               \
    } while (0)

#define expect(cond)                                                           \
    do                                                                         \
    {                                                                          \
        if (!(cond))                                                           \
        {                                                                      \
            panicf("Expected: " #cond);                                        \
        }                                                                      \
    } while (0)

#define ignore(cond)                                                           \
    do                                                                         \
    {                                                                          \
        _Bool cond_ = (cond);                                                  \
        (void)cond_;                                                           \
    } while (0)

#endif
