#include "Arcade.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <iomanip>

#include "TicTacToe.h"
#include "MineSweeper.h"
#include "Game2048.h"
#include "Blackjack.h"
#include "Battleship.h"
#include "Snake.h"
#include "Solitaire.h"
#include "Bomberman.h"
#include "CyberDefense.h"

Arcade::Arcade() {
    auto ttt = std::make_shared<TicTacToe>(); 
    auto ms = std::make_shared<MineSweeper>(); 
    auto g2048 = std::make_shared<Game2048>(); 
    auto bj = std::make_shared<Blackjack>(); 
    auto bs = std::make_shared<Battleship>(); 
    auto snake = std::make_shared<Snake>(); 
    auto sol = std::make_shared<Solitaire>(); 
    auto bomber = std::make_shared<Bomberman>(); 
    auto pvz = std::make_shared<CyberDefense>();
    
    ms->setDatabase(&Database::getInstance());
    g2048->setDatabase(&Database::getInstance());
    bj->setDatabase(&Database::getInstance());
    snake->setDatabase(&Database::getInstance());
    bomber->setDatabase(&Database::getInstance());

    games.push_back(ttt);
    games.push_back(ms);
    games.push_back(g2048);
    games.push_back(bj);
    games.push_back(bs);
    games.push_back(snake);
    games.push_back(sol);
    games.push_back(bomber);
    games.push_back(pvz);
}

void Arcade::drawMenu(int selected) {
    Display::clearScreen();

    std::string colors[] = {Color::RED, Color::YELLOW, Color::GREEN, Color::CYAN, Color::BLUE, Color::MAGENTA};
    std::string cBorder = Color::YELLOW;
    std::string cTitle = Color::CYAN;
    std::string cSelect = Color::GREEN;
    std::string cScore = Color::MAGENTA;
    std::string cExit = Color::RED;

    if (rainbowMode) {
        cBorder = colors[rand() % 6];
        cTitle = colors[rand() % 6];
        cSelect = colors[rand() % 6];
    }
    
    Display::printColored("\n===============================================", cBorder);
    Display::printColored("\n           T E R M I N A L   A R C A D E       ", cTitle);
    Display::printColored("\n===============================================\n", cBorder);

    std::cout << "  Select a game to play:\n\n";

    for (size_t i = 0; i < games.size(); i++) {
        if ((int)i == selected) {
            Display::printColored("  > [ " + games[i]->getName() + " ]\n", cSelect);
        } else {
            std::cout << "      " << games[i]->getName() << "\n";
        }
    }

    std::cout << "\n";
    
    if (selected == games.size()) {
        Display::printColored("  > [ Exit Arcade ]\n", cExit);
    } else {
        std::cout << "      Exit Arcade\n";
    }

    Display::printColored("\n-----------------------------------------------\n", cBorder);
    
    if (selected < games.size()) {
        Display::printColored(" INFO: ", cTitle);
        std::cout << games[selected]->getDescription() << "\n\n";

        Display::printColored(" --- TOP 3 SCORES ---\n", cScore);
        
        auto topScores = Database::getInstance().getTopScores(games[selected]->getName(), games[selected]->isHigherScoreBetter(), 3);
        
        if (topScores.empty()) {
            std::cout << "  No records yet. Be the first!\n\n\n";
        } else {
            for (size_t i = 0; i < 3; i++) {
                std::cout << "  " << (i + 1) << ". " << std::left;
                if (i < topScores.size()) {
                    std::cout << std::setw(15) << topScores[i].playerName 
                              << " - " << topScores[i].score;
                } else {
                    std::cout << std::setw(15) << "---" << " - " << "---";
                }
                
                if (!games[selected]->isHigherScoreBetter()) {
                    std::cout << " sec";
                }
                std::cout << "\n";
            }
        }
    } else {
        Display::printColored(" INFO: ", Color::RED);
        std::cout << "Close the arcade and return to desktop.\n\n\n";
    }

    Display::printColored("-----------------------------------------------\n", cBorder);
    std::cout << "[W/S] Navigate | [SPACE/ENTER] Play | [Q] Quit\n> ";
}

void Arcade::run() {
    int selected = 0;
    int totalOptions = games.size() + 1;

    while (true) {
        drawMenu(selected);

        int input = getInput();

        if (input >= 'A' && input <= 'Z') {
            inputHistory += (char)input;
            if (inputHistory.length() > 10) inputHistory.erase(0, 1);
            if (inputHistory == "WWSSADADBA") {
                rainbowMode = !rainbowMode; 
                inputHistory = "";          
            }
        }
        
        if (input == 'Q') break;
        if (input == 'W' && selected > 0) selected--;
        if (input == 'S' && selected < totalOptions - 1) selected++;
        
        if (input == ' ' || input == '\r' || input == '\n') {
            if (selected == games.size()) {
                break;
            } else {
                games[selected]->play();
            }
        }
    }
    
    Display::clearScreen();
    Display::printColored("Thanks for playing! Shutting down...\n", Color::CYAN);
}