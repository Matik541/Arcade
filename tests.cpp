#include <iostream>
#include <cassert>
#include <vector>
#include <cstdio>
#include "TicTacToe.h"
#include "Database.h"
#include "Blackjack.h"
#include "Game2048.h"
#include "MineSweeper.h"
#include "CyberDefense.h"
#include "Battleship.h"

/**
 * @file tests.cpp
 * @brief Testy jednostkowe logiki gier i mechanizmów bazy danych.
 * @details Zawiera asercje weryfikujące poprawność checkWin, getHandValue, slideLine, canPlaceShip oraz sortowanie wyników.
 */

/**
 * @brief Uruchamia testy jednostkowe dla gier w Arcade.
 * @details Testuje m.in. funkcje checkWin, getHandValue, slideLine, hasTowerAt, isEnemyInLane, canPlaceShip.
 */
void runArcadeTests() {
    // 1. Testy logiki TicTacToe
    std::cout << "Running TicTacToe logic tests...\n";
    TicTacToe game;
    
    std::vector<char> boardXWin = {
        'X', 'X', 'X',
        'O', ' ', 'O',
        ' ', ' ', ' '
    };
    
    assert(game.checkWin(boardXWin, 'X') == true);
    assert(game.checkWin(boardXWin, 'O') == false);
    
    // 2. Testy logiki Database
    std::cout << "Running Database logic tests...\n";
    Database& db = Database::getInstance();
    
    std::string testFile = "test_scores_temp.txt";
    std::remove(testFile.c_str()); // Upewnienie się, że plik testowy nie istnieje
    db.setFilenameForTest(testFile);
    
    // Zapisanie testowych rekordów
    db.saveScore("TestGame", "PlayerA", 10, true);
    db.saveScore("TestGame", "PlayerB", 30, true);
    db.saveScore("TestGame", "PlayerC", 20, true);
    
    // Test: wyższe wyniki są lepsze (higherIsBetter = true) -> sortowanie malejąco
    {
        auto top = db.getTopScores("TestGame", true, 3);
        assert(top.size() == 3);
        assert(top[0].playerName == "PlayerB" && top[0].score == 30);
        assert(top[1].playerName == "PlayerC" && top[1].score == 20);
        assert(top[2].playerName == "PlayerA" && top[2].score == 10);
    }
    
    // Test: niższe wyniki są lepsze (higherIsBetter = false) -> sortowanie rosnąco
    {
        auto top = db.getTopScores("TestGame", false, 3);
        assert(top.size() == 3);
        assert(top[0].playerName == "PlayerA" && top[0].score == 10);
        assert(top[1].playerName == "PlayerC" && top[1].score == 20);
        assert(top[2].playerName == "PlayerB" && top[2].score == 30);
    }
    
    // Test limitowania wyników do Top 2
    {
        auto top = db.getTopScores("TestGame", true, 2);
        assert(top.size() == 2);
        assert(top[0].playerName == "PlayerB" && top[0].score == 30);
        assert(top[1].playerName == "PlayerC" && top[1].score == 20);
    }
    
    // Przywrócenie pierwotnego stanu i usunięcie pliku tymczasowego
    db.setFilenameForTest("scores.txt");
    std::remove(testFile.c_str());

    // 3. Testy logiki Blackjack
    std::cout << "Running Blackjack logic tests...\n";
    Blackjack bj;
    // Test sumowania kart i optymalizacji wartości Asów
    assert(bj.getHandValue({{ "2", "♥", 2 }, { "5", "♦", 5 }}) == 7);
    assert(bj.getHandValue({{ "10", "♣", 10 }, { "J", "♠", 10 }}) == 20);
    assert(bj.getHandValue({{ "A", "♠", 11 }, { "9", "♥", 9 }}) == 20);
    assert(bj.getHandValue({{ "A", "♠", 11 }, { "9", "♥", 9 }, { "5", "♣", 5 }}) == 15);
    assert(bj.getHandValue({{ "A", "♥", 11 }, { "A", "♦", 11 }}) == 12);
    assert(bj.getHandValue({{ "A", "♥", 11 }, { "A", "♦", 11 }, { "J", "♠", 10 }}) == 12);

    // 4. Testy logiki Game2048
    std::cout << "Running Game2048 logic tests...\n";
    Game2048 g2048;
    g2048.size = 4;
    
    std::vector<int> line1 = {2, 2, 0, 0};
    assert(g2048.slideLine(line1) == true);
    assert(line1 == std::vector<int>({4, 0, 0, 0}));
    
    std::vector<int> line2 = {0, 2, 0, 2};
    assert(g2048.slideLine(line2) == true);
    assert(line2 == std::vector<int>({4, 0, 0, 0}));
    
    std::vector<int> line3 = {2, 2, 2, 2};
    assert(g2048.slideLine(line3) == true);
    assert(line3 == std::vector<int>({4, 4, 0, 0}));
    
    std::vector<int> line4 = {4, 8, 16, 32};
    assert(g2048.slideLine(line4) == false);
    assert(line4 == std::vector<int>({4, 8, 16, 32}));

    // 5. Testy logiki MineSweeper
    std::cout << "Running MineSweeper logic tests...\n";
    MineSweeper ms;
    ms.width = 3;
    ms.height = 3;
    ms.numMines = 1;
    ms.board = {
        { 0,  1, -1},
        { 0,  1,  1},
        { 0,  0,  0}
    };
    ms.revealed = {
        { false, false, false },
        { false, false, false },
        { false, false, false }
    };
    assert(ms.checkWin() == false);
    ms.revealed = {
        { true,  true,  false },
        { true,  true,  true  },
        { true,  true,  true  }
    };
    assert(ms.checkWin() == true);
    ms.revealed[0][0] = false;
    assert(ms.checkWin() == false);

    // 6. Testy logiki CyberDefense
    std::cout << "Running CyberDefense logic tests...\n";
    CyberDefense cd;
    assert(cd.hasTowerAt(2, 3) == false);
    assert(cd.getTowerIndex(2, 3) == -1);
    
    cd.towers.push_back({ 2, 3, 1, 100, 0 });
    assert(cd.hasTowerAt(2, 3) == true);
    assert(cd.getTowerIndex(2, 3) == 0);
    assert(cd.hasTowerAt(4, 4) == false);
    
    assert(cd.isEnemyInLane(2, 0) == false);
    cd.enemies.push_back({ 8, 2, 50, 50, 'V', 0, 1 });
    assert(cd.isEnemyInLane(2, 0) == true);
    assert(cd.isEnemyInLane(2, 10) == false);
    assert(cd.isEnemyInLane(3, 0) == false);

    // 7. Testy logiki Battleship
    std::cout << "Running Battleship logic tests...\n";
    Battleship bs;
    bs.playerBoard.assign(10, std::vector<int>(10, -1));
    
    assert(bs.canPlaceShip(bs.playerBoard, 7, 2, 3, true) == true);
    assert(bs.canPlaceShip(bs.playerBoard, 8, 2, 3, true) == false);
    assert(bs.canPlaceShip(bs.playerBoard, 2, 8, 3, false) == false);
    
    bs.playerBoard[2][2] = 1;
    bs.playerBoard[2][3] = 1;
    bs.playerBoard[2][4] = 1;
    
    assert(bs.canPlaceShip(bs.playerBoard, 3, 1, 3, false) == false);
    assert(bs.canPlaceShip(bs.playerBoard, 1, 1, 3, false) == true);
    
    std::vector<ShipData> fleet = {
        {"Cruiser", 3, false, 0, 0, true},
        {"Submarine", 2, true, 0, 0, true}
    };
    assert(bs.checkWin(fleet) == false);
    fleet[0].sunk = true;
    assert(bs.checkWin(fleet) == true);

    std::cout << "[PASSED] All tests passed successfully.\n";
}