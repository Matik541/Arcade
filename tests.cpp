#include <iostream>
#include <cassert>
#include <vector>
#include "TicTacToe.h"

/**
 * @file tests.cpp
 * @brief Testy jednostkowe logiki gier arcade.
 * @details Zawiera asercje weryfikujące poprawność algorytmów wykrywania wygranej.
 */

/**
 * @brief Uruchamia testy jednostkowe dla gier w Arcade.
 * @details Testuje m.in. funkcję checkWin z klasy TicTacToe.
 */
void runArcadeTests() {
    std::cout << "Running TicTacToe logic tests...\n";
    TicTacToe game;
    
    std::vector<char> boardXWin = {
        'X', 'X', 'X',
        'O', ' ', 'O',
        ' ', ' ', ' '
    };
    
    assert(game.checkWin(boardXWin, 'X') == true);
    assert(game.checkWin(boardXWin, 'O') == false);
    
    std::cout << "[PASSED] All tests passed successfully.\n";
}