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
}
