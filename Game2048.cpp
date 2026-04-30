#include "Game2048.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

Game2048::Game2048() : Game("2048", "Combine tiles to reach 2048! (Higher is better)", true, true) {
    srand(static_cast<unsigned int>(time(0)));
}

bool Game2048::setupOptions() {
    int selected = 1; 
    std::string options[] = {
        "Small (3x3)",
        "Classic (4x4) <- DEFAULT & SCORED",
        "Large (5x5)",
        "Toggle Endless Mode (Current: OFF)"
    };

    endlessMode = false;

    while (true) {
        options[3] = "Toggle Endless Mode (Current: " + std::string(endlessMode ? "ON)" : "OFF)");

        Display::clearScreen();
        Display::printColored("--- 2048 SETUP ---\n\n", Color::YELLOW);
        
        for (int i = 0; i < 4; i++) {
            if (i == selected) Display::printColored("> " + options[i] + "\n", Color::GREEN);
            else std::cout << "  " << options[i] << "\n";
        }
        
        std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

        int input = getInput(); 
        if (input == 'Q') return false;
        if (input == 'W' && selected > 0) selected--;
        if (input == 'S' && selected < 3) selected++;
        
        if (input == ' ' || input == '\r' || input == '\n') {
            if (selected == 0) { size = 3; break; }
            if (selected == 1) { size = 4; break; }
            if (selected == 2) { size = 5; break; }
            if (selected == 3) { endlessMode = !endlessMode; } // Toggle and stay in menu
        }
    }
    return true;
}

void Game2048::resetBoard() {
    board.assign(size, std::vector<int>(size, 0));
    score = 0;
    hasWon = false;
    spawnTile();
    spawnTile(); // Start with 2 tiles
}

void Game2048::spawnTile() {
    std::vector<std::pair<int, int>> emptySpots;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (board[y][x] == 0) emptySpots.push_back({x, y});
        }
    }

    if (!emptySpots.empty()) {
        int r = rand() % emptySpots.size();
        // 90% chance for a 2, 10% chance for a 4
        board[emptySpots[r].second][emptySpots[r].first] = (rand() % 10 == 0) ? 4 : 2;
    }
}

bool Game2048::slideLine(std::vector<int>& line) {
    std::vector<int> temp;
    for (int val : line) if (val != 0) temp.push_back(val); // Extract non-zeros
    
    std::vector<int> merged;
    for (size_t i = 0; i < temp.size(); i++) {
        // Merge identical neighbors
        if (i + 1 < temp.size() && temp[i] == temp[i+1]) {
            merged.push_back(temp[i] * 2);
            score += temp[i] * 2;
            
            if (temp[i] * 2 == 2048 && !endlessMode) hasWon = true;
            i++; // Skip the merged tile
        } else {
            merged.push_back(temp[i]);
        }
    }
    
    bool changed = false;
    for (int i = 0; i < size; i++) {
        int newVal = (i < merged.size()) ? merged[i] : 0; // Pad right side with zeros
        if (line[i] != newVal) changed = true;
        line[i] = newVal;
    }
    return changed;
}

bool Game2048::move(int direction) {
    bool boardChanged = false;

    for (int i = 0; i < size; i++) {
        std::vector<int> line(size);

        // 1. Extract the line depending on direction
        for (int j = 0; j < size; j++) {
            if (direction == 0) line[j] = board[j][i];               // UP (Col top-to-bottom)
            else if (direction == 1) line[j] = board[size - 1 - j][i]; // DOWN (Col bottom-to-top)
            else if (direction == 2) line[j] = board[i][j];          // LEFT (Row left-to-right)
            else if (direction == 3) line[j] = board[i][size - 1 - j]; // RIGHT (Row right-to-left)
        }

        // 2. Slide and Merge
        if (slideLine(line)) boardChanged = true;

        // 3. Put the line back
        for (int j = 0; j < size; j++) {
            if (direction == 0) board[j][i] = line[j];
            else if (direction == 1) board[size - 1 - j][i] = line[j];
            else if (direction == 2) board[i][j] = line[j];
            else if (direction == 3) board[i][size - 1 - j] = line[j];
        }
    }

    if (boardChanged) spawnTile();
    return boardChanged;
}

bool Game2048::isGameOver() {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (board[y][x] == 0) return false; // Empty spot exists
            // Check right and down for potential merges
            if (x < size - 1 && board[y][x] == board[y][x + 1]) return false;
            if (y < size - 1 && board[y][x] == board[y + 1][x]) return false;
        }
    }
    return true; // Board full and no merges possible
}

void Game2048::drawBoard(bool gameOver) {
    Display::clearScreen();
    Display::printColored("=== 2048 ===\n\n", Color::YELLOW);
    
    std::cout << "Score: " << score << "\n";
    std::cout << "[WASD] Slide Tiles | [Q] Quit\n\n";

    // Dynamic Top Border
    std::cout << "+";
    for(int i=0; i<size; i++) std::cout << " ———— +";
    std::cout << "\n";

    for (int y = 0; y < size; y++) {
        std::cout << "|";
        for (int x = 0; x < size; x++) {
            if (board[y][x] == 0) {
                std::cout << "      |";
            } else {
                // Color mapping for numbers
                int val = board[y][x];
                std::string color = Color::RESET;
                if (val <= 4) color = Color::RESET;
                else if (val <= 16) color = Color::CYAN;
                else if (val <= 64) color = Color::BLUE;
                else if (val <= 256) color = Color::GREEN;
                else if (val <= 1024) color = Color::MAGENTA;
                else color = Color::RED;

                // Center the number in a 6-character space
                std::string numStr = std::to_string(val);
                int padding = (6 - numStr.length()) / 2;
                int extraPad = (6 - numStr.length()) % 2;
                
                std::cout << std::string(padding, ' ');
                Display::printColored(numStr, color);
                std::cout << std::string(padding + extraPad, ' ') << "|";
            }
        }
        
        std::cout << "\n+";
        for(int i=0; i<size; i++) std::cout << " ———— +";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Game2048::play() {
    bool skipSetup = false;

    while (true) {
        if (!skipSetup && !setupOptions()) return; 

        resetBoard();
        bool gameRunning = true;

        while (gameRunning) {
            drawBoard(false);

            int input = getInput();
            if (input == 'Q') {
                std::cout << "\nAre you sure you want to quit to menu? (Y/N): ";
                bool confirmQuit = false;
                while (true) {
                    int ans = getInput();
                    if (ans == 'Y') { confirmQuit = true; break; }
                    if (ans == 'N') { break; }
                }
                if (confirmQuit) return;
                continue; // Skips the rest of the turn and redraws the board
            }
            
            bool validMove = false;
            if (input == 'W') validMove = move(0); 
            if (input == 'S') validMove = move(1); 
            if (input == 'A') validMove = move(2); 
            if (input == 'D') validMove = move(3); 

            if (hasWon && !endlessMode) {
                drawBoard(true);
                Display::printColored("YOU REACHED 2048! YOU WIN!\n\n", Color::GREEN);
                gameRunning = false;
            } 
            else if (isGameOver()) {
                drawBoard(true);
                Display::printColored("NO MORE MOVES! Game Over.\n\n", Color::RED);
                gameRunning = false;
            }
        }

        // Post-game Score Save
        if (size == 4 && db != nullptr) {
            std::cout << "Enter your name for the Scoreboard (no spaces): ";
            std::string playerName;
            std::cin >> playerName;
            // Notice we pass true (higherScoreBetter) here because the Game class manages it
            db->saveScore(name, playerName, score, isHigherScoreBetter()); 
            Display::printColored("Score Saved!\n\n", Color::GREEN);
            while((getchar()) != '\n'); 
        }

// NEW INTERACTIVE MENU
        int endSel = 0;
        std::string endOpts[] = {
            "Play Again (Same Setup)",
            "Change Setup",
            "Quit to Main Menu"
        };

        bool madeChoice = false; 
        
        while (!madeChoice) {
            Display::clearScreen();
            drawBoard(true); 

            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

            int choice = getInput();
            if (choice == 'Q' ) return;
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 2) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { skipSetup = true; madeChoice = true; }
                if (endSel == 1) { skipSetup = false; madeChoice = true; }
                if (endSel == 2) { return; }
            }
        }
    }
}