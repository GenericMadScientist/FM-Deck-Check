#include <algorithm>

#include "rng.h"
#include "starter_deck.h"
#include "starter_deck_data.h"

namespace deck_check {
    constexpr auto groups = deck_groups();

    starter_deck::starter_deck(uint32_t seed)
    {
        auto cards_added = 0;

        for (auto i = 0; i < 7; ++i) {
            auto cards_added_in_group = 0;
            while (cards_added_in_group < group_sizes[i]) {
                seed = next_seed(seed);
                auto slot = deck_pool_slot(seed);
                auto new_card = groups[i][slot];
                for (auto j = 0; j < new_card; ++j)
                    seed = next_seed(seed);
                if (std::count(std::cbegin(cards), std::cbegin(cards) + cards_added,
                               new_card) < 3) {
                    cards[cards_added] = new_card;
                    ++cards_added;
                    ++cards_added_in_group;
                }
            }
        }
    }
}
