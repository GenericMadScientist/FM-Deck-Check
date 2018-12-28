#include <algorithm>

#include "rng.h"
#include "starter_deck.h"
#include "starter_deck_data.h"

namespace deck_check {
    constexpr auto groups = deck_groups();

    constexpr std::array<LinearMap, 723> card_rng_advances()
    {
        auto advances = std::array<LinearMap, 723>();
        auto current_advance = fm_rng_advance();

        for (auto i = 0; i <= 722; ++i) {
            advances[i] = current_advance;
            current_advance *= fm_rng_advance();
        }

        return advances;
    }

    constexpr auto card_advances = card_rng_advances();

    constexpr std::array<LinearMap, 42> first_group_advances()
    {
        auto advances = std::array<LinearMap, 42>();

        for (auto i = 0; i < 42; ++i)
            advances[i] = card_advances[first_group_members[i]];

        return advances;
    }

    constexpr auto first_group_advs = first_group_advances();

    constexpr uint32_t advance_to_next_card(int card, uint32_t seed)
    {
        return card_advances[card](seed);
    }

    starter_deck::starter_deck(uint32_t seed)
    {
        auto quantities = std::array<int8_t, 723>();
        auto cards_added = 0;
        auto cumulative_target_deck_size = 0;

        seed = next_seed(seed);

        for (auto i = 0; i < 7; ++i) {
            cumulative_target_deck_size += group_sizes[i];
            while (cards_added < cumulative_target_deck_size) {
                const auto new_card = groups[i][deck_pool_slot(seed)];
                seed = advance_to_next_card(new_card, seed);
                if (quantities[new_card] < 3) {
                    ++quantities[new_card];
                    cards[cards_added++] = new_card;
                }
            }
        }
    }

    int card_group(int card) noexcept
    {
        for (auto i = 0; i < 7; ++i) {
            if (std::find(groups[i].cbegin(), groups[i].cend(), card)
                != groups[i].cend())
                return i;
        }

        return -1;
    }

    starter_deck_filter::starter_deck_filter(const std::vector<int>& cards)
    {
        for (const auto c : cards) {
            if (card_counts[c] == 0) {
                auto group = card_group(c);
                if (group == -1)
                    invalid_cards_in_filter = true;
                else
                    filter_parts[group].push_back(c);
            }
            ++card_counts[c];
        }

        for (auto i = 0; i < 42; ++i)
            first_group_card_counts[i] = card_counts[first_group_members[i]];
    }

    bool starter_deck_filter::first_group_matches(uint32_t& seed) const noexcept
    {
        auto quantities = std::array<int8_t, 42>();
        auto cards_added = 0;

        while (cards_added < 16) {
            const auto new_card = first_group_pool[deck_pool_slot(seed)];
            seed = first_group_advs[new_card](seed);
            if (quantities[new_card] < 3) {
                ++quantities[new_card];
                ++cards_added;
            }
        }

        for (auto i = 0; i < 42; ++i)
            if (first_group_card_counts[i] > quantities[i])
                return false;

        return true;
    }

    bool starter_deck_filter::group_matches(uint32_t& seed, int group) const
        noexcept
    {
        auto quantities = std::array<int8_t, 723>();
        auto cards_added = 0;

        while (cards_added < group_sizes[group]) {
            const auto new_card = groups[group][deck_pool_slot(seed)];
            seed = advance_to_next_card(new_card, seed);
            if (quantities[new_card] < 3) {
                ++quantities[new_card];
                ++cards_added;
            }
        }

        return
            std::all_of(filter_parts[group].cbegin(),
                        filter_parts[group].cend(),
                        [&](int i){ return card_counts[i] <= quantities[i]; });
    }

    bool starter_deck_filter::deck_matches(uint32_t seed) const noexcept
    {
        seed = next_seed(seed);

        if (!first_group_matches(seed))
            return false;

        for (auto i = 1; i < 7; ++i)
            if (!group_matches(seed, i))
                return false;

        return true;
    }

    std::vector<int> starter_deck_filter::matching_decks(int first_frame,
                                                         int numb_of_frames)
        const
    {
        if (invalid_cards_in_filter)
            return std::vector<int>();

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
