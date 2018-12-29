#pragma once

#include "cxxopts.hpp"

namespace deck_check {
    cxxopts::Options program_options();
    void execute_according_to_input(const cxxopts::ParseResult& input,
                                    const cxxopts::Options& options);
}
