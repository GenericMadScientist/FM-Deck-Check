#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace deck_check {
    class starter_deck_filter;

    class filter_results {
    private:
        int max_numb_of_stored_results; // Not const to allow copy assignment.
        int results = 0;
        std::vector<int> first_results;
        void add_result(int result);
        void add_result_set(const filter_results& rhs);
        friend class starter_deck_filter;
    public:
        explicit filter_results(int limit = 1000)
            : max_numb_of_stored_results{limit} {}
        int numb_of_results() const noexcept { return results; }
        const std::vector<int>& initial_results() const noexcept
        {
            return first_results;
        }
    };

    std::vector<int> helpful_hints(const std::vector<int>& frames);

    class starter_deck_filter {
    private:
        std::array<int8_t, 723> card_counts = std::array<int8_t, 723>();
        std::array<int8_t, 42> first_group_card_counts;
        std::array<std::vector<int>, 7> filter_parts;
        bool invalid_cards_in_filter = false;
        bool first_group_matches(uint32_t& seed) const noexcept;
        bool group_matches(uint32_t& seed, int group) const noexcept;
        bool deck_matches(uint32_t seed) const noexcept;
        filter_results decks_in_range(int first_frame, int numb_of_frames)
            const;
    public:
        explicit starter_deck_filter(const std::vector<int>& cards);
        filter_results matching_decks(int first_frame, int numb_of_frames)
            const;
        filter_results all_matching_decks() const;
    };
}
