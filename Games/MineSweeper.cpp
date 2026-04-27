#include "MineSweeper.h"
#include "../Display.h"
#include "../Input.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>

MineSweeper::MineSweeper() : Game("MineSweeper", "Find all the mines without triggering them!", true) {
    srand(static_cast<unsigned int>(time(0)));
}

bool MineSweeper::setupOptions() {
    Display::clearScreen();
    Display::printColored("--- MineSweeper SETUP ---\n\n", Color::YELLOW);
    std::cout << "[1] Beginner (9x9, 10 Mines)\n";
    Display::printColored("[2] Intermediate (16x16, 40 Mines) <- DEFAULT & SCORED\n", Color::CYAN);
    std::cout << "[3] Expert (30x16, 99 Mines)\n";
    std::cout << "[4] Custom\n";
    std::cout << "[Q] Quit to Menu\n\n> ";

    while (true) {
        char input = toupper(_getch());
        if (input == 'Q') return false;
        
        // 2 is Default, or if they explicitly press 2
        if (input == '2' || input == '\r' || input == '\n' || input == ' ') {
            width = 16; height = 16; numMines = 40; difficulty = 2; break;
        } else if (input == '1') {
            width = 9; height = 9; numMines = 10; difficulty = 1; break;
        } else if (input == '3') {
            width = 30; height = 16; numMines = 99; difficulty = 3; break;
        } else if (input == '4') {
            difficulty = 4;
            std::cout << "\nWidth (min 5, max 30): "; std::cin >> width;
            std::cout << "Height (min 5, max 20): "; std::cin >> height;
            std::cout << "Mines: "; std::cin >> numMines;
            // Catch edge cases
            if(width < 5) width = 5; if(width > 30) width = 30;
            if(height < 5) height = 5; if(height > 20) height = 20;
            if(numMines >= width * height) numMines = (width * height) - 1;
            break;
        }
    }
    return true;
}

void MineSweeper::resetBoard() {
    board.assign(height, std::vector<int>(width, 0));
    revealed.assign(height, std::vector<bool>(width, false));
    flagged.assign(height, std::vector<bool>(width, false));
    cursorX = 0;
    cursorY = 0;
    firstClick = true;
    srand(0); // testing 
}

void MineSweeper::placeMines(int safeX, int safeY) {
    int minesPlaced = 0;
    while (minesPlaced < numMines) {
        int rX = rand() % width;
        int rY = rand() % height;
        // Don't place a mine on the first clicked cell, or where one already exists
        if (board[rY][rX] != -1 && (rX != safeX || rY != safeY)) {
            board[rY][rX] = -1;
            minesPlaced++;
        }
    }
}

void MineSweeper::calculateNumbers() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (board[y][x] == -1) continue;
            int count = 0;
            // Check all 8 neighbors
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
    Display::printColored("=== MineSweeper ===\n\n", Color::YELLOW);
    
    int flagsLeft = numMines;
    for(auto& row : flagged) for(bool f : row) if(f) flagsLeft--;

    std::cout << "Mines: " << numMines << " | Flags left: " << flagsLeft;
    if (!firstClick && !gameOver) {
        std::cout << " | Time: " << (time(0) - startTime) << "s";
    }
    std::cout << "\n\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Draw Cursor
            if (x == cursorX && y == cursorY && !gameOver) Display::printColored("[", Color::CYAN);
            else std::cout << " ";

            // Draw Cell Contents
            if (revealed[y][x] || gameOver) {
                if (board[y][x] == -1) {
                    Display::printColored(isWin ? "X" : "*", isWin ? Color::GREEN : Color::RED);
                } else if (board[y][x] == 0) {
                    std::cout << ".";
                } else {
                    // Color code numbers
                    std::string colors[] = {Color::RESET, Color::BLUE, Color::GREEN, Color::RED, Color::MAGENTA, Color::CYAN, Color::YELLOW, Color::RESET, Color::RESET};
                    Display::printColored(std::to_string(board[y][x]), colors[board[y][x]]);
                }
            } else if (flagged[y][x]) {
                Display::printColored("F", Color::YELLOW);
            } else {
                std::cout << "#";
            }

            // Draw Cursor End
            if (x == cursorX && y == cursorY && !gameOver) Display::printColored("]", Color::CYAN);
            else std::cout << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n[W/A/S/D] Move | [SPACE] Reveal | [F] Flag | [Q] Quit\n\n";
}

void MineSweeper::revealCell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height || revealed[y][x] || flagged[y][x]) return;
    
    revealed[y][x] = true;

    // Flood fill if it's an empty space
    if (board[y][x] == 0) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                revealCell(x + dx, y + dy);
            }
        }
    }
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

            char input = toupper(_getch());
            if (input == 'Q') return;
            
            // Cursor Movement
            if (input == 'W' && cursorY > 0) cursorY--;
            if (input == 'S' && cursorY < height - 1) cursorY++;
            if (input == 'A' && cursorX > 0) cursorX--;
            if (input == 'D' && cursorX < width - 1) cursorX++;

            // Flagging
            if (input == 'F') {
                if (!revealed[cursorY][cursorX]) {
                    flagged[cursorY][cursorX] = !flagged[cursorY][cursorX];
                }
            }

            // Revealing
            if (input == ' ' || input == '\r' || input == '\n') {
                if (flagged[cursorY][cursorX]) continue; // Protect flagged cells

                if (firstClick) {
                    placeMines(cursorX, cursorY);
                    calculateNumbers();
                    startTime = time(0);
                    firstClick = false;
                }

                if (board[cursorY][cursorX] == -1) { // Hit a mine!
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

        // --- END OF MATCH LOGIC ---
        drawBoard(true, isWin); 

        if (isWin) {
            int timeTaken = time(0) - startTime;
            Display::printColored("YOU WIN! Cleared in " + std::to_string(timeTaken) + " seconds.\n\n", Color::GREEN);
            
            if (difficulty == 2 && db != nullptr) {
                std::cout << "NEW RECORD! Enter your name (no spaces): ";
                std::string playerName;
                std::cin >> playerName;
                db->saveScore(name, playerName, timeTaken);
                Display::printColored("Score Saved!\n\n", Color::GREEN);
                
                while((getchar()) != '\n');
            }
        } else {
            Display::printColored("BOOM! You hit a mine. Game Over.\n\n", Color::RED);
        }

        // Post-Game Menu
        std::cout << "[1] Play Again (Same Settings)\n";
        std::cout << "[2] Change Settings\n";
        std::cout << "[Q] Quit to Main Menu\n\n> ";

        while (true) {
            char choice = toupper(_getch());
            if (choice == '1') { skipSetup = true; break; }
            if (choice == '2') { skipSetup = false; break; }
            if (choice == 'Q') { return; }
        }
    }
}