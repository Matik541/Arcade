#include "Arcade.h"
#include "Display.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Input.h"

#include "TicTacToe.h"
#include "MineSweeper.h"
#include "Game2048.h"
#include "Blackjack.h"
#include "Battleship.h"
#include "Snake.h"

Arcade::Arcade() {
    running = true;
    selectedIndex = 0; 
    db = std::make_unique<FileDatabase>("scores.txt");

    auto ttt = std::make_shared<TicTacToe>();
    auto ms = std::make_shared<MineSweeper>();
    auto g2048 = std::make_shared<Game2048>();
    auto bj = std::make_shared<Blackjack>();
    auto bs = std::make_shared<Battleship>();
    auto sn = std::make_shared<Snake>();

    ms->setDatabase(db.get());
    g2048->setDatabase(db.get());
    bj->setDatabase(db.get());
    sn->setDatabase(db.get());

    soloGames.push_back(ms);
    soloGames.push_back(g2048);
    soloGames.push_back(bj);
    soloGames.push_back(sn);

    pvpGames.push_back(ttt);
    pvpGames.push_back(bs);
	 //pvpGames.push_back(std::make_shared<NIM>()); 
}

void Arcade::renderFrame() {
    Display::clearScreen();
    Display::printColored("=== ARCADE MACHINE ===\n\n", Color::YELLOW);

    std::shared_ptr<Game> currentGame;
    if (selectedIndex < soloGames.size()) {
        currentGame = soloGames[selectedIndex];
    }
    else {
        currentGame = pvpGames[selectedIndex - soloGames.size()];
    }

    std::cout << std::left << std::setw(40) << "--- GAMES ---" << "--- SCOREBOARD (Top 5) ---\n";

    // 2. Build the Left Menu dynamically
    std::vector<std::string> leftMenu;

    leftMenu.push_back("[ SOLO GAMES ]");
    for (size_t i = 0; i < soloGames.size(); ++i) {
        leftMenu.push_back((selectedIndex == i ? "> " : "  ") + soloGames[i]->getName());
    }

    leftMenu.push_back(""); 
    leftMenu.push_back("[ PvP / PvB ]");
    for (size_t i = 0; i < pvpGames.size(); ++i) {
        int globalIndex = i + soloGames.size();
        leftMenu.push_back((selectedIndex == globalIndex ? "> " : "  ") + pvpGames[i]->getName());
    }

    // 3. Build the Right Scoreboard dynamically
    std::vector<std::string> rightScores;
    if (currentGame->usesScoreboard()) {
        auto scores = db->getTop5(currentGame->getName(), currentGame->isHigherScoreBetter());
        for (int i = 0; i < 5; ++i) {
            if (i < scores.size()) {
                rightScores.push_back(std::to_string(i + 1) + ". " + scores[i].playerName + " - " + std::to_string(scores[i].score));
            }
            else {
                rightScores.push_back(std::to_string(i + 1) + ". ---");
            }
        }
    }
    else {
        rightScores.push_back("No scoreboard");
        rightScores.push_back("for this game mode.");
    }

    int maxLines = std::max(leftMenu.size(), rightScores.size());
    for (int i = 0; i < maxLines; ++i) {
        std::string leftStr = (i < leftMenu.size()) ? leftMenu[i] : "";
        std::string rightStr = (i < rightScores.size()) ? rightScores[i] : "";

        if (leftStr.length() > 0 && leftStr[0] == '>') {
            Display::printColored(leftStr, Color::GREEN); 
        }
        else if (leftStr.find("[") != std::string::npos) {
            Display::printColored(leftStr, Color::CYAN); 
        }
        else {
            std::cout << leftStr;
        }

        int padding = 40 - leftStr.length();
        if (padding > 0) std::cout << std::string(padding, ' ');

        std::cout << rightStr << "\n";
    }

    std::cout << "\n";
    Display::drawLine(80, '=');

    Display::printColored("SELECTED: " + currentGame->getName() + "\n", Color::YELLOW);
    std::cout << currentGame->getDescription() << "\n\n";
    std::cout << "Controls: [W/S] Navigate | [SPACE] Play | [Q] Quit\n";
}

void Arcade::handleInput() {
    int input = getInput();

    int totalGames = soloGames.size() + pvpGames.size();

    if (totalGames == 0) {
        if (input == 'Q') running = false;
        return;
    }

    if (input == 'Q') {
        running = false;
    }
    else if (input == 'W') {
        selectedIndex--;
        if (selectedIndex < 0) {
            selectedIndex = totalGames - 1;
        }
    }
    else if (input == 'S') {
        selectedIndex++;
        if (selectedIndex >= totalGames) {
            selectedIndex = 0;
        }
    }
    else if (input == '\r' || input == '\n' || input == ' ' || input == 'P') {
        if (selectedIndex < soloGames.size()) {
            soloGames[selectedIndex]->play();
        }
        else {
            pvpGames[selectedIndex - soloGames.size()]->play();
        }
    }
}

void Arcade::run() {
    while (running) {
        renderFrame();
        handleInput();
    }
}

// UUDDLRLRBA - add an easter egg if the user inputs this famous Konami Code sequence on the main menu (not in a game) - maybe it unlocks a secret game or just prints a fun message?