#include "MineSweeper.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cmath>

MineSweeper::MineSweeper() : Game("MineSweeper", "Find all the mines without triggering them!", true, false) {
    srand(static_cast<unsigned int>(time(0)));
}

bool MineSweeper::setupOptions() {
    int selected = 1; 
    std::string options[] = {
        "Beginner (9x9, 10 Mines)",
        "Intermediate (16x16, 40 Mines) <- DEFAULT & SCORED",
        "Expert (30x16, 99 Mines)",
        "Custom"
    };

    while (true) {
        Display::clearScreen();
        Display::printColored("--- MINESWEEPER SETUP ---\n\n", Color::YELLOW);
        
        for (int i = 0; i < 4; i++) {
            if (i == selected) {
                Display::printColored("> " + options[i] + "\n", Color::GREEN);
            } else {
                std::cout << "  " << options[i] << "\n";
            }
        }
        
        std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

        int input = getInput(); 
        if (input == 'Q') return false;
        if (input == 'W' && selected > 0) selected--;
        if (input == 'S' && selected < 3) selected++;
        
        if (input == ' ' || input == '\r' || input == '\n') {
            if (selected == 0) { width = 9; height = 9; numMines = 10; difficulty = 1; break; }
            if (selected == 1) { width = 16; height = 16; numMines = 40; difficulty = 2; break; }
            if (selected == 2) { width = 30; height = 16; numMines = 99; difficulty = 3; break; }
            if (selected == 3) { 
                difficulty = 4;
                std::cout << "\nWidth (min 5, max 30): "; std::cin >> width;
                std::cout << "Height (min 5, max 20): "; std::cin >> height;
                std::cout << "Mines: "; std::cin >> numMines;
                if(width < 5) width = 5; if(width > 30) width = 30;
                if(height < 5) height = 5; if(height > 20) height = 20;
                if(numMines >= width * height) numMines = (width * height) - 9; // Leave room for safe opening
                break; 
            }
        }
    }
    return true;
}

void MineSweeper::resetBoard() {
    board.assign(height, std::vector<int>(width, 0));
    revealed.assign(height, std::vector<bool>(width, false));
    cellState.assign(height, std::vector<int>(width, 0)); // 0 = Hidden
    cursorX = 0;
    cursorY = 0;
    firstClick = true;
}

void MineSweeper::placeMines(int safeX, int safeY) {
    int minesPlaced = 0;
    int attempts = 0;
    
    while (minesPlaced < numMines) {
        int rX = rand() % width;
        int rY = rand() % height;
        
        // Ensure a 3x3 safe zone around the first click so it opens up nicely
        bool isSafeZone = (std::abs(rX - safeX) <= 1 && std::abs(rY - safeY) <= 1);
        
        // Fallback if the board is incredibly dense with mines and we get stuck
        if (attempts > 1000) isSafeZone = (rX == safeX && rY == safeY);

        if (board[rY][rX] != -1 && !isSafeZone) {
            board[rY][rX] = -1;
            minesPlaced++;
        }
        attempts++;
    }
}

void MineSweeper::calculateNumbers() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (board[y][x] == -1) continue;
            int count = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int ny = y + dy, nx = x + dx;
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width && board[ny][nx] == -1) {
                        count++;
                    }
                }
            }
            board[y][x] = count;
        }
    }
}

void MineSweeper::drawBoard(bool gameOver, bool isWin) {
    Display::clearScreen();
    Display::printColored("=== MINESWEEPER ===\n\n", Color::YELLOW);
    
    int flagsLeft = numMines;
    for(auto& row : cellState) for(int state : row) if(state == 1) flagsLeft--;

    std::cout << "Mines: " << numMines << " | Flags left: " << flagsLeft;
    if (!firstClick && !gameOver) std::cout << " | Time: " << (time(0) - startTime) << "s";
    std::cout << "\n[WASD] Move | [SPACE] Reveal/Chord | [F] Flag/? | [Q] Quit\n\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == cursorX && y == cursorY && !gameOver) Display::printColored("[", Color::CYAN);
            else std::cout << " ";

            if (revealed[y][x] || gameOver) {
                if (board[y][x] == -1) {
                    Display::printColored(isWin ? "X" : "*", isWin ? Color::GREEN : Color::RED);
                } else if (board[y][x] == 0) {
                    std::cout << " ";
                } else {
                    std::string colors[] = {Color::RESET, Color::BLUE, Color::GREEN, Color::RED, Color::MAGENTA, Color::CYAN, Color::YELLOW, Color::RESET, Color::RESET};
                    Display::printColored(std::to_string(board[y][x]), colors[board[y][x]]);
                }
            } else if (cellState[y][x] == 1) { // Flag
                Display::printColored("F", Color::YELLOW);
            } else if (cellState[y][x] == 2) { // Question Mark
                Display::printColored("?", Color::CYAN);
            } else {
                std::cout << "#";
            }

            if (x == cursorX && y == cursorY && !gameOver) Display::printColored("]", Color::CYAN);
            else std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void MineSweeper::revealCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height || revealed[y][x] || cellState[y][x] == 1) return;
    
    revealed[y][x] = true;

    if (board[y][x] == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                revealCell(x + dx, y + dy);
            }
        }
    }
}

// NEW: Opens surrounding cells if flag count matches number
bool MineSweeper::chordCell(int x, int y) {
    int flagCount = 0;
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            int ny = y + dy, nx = x + dx;
            if (ny >= 0 && ny < height && nx >= 0 && nx < width && cellState[ny][nx] == 1) {
                flagCount++;
            }
        }
    }

    // If flags match the number, reveal everything else
    if (flagCount == board[y][x]) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int ny = y + dy, nx = x + dx;
                if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                    if (!revealed[ny][nx] && cellState[ny][nx] != 1) {
                        if (board[ny][nx] == -1) return false; // Hit a mine!
                        revealCell(nx, ny);
                    }
                }
            }
        }
    }
    return true; // Safe
}

bool MineSweeper::checkWin() {
    int unrevealedCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (!revealed[y][x]) unrevealedCount++;
        }
    }
    return unrevealedCount == numMines;
}

void MineSweeper::play() {
    bool skipSetup = false;

    while (true) {
        if (!skipSetup && !setupOptions()) return; 

        resetBoard();
        bool gameRunning = true;
        bool isWin = false;

        while (gameRunning) {
            drawBoard(false, false);

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
            
            if (input == 'W' && cursorY > 0) cursorY--;
            if (input == 'S' && cursorY < height - 1) cursorY++;
            if (input == 'A' && cursorX > 0) cursorX--;
            if (input == 'D' && cursorX < width - 1) cursorX++;

            // Cycle: Hidden (0) -> Flag (1) -> Question (2) -> Hidden (0)
            if (input == 'F' && !revealed[cursorY][cursorX]) {
                cellState[cursorY][cursorX] = (cellState[cursorY][cursorX] + 1) % 3;
            }

            if (input == ' ' || input == '\r' || input == '\n') {
                if (cellState[cursorY][cursorX] == 1) continue; 

                // CHORDING logic
                if (revealed[cursorY][cursorX] && board[cursorY][cursorX] > 0) {
                    if (!chordCell(cursorX, cursorY)) {
                        gameRunning = false;
                        isWin = false;
                    } else if (checkWin()) {
                        gameRunning = false;
                        isWin = true;
                    }
                    continue; 
                }

                if (firstClick) {
                    placeMines(cursorX, cursorY);
                    calculateNumbers();
                    startTime = time(0);
                    firstClick = false;
                }

                if (!revealed[cursorY][cursorX]) {
                    if (board[cursorY][cursorX] == -1) { 
                        gameRunning = false;
                        isWin = false;
                    } else {
                        revealCell(cursorX, cursorY);
                        if (checkWin()) {
                            gameRunning = false;
                            isWin = true;
                        }
                    }
                }
            }
        }

        drawBoard(true, isWin); 

        if (isWin) {
            int timeTaken = time(0) - startTime;
            Display::printColored("YOU WIN! Cleared in " + std::to_string(timeTaken) + " seconds.\n\n", Color::GREEN);
            
            if (difficulty == 2 && db != nullptr) {
                std::cout << "NEW RECORD! Enter your name (no spaces): ";
                std::string playerName;
                std::cin >> playerName;
                db->saveScore(name, playerName, timeTaken, isHigherScoreBetter());
                Display::printColored("Score Saved!\n\n", Color::GREEN);
                while((getchar()) != '\n'); 
            }
        } else {
            Display::printColored("BOOM! You hit a mine. Game Over.\n\n", Color::RED);
        }
// NEW INTERACTIVE MENU
        int endSel = 0;
        std::string endOpts[] = {
            "Play Again (Same Setup)",
            "Change Setup",
            "Quit to Main Menu"
        };

        // Note: We use a boolean flag so we only process the choice once
        bool madeChoice = false; 
        
        while (!madeChoice) {
            Display::clearScreen();
            drawBoard(true, isWin); 

            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select\n> ";

            int choice = getInput();
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