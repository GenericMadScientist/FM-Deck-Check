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

    template<class DeckAddFun>
    void construct_deck(uint32_t seed, std::array<int8_t, 723>& quantities,
                        DeckAddFun f)
    {
        auto cards_added = 0;

        for (auto i = 0; i < 7; ++i) {
            const auto group_size = cumulative_group_sizes[i];
            while (cards_added < group_size) {
                seed = next_seed(seed);
                const auto new_card = groups[i][deck_pool_slot(seed)];
                seed = card_advances[new_card](seed);
                if (quantities[new_card] < 3) {
                    ++quantities[new_card];
                    f(new_card, cards_added);
                    ++cards_added;
                }
            }
        }
    }

    starter_deck::starter_deck(uint32_t seed)
    {
        auto quantities = std::array<int8_t, 723>();
        construct_deck(seed, quantities,
                       [&](int card, int card_count)
                       { cards[card_count] = card; });
    }

    starter_deck_filter::starter_deck_filter(const std::vector<int>& cards)
    {
        for (const auto c : cards) {
            if (card_counts[c] == 0)
                filter_cards.push_back(c);
            ++card_counts[c];
        }
    }

    bool starter_deck_filter::deck_matches(uint32_t seed) const noexcept
    {
        auto quantities = std::array<int8_t, 723>();
        construct_deck(seed, quantities, [](int, int){});
        return
            std::all_of(filter_cards.cbegin(), filter_cards.cend(),
                        [&](int i){ return card_counts[i] <= quantities[i]; });
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
