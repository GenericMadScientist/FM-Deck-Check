#pragma once

#include <array>
#include <cstdint>

namespace deck_check {
    class LinearMap {
    private:
        uint32_t m = 1;
        uint32_t i = 0;
    public:
        constexpr LinearMap() noexcept {}
        constexpr LinearMap(uint32_t mult, uint32_t inc) noexcept
        : m{mult}, i{inc} {}

        constexpr uint32_t operator()(uint32_t seed) const noexcept
            {
                return m * seed + i;
            }
        constexpr LinearMap& operator*=(const LinearMap rhs) noexcept
            {
                i += m * rhs.i;
                m *= rhs.m;
                return *this;
            }
    };

    constexpr LinearMap operator*(const LinearMap lhs, const LinearMap rhs)
        noexcept
    {
        auto product = lhs;
        product *= rhs;
        return product;
    }

    constexpr LinearMap fm_rng_advance() noexcept
    {
        return LinearMap(0x41C64E6D, 0x3039);
    }

    constexpr uint32_t next_seed(uint32_t seed) noexcept
    {
        return fm_rng_advance()(seed);
    }

    constexpr uint32_t deck_pool_slot(uint32_t seed) noexcept
    {
        return (seed >> 16) & 0x7FF;
    }

    constexpr uint32_t initial_seed = 0x55555555;

    constexpr uint32_t nth_seed_after(uint32_t seed, int n) noexcept
    {
        if (n < 0) {
            // Safe way to add 0x8000000 without integer overflow
            n += 0x7FFFFFFF;
            ++n;

            seed += 0x80000000u;
        }

        auto advance = fm_rng_advance();

        while (n > 0) {
            if (n & 1)
                seed = advance(seed);
            advance *= advance;
            n >>= 1;
        }

        return seed;
    }

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

    constexpr std::array<LinearMap, 723> card_advances = card_rng_advances();

    constexpr uint32_t advance_to_next_card(int card, uint32_t seed)
    {
        return card_advances[card](seed);
    }

    std::array<int, 40> starter_deck(int frame_number);
}
