#include "TicTacToe.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

TicTacToe::TicTacToe() : Game("Tic-Tac-Toe", "Classic 3x3 game. Get 3 in a row to win!", false) {
    srand(static_cast<unsigned int>(time(0))); 
}

void TicTacToe::resetBoard() {
    board.assign(9, ' ');
    currentPlayer = 'X'; 
}

// 1. Fully Interactive Setup Menus
bool TicTacToe::setupOptions() {
    // --- MODE SELECTION ---
    int sel = 0;
    std::string modes[] = {"Player vs Player", "Player vs BOT"};
    
    while (true) {
        Display::clearScreen();
        Display::printColored("--- TIC-TAC-TOE SETUP ---\n\n", Color::YELLOW);
        std::cout << "Choose Game Mode:\n";
        
        for (int i = 0; i < 2; i++) {
            if (i == sel) Display::printColored("> " + modes[i] + "\n", Color::GREEN);
            else std::cout << "  " + modes[i] << "\n";
        }
        std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

        int input = getInput();
        if (input == 'Q') return false;
        if (input == 'W' && sel > 0) sel--;
        if (input == 'S' && sel < 1) sel++;
        if (input == ' ' || input == '\r' || input == '\n') {
            vsBot = (sel == 1);
            break;
        }
    }

    // --- BOT SETTINGS ---
    if (vsBot) {
        // Difficulty Selection
        sel = 0;
        std::string diffs[] = {"Easy (Random)", "Mid (Will block and win)", "Hard (Unbeatable)"};
        
        while (true) {
            Display::clearScreen();
            Display::printColored("--- TIC-TAC-TOE SETUP ---\n\n", Color::YELLOW);
            std::cout << "Select Bot Difficulty:\n";
            
            for (int i = 0; i < 3; i++) {
                if (i == sel) Display::printColored("> " + diffs[i] + "\n", Color::GREEN);
                else std::cout << "  " + diffs[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

            int input = getInput();
            if (input == 'Q') return false;
            if (input == 'W' && sel > 0) sel--;
            if (input == 'S' && sel < 2) sel++;
            if (input == ' ' || input == '\r' || input == '\n') {
                botDifficulty = sel + 1; // 1=Easy, 2=Mid, 3=Hard
                break;
            }
        }

        // Play Order Selection
        sel = 0;
        std::string orders[] = {"First (X)", "Second (O)"};
        
        while (true) {
            Display::clearScreen();
            Display::printColored("--- TIC-TAC-TOE SETUP ---\n\n", Color::YELLOW);
            std::cout << "Do you want to play First or Second?\n";
            
            for (int i = 0; i < 2; i++) {
                if (i == sel) Display::printColored("> " + orders[i] + "\n", Color::GREEN);
                else std::cout << "  " + orders[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

            int input = getInput();
            if (input == 'Q') return false;
            if (input == 'W' && sel > 0) sel--;
            if (input == 'S' && sel < 1) sel++;
            if (input == ' ' || input == '\r' || input == '\n') {
                if (sel == 0) {
                    botPiece = 'O';
                    playerPiece = 'X';
                } else {
                    botPiece = 'X';
                    playerPiece = 'O';
                }
                break;
            }
        }
    }
    return true;
}

void TicTacToe::drawBoard() {
    Display::clearScreen();
    Display::printColored("=== TIC-TAC-TOE ===\n\n", Color::YELLOW);
    
    if (vsBot) {
        std::string diffLevel = (botDifficulty == 1) ? "Easy" : (botDifficulty == 2) ? "Mid" : "Hard";
        std::cout << "Mode: vs BOT (" << diffLevel << ") Playing as " << botPiece << "\n";
    } else {
        std::cout << "Mode: Player vs Player\n";
    }
    
    std::cout << "Press [Q] at any time to quit.\n\n";

    for (int i = 0; i < 9; i++) {
        if (board[i] == 'X') {
            Display::printColored(" X ", Color::RED);
        } else if (board[i] == 'O') {
            Display::printColored(" O ", Color::BLUE);
        } else {
            std::cout << " " << (i + 1) << " ";
        }

        if (i % 3 != 2) std::cout << "|";
        else if (i != 8) std::cout << "\n — + — + — \n";
    }
    std::cout << "\n\n";
}

bool TicTacToe::checkWin(const std::vector<char>& b, char player) {
    int winLines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}            
    };

    for (int i = 0; i < 8; i++) {
        if (b[winLines[i][0]] == player && b[winLines[i][1]] == player && b[winLines[i][2]] == player) {
            return true;
        }
    }
    return false;
}

bool TicTacToe::checkDraw(const std::vector<char>& b) {
    for (char c : b) {
        if (c == ' ') return false;
    }
    return true;
}

// --- AI LOGIC (Unchanged) ---
int TicTacToe::getRandomMove() {
    std::vector<int> emptySpots;
    for (int i = 0; i < 9; i++) if (board[i] == ' ') emptySpots.push_back(i);
    if (!emptySpots.empty()) return emptySpots[rand() % emptySpots.size()];
    return -1;
}

int TicTacToe::getWinningOrBlockingMove(char piece) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = piece;
            bool win = checkWin(board, piece);
            board[i] = ' '; 
            if (win) return i;
        }
    }
    return -1;
}

int TicTacToe::minimax(std::vector<char>& b, int depth, bool isMaximizing) {
    if (checkWin(b, botPiece)) return 10 - depth; 
    if (checkWin(b, playerPiece)) return -10 + depth; 
    if (checkDraw(b)) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 9; i++) {
            if (b[i] == ' ') {
                b[i] = botPiece;
                bestScore = std::max(minimax(b, depth + 1, false), bestScore);
                b[i] = ' ';
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 9; i++) {
            if (b[i] == ' ') {
                b[i] = playerPiece;
                bestScore = std::min(minimax(b, depth + 1, true), bestScore);
                b[i] = ' ';
            }
        }
        return bestScore;
    }
}

int TicTacToe::getBestMove() {
    int bestScore = -1000;
    int move = -1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            board[i] = botPiece;
            int score = minimax(board, 0, false);
            board[i] = ' ';
            if (score > bestScore) {
                bestScore = score;
                move = i;
            }
        }
    }
    return move;
}

void TicTacToe::botMove() {
    int chosenMove = -1;
    if (botDifficulty == 1) {
        chosenMove = getRandomMove();
    } else if (botDifficulty == 2) {
        chosenMove = getWinningOrBlockingMove(botPiece);     
        if (chosenMove == -1) chosenMove = getWinningOrBlockingMove(playerPiece); 
        if (chosenMove == -1) chosenMove = getRandomMove();                       
    } else if (botDifficulty == 3) { 
        int emptyCount = 0;
        for(char c : board) if(c == ' ') emptyCount++;
        if (emptyCount == 9) chosenMove = 0; 
        else chosenMove = getBestMove();
    }
    if (chosenMove != -1) board[chosenMove] = botPiece;
}

// --- GAME LOOP ---
void TicTacToe::play() {
    bool skipSetup = false;

    while (true) {
        if (!skipSetup && !setupOptions()) return; 

        resetBoard();
        bool gameRunning = true;

        while (gameRunning) {
            drawBoard();

            if (vsBot && currentPlayer == botPiece) {
                Display::printColored("Bot is making a move...\n", Color::CYAN);
                botMove();
            } else {
                std::cout << "Player " << currentPlayer << ", select a spot (1-9): ";
                
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
                    continue; 
                }

                int move = input - '1'; 
                if (move < 0 || move > 8 || board[move] != ' ') {
                    continue; 
                }
                board[move] = currentPlayer;
            }

            if (checkWin(board, currentPlayer) || checkDraw(board)) {
                gameRunning = false;
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }

        // 2. Interactive Post-Game Menu
        int endSel = 0;
        std::string endOpts[] = {
            "Play Again (Same Settings)",
            "Change Settings",
            "Quit to Main Menu"
        };

        while (true) {
            drawBoard(); // Redraw board so it doesn't flicker away
            
            // Re-print the win/draw message
            if (checkWin(board, currentPlayer)) {
                Display::printColored("Player " + std::string(1, currentPlayer) + " WINS!\n\n", Color::GREEN);
            } else {
                Display::printColored("It's a DRAW!\n\n", Color::YELLOW);
            }

            // Print Interactive Menu
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select\n> ";

            int choice = getInput();
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 2) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { skipSetup = true; break; }
                if (endSel == 1) { skipSetup = false; break; }
                if (endSel == 2) { return; }
            }
        }
    }
}