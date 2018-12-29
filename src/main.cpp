#include <chrono>
#include <iostream>

#include "cxxopts.hpp"

#include "starter_deck.h"

int main(int argc, char* argv[])
{
    cxxopts::Options options("FM Deck Check", "Checks FM decks.");

    options.add_options()
        ("h,help", "Print help and exit.")
        ;

    auto result = options.parse(argc, argv);
    std::cout << result["h"].as<bool>() << '\n';
}
