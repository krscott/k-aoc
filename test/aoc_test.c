// #include "aoc.h"

#include "aoc.h"
#include "ktest.inc"

KTEST_MAIN
{
    KTEST(t_modulo)
    {
        ASSERT_INT_EQ(modulo(0, 100), 0);
        ASSERT_INT_EQ(modulo(30, 100), 30);
        ASSERT_INT_EQ(modulo(100, 100), 0);
        ASSERT_INT_EQ(modulo(130, 100), 30);
        ASSERT_INT_EQ(modulo(200, 100), 0);

        ASSERT_INT_EQ(modulo(-30, 100), 70);
        ASSERT_INT_EQ(modulo(-100, 100), 0);
        ASSERT_INT_EQ(modulo(-130, 100), 70);
        ASSERT_INT_EQ(modulo(-200, 100), 0);
    }

    KTEST(t_day01_count_hits)
    {
        ASSERT_INT_EQ(day01_count_hits(50, 0), 0);
        ASSERT_INT_EQ(day01_count_hits(50, 50), 1);
        ASSERT_INT_EQ(day01_count_hits(50, 150), 2);

        ASSERT_INT_EQ(day01_count_hits(0, 1), 0);
        ASSERT_INT_EQ(day01_count_hits(0, 100), 1);
        ASSERT_INT_EQ(day01_count_hits(0, 150), 1);
        ASSERT_INT_EQ(day01_count_hits(0, 200), 2);

        ASSERT_INT_EQ(day01_count_hits(50, -50), 1);
        ASSERT_INT_EQ(day01_count_hits(50, -100), 1);
        ASSERT_INT_EQ(day01_count_hits(50, -150), 2);
        ASSERT_INT_EQ(day01_count_hits(50, -200), 2);

        ASSERT_INT_EQ(day01_count_hits(0, -1), 0);
        ASSERT_INT_EQ(day01_count_hits(0, -100), 1);
        ASSERT_INT_EQ(day01_count_hits(0, -150), 1);
        ASSERT_INT_EQ(day01_count_hits(0, -200), 2);
    }
}
