#pragma once

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
}
