#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace deck_check {
    class starter_deck {
        using const_iterator = std::array<int, 40>::const_iterator;
    private:
        std::array<int, 40> cards;
    public:
        explicit starter_deck(uint32_t seed);
        constexpr const_iterator begin() const noexcept
        {
            return cards.begin();
        }
        constexpr const_iterator end() const noexcept
        {
            return cards.end();
        }
        constexpr const_iterator cbegin() const noexcept
        {
            return cards.cbegin();
        }
        constexpr const_iterator cend() const noexcept
        {
            return cards.cend();
        }
    };

    class starter_deck_filter;

    class filter_results {
    private:
        int max_numb_of_stored_results; // Not const to allow copy assignment.
        int results = 0;
        std::vector<int> first_results;
        void add_result(int result);
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
        std::vector<int> matching_decks(int first_frame, int numb_of_frames)
            const;
    };
}
