#include "Arcade.h"
#include "Display.h"
#include "TicTacToe.h"
#include "MineSweeper.h"
#include "Game2048.h"
#include <iostream>
#include <iomanip>
#include <conio.h>

// Helper stub classes for demonstration (replace with actual includes above)
//class MineSweeper : public Game { public: MineSweeper() : Game("MineSweeper", "Find mines without exploding.", true) {} void play() override {} };
//class Game2048 : public Game { public: Game2048() : Game("2048", "Combine tiles to reach 2048.", true) {} void play() override {} };
//class Nim : public Game { public: Nim() : Game("NIM", "Math strategy game.", false) {} void play() override {} };

Arcade::Arcade() {
    running = true;
    selectedGroup = 0;
    selectedGameIndex = 0;
    db = std::make_unique<MockDatabase>();

    // Load Games
    soloGames.push_back(std::make_shared<MineSweeper>());
    soloGames.push_back(std::make_shared<Game2048>());

    pvpGames.push_back(std::make_shared<TicTacToe>());
    //pvpGames.push_back(std::make_shared<Nim>());
}

void Arcade::renderFrame() {
    Display::clearScreen();
    Display::printColored("=== ARCADE MACHINE ===\n\n", Color::YELLOW);

    auto& currentList = (selectedGroup == 0) ? soloGames : pvpGames;
    auto currentGame = currentList[selectedGameIndex];

    // --- RENDER TOP SECTION (Split Screen) ---
    // Left side: Menu | Right side: Scoreboard

    std::cout << std::left << std::setw(40) << "--- GAMES ---" << "--- SCOREBOARD (Top 5) ---\n";

    for (int i = 0; i < 5; ++i) {
        // Left Column (Menu)
        std::string leftStr = "";
        if (i == 0) leftStr = (selectedGroup == 0 ? "> SOLO" : "  SOLO");
        else if (i == 1) leftStr = (selectedGroup == 0 ? "    " + soloGames[0]->getName() : "");
        else if (i == 2) leftStr = (selectedGroup == 1 ? "> PvP / PvB" : "  PvP / PvB");
        else if (i == 3) leftStr = (selectedGroup == 1 ? "    " + pvpGames[0]->getName() : "");

        // Emphasize selected group
        if (leftStr.length() > 0 && leftStr[0] == '>') {
            Display::printColored(leftStr, Color::GREEN);
        }
        else {
            std::cout << leftStr;
        }

        // Pad out the left side to 40 characters safely
        int padding = 40 - leftStr.length();
        if (padding > 0) {
            std::cout << std::string(padding, ' ');
        }

        // Right Column (Scores)
        if (currentGame->usesScoreboard()) {
            auto scores = db->getTop5(currentGame->getName());
            if (i < scores.size()) {
                std::cout << (i + 1) << ". " << scores[i].playerName << " - " << scores[i].score;
            }
            else {
                std::cout << (i + 1) << ". ---";
            }
        }
        else {
            if (i == 2) std::cout << "(No scoreboard for this game)";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
    Display::drawLine(80, '=');

    // --- RENDER BOTTOM SECTION (Description) ---
    Display::printColored("SELECTED: " + currentGame->getName() + "\n", Color::CYAN);
    std::cout << currentGame->getDescription() << "\n\n";
    std::cout << "Controls: [S] Switch Group | [W/A/S/D] Navigate Games | [ENTER] Play | [Q] Quit\n";
}

void Arcade::handleInput() {
    // _getch() instantly captures a single keystroke without needing Enter
    char input = _getch();
    input = toupper(input);

    auto& currentList = (selectedGroup == 0) ? soloGames : pvpGames;

    if (input == 'Q') running = false;
    else if (input == 'S') {
        selectedGroup = (selectedGroup == 0) ? 1 : 0;
        selectedGameIndex = 0;
    }
    else if (input == 'W' && selectedGameIndex > 0) selectedGameIndex--;
    else if (input == 'S' && selectedGameIndex < currentList.size() - 1) selectedGameIndex++;
    // Enter key registers as '\r' (Carriage Return) when using _getch()
    else if (input == '\r' || input == 'P') {
        currentList[selectedGameIndex]->play();
    }
}

void Arcade::run() {
    while (running) {
        renderFrame();
        handleInput();
    }
}


/*
void SomeRealTimeGame::play() {
    bool gameRunning = true;
    while (gameRunning) {

        // 1. Check if the player pressed a key instantly
        if (_kbhit()) {
            char key = _getch(); // Grab that key
            // ... Update player direction/action
        }

        // 2. Move the enemies, update the map, etc.

        // 3. Render the screen

        // 4. Add a tiny sleep delay so the game doesn't run at lightspeed
    }
}
*/