#include <array>
#include <vector>

#include "gtest/gtest.h"

#include "starter_deck.h"

namespace deck_check {
    TEST(FilterTest, Matches)
    {
        const auto dh_filter = starter_deck_filter(std::vector<int>({336}));

        const auto dh_decks_in_first_ten = std::vector<int>({5, 6, 7});
        const auto dh_decks_in_later_range =
            std::vector<int>({1006, 1007, 1008});

        ASSERT_EQ(dh_filter.matching_decks(0, 10).initial_results(),
                  dh_decks_in_first_ten);
        ASSERT_EQ(dh_filter.matching_decks(1000, 10).initial_results(),
                  dh_decks_in_later_range);
    }

    TEST(FilterTest, InvalidCards)
    {
        const auto mobc_filter = starter_deck_filter(std::vector<int>({722}));
        const auto glitch_card_filter =
            starter_deck_filter(std::vector<int>({900}));

        const auto no_results = std::vector<int>();

        ASSERT_EQ(mobc_filter.matching_decks(0, 100).initial_results(),
                  no_results);
        ASSERT_EQ(glitch_card_filter.matching_decks(0, 100).initial_results(),
                  no_results);
    }

    TEST(HintTest, ManipDecks)
    {
        const auto manip_decks = std::vector<int>({706, 711});
        const auto manip_diffs = std::vector<int>({558, 563});

        ASSERT_EQ(helpful_hints(manip_decks), manip_diffs);
    }
}
