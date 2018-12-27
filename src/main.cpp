#include <chrono>
#include <iostream>

#include "starter_deck.h"

int main()
{
    std::cout << "Hello, World!\n";

    auto deck = deck_check::starter_deck(0);
    auto count = 0;

    auto start = std::chrono::system_clock::now();

    for (auto i = 0u; i < 10000000u; ++i) {
        deck = deck_check::starter_deck(i);
        if (*(deck.cbegin()) == 9)
            ++count;
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << count << '\n';
    std::cout << elapsed.count() << "ms taken\n";
}
