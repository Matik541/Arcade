#include "Arcade.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <iomanip>

// Include all games
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
    // Initialize Database
    db = std::make_unique<Database>("scores.txt");

    // Initialize all games and safely pass the database pointer to all of them
    auto ttt = std::make_shared<TicTacToe>(); 
    auto ms = std::make_shared<MineSweeper>(); 
    auto g2048 = std::make_shared<Game2048>(); 
    auto bj = std::make_shared<Blackjack>(); 
    auto bs = std::make_shared<Battleship>(); 
    auto snake = std::make_shared<Snake>(); 
    auto sol = std::make_shared<Solitaire>(); 
    auto bomber = std::make_shared<Bomberman>(); 
    auto pvz = std::make_shared<CyberDefense>();
    
    ms->setDatabase(db.get());
    g2048->setDatabase(db.get());
    bj->setDatabase(db.get());
    snake->setDatabase(db.get());
    bomber->setDatabase(db.get());

    // Push into unified list
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
    
    // ASCII Banner
    Display::printColored("\n===============================================", Color::YELLOW);
    Display::printColored("\n           T E R M I N A L   A R C A D E       ", Color::CYAN);
    Display::printColored("\n===============================================\n", Color::YELLOW);

    std::cout << "  Select a game to play:\n\n";

    // Draw unified game list
    for (size_t i = 0; i < games.size(); i++) {
        if ((int)i == selected) {
            Display::printColored("  > [ " + games[i]->getName() + " ]\n", Color::GREEN);
        } else {
            std::cout << "      " << games[i]->getName() << "\n";
        }
    }

    std::cout << "\n";
    
    // Exit Button
    if (selected == games.size()) {
        Display::printColored("  > [ Exit Arcade ]\n", Color::RED);
    } else {
        std::cout << "      Exit Arcade\n";
    }

    // Dynamic Info & Scoreboard Box
    Display::printColored("\n-----------------------------------------------\n", Color::YELLOW);
    
    if (selected < games.size()) {
        Display::printColored(" INFO: ", Color::CYAN);
        std::cout << games[selected]->getDescription() << "\n\n";

        Display::printColored(" --- TOP 3 SCORES ---\n", Color::MAGENTA);
        
        // Fetch Top Scores for the hovered game
        auto topScores = db->getTopScores(games[selected]->getName(), games[selected]->isHigherScoreBetter(), 3);
        
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
                
                // Add seconds suffix if lower is better (like Minesweeper times)
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

    Display::printColored("-----------------------------------------------\n", Color::YELLOW);
    std::cout << "[W/S] Navigate | [SPACE/ENTER] Play | [Q] Quit\n> ";
}

void Arcade::run() {
    int selected = 0;
    int totalOptions = games.size() + 1; // +1 for the Exit button at the bottom

    while (true) {
        drawMenu(selected);

        int input = getInput();
        
        if (input == 'Q') break;
        if (input == 'W' && selected > 0) selected--;
        if (input == 'S' && selected < totalOptions - 1) selected++;
        
        if (input == ' ' || input == '\r' || input == '\n') {
            if (selected == games.size()) {
                break; // Exit selected
            } else {
                games[selected]->play();
            }
        }
    }
    
    Display::clearScreen();
    Display::printColored("Thanks for playing! Shutting down...\n", Color::CYAN);
}

// UUDDLRLRBA - add an easter egg if the user inputs this famous Konami Code sequence on the main menu (not in a game) - maybe it unlocks a secret game or just prints a fun message?