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

    bool starter_deck_filter::deck_matches(uint32_t seed) const noexcept
    {
        const auto deck = starter_deck(seed);
        auto quantities = std::array<int8_t, 723>();
        for (const auto deck_card : deck)
            ++quantities[deck_card];
        for (const auto filter_card : filter_cards)
            --quantities[filter_card];
        return std::all_of(quantities.cbegin(), quantities.cend(),
                           [](int x){ return x >= 0; });
    }

    std::vector<int> starter_deck_filter::matching_decks(int first_frame,
                                                         int numb_of_frames)
        const
    {
        auto seed = nth_seed_after(initial_seed, first_frame);
        auto frames = std::vector<int>();

        for (auto i = 0; i < numb_of_frames; ++i) {
            if (deck_matches(seed))
                frames.push_back(first_frame + i);
            seed = next_seed(seed);
        }

        return frames;
    }
}
