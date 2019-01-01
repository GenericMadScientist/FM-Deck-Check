#include <iostream>
#include <vector>

#include "options.h"
#include "starter_deck.h"

namespace deck_check {
    cxxopts::Options program_options()
    {
        auto options = cxxopts::Options("deck_check",
                                        "Checks FM starter decks.");

        options.add_options()
            ("c,card", "Add a card to the filter.",
             cxxopts::value<std::vector<int>>())
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

        if (input.count("card")) {
            const auto& cards = input["card"].as<std::vector<int>>();
            std::cout << "Please wait..." << std::endl;
            const auto filter = starter_deck_filter(cards);
            const auto results = filter.all_matching_decks();
            if (results.numb_of_results() == 1)
                std::cout << "There is one matching deck.";
            else
                std::cout << "There are " << results.numb_of_results()
                          << " matching decks.";
            std::cout << std::endl;
        } else
            std::cout << "Please enter at least one card." << std::endl;
    }
}
