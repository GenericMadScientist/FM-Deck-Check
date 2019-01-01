#include <algorithm>
#include <numeric>

#include "rng.h"
#include "starter_deck.h"
#include "starter_deck_data.h"

namespace deck_check {
    static constexpr std::array<LinearMap, 42> first_group_advances()
    {
        auto advances = std::array<LinearMap, 42>();

        for (auto i = 0; i < 42; ++i)
            advances[i] = card_advances[first_group_members[i]];

        return advances;
    }

    constexpr auto first_group_advs = first_group_advances();

    void filter_results::add_result(int result)
    {
        if (static_cast<int>(first_results.size()) < max_numb_of_stored_results)
            first_results.push_back(result);

        ++results;
    }

    void filter_results::add_result_set(const filter_results& rhs)
    {
        const auto capacity = std::max(max_numb_of_stored_results - results, 0);
        const auto numb_to_add =
            std::min(static_cast<int>(rhs.first_results.size()), capacity);
        first_results.insert(first_results.end(), rhs.first_results.cbegin(),
                             rhs.first_results.cbegin() + numb_to_add);
        results += rhs.results;
    }

    struct frequency_rating {
        int16_t card;
        int8_t quantity;
        int8_t separation_power;
    };

    static bool freq_rating_comp(const frequency_rating& lhs,
                          const frequency_rating& rhs) noexcept
    {
        if (lhs.separation_power != rhs.separation_power)
            return lhs.separation_power < rhs.separation_power;

        if (lhs.card != rhs.card)
            return lhs.card < rhs.card;

        return lhs.quantity < rhs.quantity;
    }

    std::vector<int> helpful_hints(const std::vector<int>& frames)
    {
        auto counts = std::array<std::array<int, 3>, 723>();

        for (const auto frame : frames) {
            const auto deck = starter_deck(frame);
            auto deck_counts = std::array<int8_t, 723>();
            for (const auto card : deck)
                ++deck_counts[card];
            for (auto i = 1; i < 723; ++i)
                for (auto j = 0; j < deck_counts[i]; ++j)
                    ++counts[i][j];
        }

        auto ratings = std::vector<frequency_rating>();

        for (auto i = 0; i < 723; ++i)
            for (auto j = 0; j < 3; ++j) {
                const auto count = counts[i][j];
                const auto usefulness =
                    std::min(count,
                             static_cast<int>(frames.size()) - count);
                if (usefulness > 0) {
                    auto rating = frequency_rating();
                    rating.card = i;
                    rating.quantity = j + 1;
                    rating.separation_power = usefulness;
                    ratings.push_back(rating);
                }
            }

        std::sort(ratings.begin(), ratings.end(), freq_rating_comp);

        auto hints = std::vector<int>();

        for (const auto& rating : ratings) {
            const auto card = rating.card;
            if (std::find(hints.cbegin(), hints.cend(), card) == hints.cend())
                hints.push_back(card);
        }

        return hints;
    }

    starter_deck_filter::starter_deck_filter(const std::vector<int>& cards)
    {
        for (const auto c : cards) {
            auto group = -1;
            if ((c >= 1) && (c <= 722))
                group = card_groups[c];

            if (group == -1)
                invalid_cards_in_filter = true;
            else if (card_counts[c]++ == 0)
                filter_parts[group].push_back(c);
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
            const auto new_card = deck_groups[group][deck_pool_slot(seed)];
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

    filter_results starter_deck_filter::decks_in_range(int first_frame,
                                                       int numb_of_frames)
        const
    {
        if (invalid_cards_in_filter)
            return filter_results();

        auto seed = nth_seed(first_frame);
        auto frames = filter_results();

        for (auto i = 0; i < numb_of_frames; ++i) {
            if (deck_matches(seed))
                frames.add_result(first_frame + i);
            seed = next_seed(seed);
        }

        return frames;
    }

    filter_results starter_deck_filter::matching_decks(int first_frame,
                                                       int numb_of_frames)
        const
    {
        constexpr auto numb_of_decks = 134217728;
        constexpr auto decks_per_task = 1048576;
        constexpr auto numb_of_tasks = (numb_of_decks - 1) / decks_per_task + 1;

        auto subresults = std::array<filter_results, numb_of_tasks>();

        #pragma omp parallel for
        for (auto i = 0; i < numb_of_frames; i += decks_per_task) {
            const auto frames_in_subjob = std::min(decks_per_task,
                                                   numb_of_frames - i);
            subresults[i / decks_per_task] = decks_in_range(first_frame + i,
                                                            frames_in_subjob);
        }

        auto results = filter_results();

        for (const auto& v : subresults)
            results.add_result_set(v);

        return results;
    }

    filter_results starter_deck_filter::all_matching_decks() const
    {
        constexpr auto numb_of_decks = 134217728;
        return matching_decks(0, numb_of_decks);
    }
}
