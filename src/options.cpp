#include <cstdlib>
#include <iostream>

#include "options.h"

namespace deck_check {
    cxxopts::Options program_options()
    {
        auto options = cxxopts::Options("deck_check",
                                        "Checks FM starter decks.");

        options.add_options()
            ("h,help", "Print help and exit.")
            ;

        return options;
    }

    void execute_according_to_input(const cxxopts::ParseResult& input,
                                    const cxxopts::Options& options)
    {
        if (input.count("help")) {
            std::cout << options.help() << std::endl;
            return;
        }
    }
}
