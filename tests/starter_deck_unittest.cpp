#include <algorithm>

#include "gtest/gtest.h"

#include "starter_deck.h"

namespace deck_check {
    TEST(StarterDeckTest, DeckSize)
    {
        const auto deck = starter_deck(0);
        ASSERT_EQ(deck.cend() - deck.cbegin(), 40);
        ASSERT_EQ(std::cend(deck) - std::cbegin(deck), 40);
    }

    TEST(StarterDeckTest, LastFourCards)
    {
        const auto first_deck = starter_deck(0x55555555);
        const auto seed_zero_deck = starter_deck(0);

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
