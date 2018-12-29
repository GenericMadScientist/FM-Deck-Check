#include <chrono>
#include <iostream>

#include "starter_deck.h"

int main()
{
    std::cout << "Hello, World!\n";

    auto start = std::chrono::system_clock::now();

    auto filter = deck_check::starter_deck_filter(std::vector<int>({123, 123, 137, 158, 161, 167, 178, 192}));
    auto filter_results = filter.matching_decks(0, 134217728);

    auto end = std::chrono::system_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << filter_results.size() << '\n';
    std::cout << elapsed.count() << "ms taken\n";
}
