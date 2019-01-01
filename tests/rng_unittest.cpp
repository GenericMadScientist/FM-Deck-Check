#include <algorithm>

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
        ASSERT_EQ(next_seed(0), 0x3039u);
        ASSERT_EQ(next_seed(1), 0x41C67EA6u);
        ASSERT_EQ(next_seed(2), 0x838CCD13u);
    }

    TEST(DeckPoolSlotTest, Prns)
    {
        ASSERT_EQ(deck_pool_slot(0u), 0);
        ASSERT_EQ(deck_pool_slot(1u), 0);
        ASSERT_EQ(deck_pool_slot(0x10000u), 1);
        ASSERT_EQ(deck_pool_slot(0x7FF0000u), 0x7FF);
        ASSERT_EQ(deck_pool_slot(0x8000000u), 0);
    }

    TEST(RngMultiAdvanceTest, MultiAdvances)
    {
        ASSERT_EQ(nth_seed(0), 0x55555555u);
        ASSERT_EQ(nth_seed(1000000), 0xFF60ED15u);
    }

    TEST(StarterDeckTest, LastFourCards)
    {
        const auto first_deck = starter_deck(0);
        const auto seed_zero_deck = starter_deck(-1199997605);

        constexpr auto first_deck_last_cards =
            std::array<int, 4>({221, 337, 332, 309});
        constexpr auto seed_zero_last_cards =
            std::array<int, 4>({115, 337, 334, 659});

        ASSERT_TRUE(std::equal(std::cbegin(first_deck) + 36,
                               std::cend(first_deck),
                               std::cbegin(first_deck_last_cards)));
        ASSERT_TRUE(std::equal(std::cbegin(seed_zero_deck) + 36,
                               std::cend(seed_zero_deck),
                               std::cbegin(seed_zero_last_cards)));
    }
}
