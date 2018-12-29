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

    TEST(FilterTest, Matches)
    {
        const auto dh_filter = starter_deck_filter(std::vector<int>({336}));

        const auto dh_decks_in_first_ten = std::vector<int>({5, 6, 7});
        const auto dh_decks_in_later_range =
            std::vector<int>({1006, 1007, 1008});

        ASSERT_EQ(dh_filter.matching_decks(0, 10), dh_decks_in_first_ten);
        ASSERT_EQ(dh_filter.matching_decks(1000, 10),
                  dh_decks_in_later_range);
    }

    TEST(FilterTest, InvalidCards)
    {
        const auto mobc_filter = starter_deck_filter(std::vector<int>({722}));
        const auto glitch_card_filter =
            starter_deck_filter(std::vector<int>({900}));

        const auto no_results = std::vector<int>();

        ASSERT_EQ(mobc_filter.matching_decks(0, 100), no_results);
        ASSERT_EQ(glitch_card_filter.matching_decks(0, 100), no_results);
    }
}
