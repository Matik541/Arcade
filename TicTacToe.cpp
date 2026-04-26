#include "TicTacToe.h"
#include "Display.h"
#include <iostream>

TicTacToe::TicTacToe() : Game("Tic-Tac-Toe", "Classic 3x3 PvP game. Get 3 in a row to win!", false) {}

void TicTacToe::play() {
    Display::clearScreen();
    Display::printColored("--- Welcome to Tic-Tac-Toe ---\n", Color::CYAN);
    std::cout << "[Game Logic goes here...]\n";
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
}