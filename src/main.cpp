#include <cstdlib>
#include <iostream>

#include "cxxopts.hpp"

#include "options.h"

int main(int argc, char* argv[])
{
    auto options = deck_check::program_options();

    try {
        auto input = options.parse(argc, argv);
        deck_check::execute_according_to_input(input, options);
    } catch (const cxxopts::OptionParseException& e) {
        std::cout << "Error parsing options: " << e.what() << std::endl;
        std::exit(1);
    }
}
