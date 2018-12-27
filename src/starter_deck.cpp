#include <algorithm>

#include "rng.h"
#include "starter_deck.h"
#include "starter_deck_data.h"

namespace deck_check {
    constexpr auto groups = deck_groups();

    constexpr std::array<LinearMap, 723> card_rng_advances()
    {
        auto advances = std::array<LinearMap, 723>();
        auto current_advance = LinearMap();

        for (auto i = 0; i <= 722; ++i) {
            advances[i] = current_advance;
            current_advance *= fm_rng_advance();
        }

        return advances;
    }

    constexpr auto card_advances = card_rng_advances();

    starter_deck::starter_deck(uint32_t seed)
    {
        auto cards_added = 0;
        auto quantities = std::array<int8_t, 723>();

        for (auto i = 0; i < 7; ++i) {
            const auto group_size = cumulative_group_sizes[i];
            while (cards_added < group_size) {
                seed = next_seed(seed);
                const auto new_card = groups[i][deck_pool_slot(seed)];
                seed = card_advances[new_card](seed);
                if (quantities[new_card] < 3) {
                    cards[cards_added++] = new_card;
                    ++quantities[new_card];
                }
            }
        }
    }
}
