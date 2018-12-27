#include "gtest/gtest.h"

#include "rng.h"

namespace deck_check {
    TEST(LinearMapTest, CallOperator)
    {
        const auto zero_map = LinearMap(0, 0);
        const auto identity_map = LinearMap(1, 0);
        const auto increment_map = LinearMap(1, 1);
        const auto double_map = LinearMap(2, 0);

        ASSERT_EQ(zero_map(0), 0);
        ASSERT_EQ(zero_map(1), 0);

        ASSERT_EQ(identity_map(0), 0);
        ASSERT_EQ(identity_map(1), 1);

        ASSERT_EQ(increment_map(0), 1);
        ASSERT_EQ(increment_map(1), 2);

        ASSERT_EQ(double_map(0), 0);
        ASSERT_EQ(double_map(1), 2);
    }

    TEST(LinearMap, ComposeOperator)
    {
        const auto zero_map = LinearMap(0, 0);
        const auto identity_map = LinearMap(1, 0);
        const auto increment_map = LinearMap(1, 1);
        const auto double_map = LinearMap(2, 0);

        ASSERT_EQ((zero_map * identity_map)(0), zero_map(identity_map(0)));
        ASSERT_EQ((zero_map * identity_map)(1), zero_map(identity_map(1)));

        ASSERT_EQ((double_map * increment_map)(1), double_map(increment_map(1)));
        ASSERT_EQ((double_map * increment_map)(1), double_map(increment_map(1)));
    }

    TEST(RngAdvanceTest, Advances)
    {
        ASSERT_EQ(next_seed(0), 0x3039);
        ASSERT_EQ(next_seed(1), 0x41C67EA6);
        ASSERT_EQ(next_seed(2), 0x838CCD13);
    }

    TEST(DeckPoolSlotTest, Prns)
    {
        ASSERT_EQ(deck_pool_slot(0), 0);
        ASSERT_EQ(deck_pool_slot(1), 0);
        ASSERT_EQ(deck_pool_slot(0x10000), 1);
        ASSERT_EQ(deck_pool_slot(0x7FF0000), 0x7FF);
        ASSERT_EQ(deck_pool_slot(0x8000000), 0);
    }
}
