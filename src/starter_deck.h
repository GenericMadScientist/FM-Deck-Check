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

    class starter_deck_filter {
    private:
        std::array<int8_t, 723> card_counts = std::array<int8_t, 723>();
        std::array<std::vector<int>, 7> filter_parts;
        bool invalid_cards_in_filter = false;
        bool group_matches(uint32_t& seed, int group) const noexcept;
        bool deck_matches(uint32_t seed) const noexcept;
    public:
        explicit starter_deck_filter(const std::vector<int>& cards);
        std::vector<int> matching_decks(int first_frame, int numb_of_frames)
            const;
    };
}
