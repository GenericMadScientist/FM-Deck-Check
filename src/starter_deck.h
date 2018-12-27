#pragma once

#include <array>
#include <cstdint>

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
}
