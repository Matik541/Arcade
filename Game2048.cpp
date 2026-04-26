#include "Game2048.h"
#include "Display.h"
#include <iostream>

Game2048::Game2048() : Game("2048", "Classic sliding puzzle game. Combine tiles to reach 2048!", false) {}

void Game2048::play() {
    Display::clearScreen();
    Display::printColored("--- Welcome to 2048 ---\n", Color::CYAN);
    std::cout << "[Game Logic goes here...]\n";
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}