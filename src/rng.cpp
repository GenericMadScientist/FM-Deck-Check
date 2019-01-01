#include "rng.h"
#include "starter_deck_data.h"

namespace deck_check {
    std::array<int, 40> starter_deck(int deck_number)
    {
        auto seed = nth_seed_after(initial_seed, deck_number);
        auto cards = std::array<int, 40>();
        auto quantities = std::array<int8_t, 723>();
        auto cards_added = 0;
        auto cumulative_target_deck_size = 0;

        seed = next_seed(seed);

        for (auto i = 0; i < 7; ++i) {
            cumulative_target_deck_size += group_sizes[i];
            while (cards_added < cumulative_target_deck_size) {
                const auto new_card = deck_groups[i][deck_pool_slot(seed)];
                seed = advance_to_next_card(new_card, seed);
                if (quantities[new_card] < 3) {
                    ++quantities[new_card];
                    cards[cards_added++] = new_card;
                }
            }
        }

        return cards;
    }
}
