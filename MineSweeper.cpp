#include "MineSweeper.h"
#include "Display.h"
#include <iostream>

MineSweeper::MineSweeper() : Game("Mine Sweeper", "Classic grid-based game. Avoid mines and reveal all safe cells!", false) {}

void MineSweeper::play() {
    Display::clearScreen();
    Display::printColored("--- Welcome to Mine Sweeper ---\n", Color::CYAN);
    std::cout << "[Game Logic goes here...]\n";
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}