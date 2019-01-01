#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <vector>

#include "options.h"
#include "rng.h"
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

    void print_card(int card)
    {
        std::cout << std::setfill('0') << std::setw(3) << card
                  << std::setfill(' ')
                  << ' ' << "Card name " << '\n';
    }

    void print_found_deck(const filter_results& results)
    {
        std::cout << "The deck has been found!\n\n"
                  << "Matching seeds:\n";
        for (const auto frame : results.initial_results())
            std::cout << "Seed " << frame << '\n';

        std::cout << "\nCards: \n";

        const auto first_seed = nth_seed_after(initial_seed,
                                               results.initial_results()[0]);
        const auto deck = starter_deck(first_seed);
        auto deck_copy = std::array<int, 40>();
        std::copy(deck.cbegin(), deck.cend(), deck_copy.begin());
        std::sort(deck_copy.begin(), deck_copy.end());

        for (const auto card : deck_copy)
            print_card(card);
    }

    void print_hints_or_deck(const filter_results& results)
    {
        if (results.numb_of_results() == 0)
            return;

        auto hints = helpful_hints(results.initial_results());

        if (hints.empty())
            print_found_deck(results);
        else {
            std::cout << "\nThere is more than one possible deck. "
                      << "Here are some cards that can help.\n\n";
            constexpr auto max_hints = 10ull;
            const auto num_of_hints = std::min(max_hints, hints.size());

            std::sort(hints.begin(), hints.begin() + num_of_hints);

            for (auto i = 0u; i < num_of_hints; ++i)
                print_card(hints[i]);
        }
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
            std::cout << '\n';

            if (results.numb_of_results() == 1)
                std::cout << "There is one matching seed.\n";
            else
                std::cout << "There are " << results.numb_of_results()
                          << " matching seeds.\n";
            print_hints_or_deck(results);
        } else
            std::cout << "Please enter at least one card.\n" << std::endl;
    }
}
