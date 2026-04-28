#include "TicTacToe.h"
#include "../Display.h"
#include "../Input.h"
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

bool TicTacToe::setupOptions() {
    Display::clearScreen();
    Display::printColored("--- TIC-TAC-TOE SETUP ---\n\n", Color::YELLOW);
    std::cout << "Choose Game Mode:\n";
    std::cout << "[1] Player vs Player\n";
    std::cout << "[2] Player vs BOT\n";
    std::cout << "[Q] Quit to Menu\n\n> ";

    while (true) {
        char input = toupper(_getch());
        if (input == 'Q') return false;
        if (input == '1') {
            vsBot = false;
            break;
        }
        if (input == '2') {
            vsBot = true;
            break;
        }
    }

    if (vsBot) {
        std::cout << "\n\nSelect Bot Difficulty:\n";
        std::cout << "[1] Easy (Random)\n";
        std::cout << "[2] Mid (Will block and win)\n";
        std::cout << "[3] Hard (Unbeatable)\n";
        std::cout << "[Q] Quit to Menu\n\n> ";

        while (true) {
            char input = toupper(_getch());
            if (input == 'Q') return false;
            if (input >= '1' && input <= '3') {
                botDifficulty = input - '0';
                break;
            }
        }

        std::cout << "\n\nDo you want to play First (X) or Second (O)?\n";
        std::cout << "[1] First (X)\n";
        std::cout << "[2] Second (O)\n";
        std::cout << "[Q] Quit to Menu\n\n> ";

        while (true) {
            char input = toupper(_getch());
            if (input == 'Q') return false;
            if (input == '1') {
                botPiece = 'O';
                playerPiece = 'X';
                break;
            }
            if (input == '2') {
                botPiece = 'X';
                playerPiece = 'O';
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

// --- AI LOGIC ---

int TicTacToe::getRandomMove() {
    std::vector<int> emptySpots;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') emptySpots.push_back(i);
    }
    if (!emptySpots.empty()) {
        return emptySpots[rand() % emptySpots.size()];
    }
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
                int score = minimax(b, depth + 1, false);
                b[i] = ' ';
                bestScore = std::max(score, bestScore);
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 9; i++) {
            if (b[i] == ' ') {
                b[i] = playerPiece;
                int score = minimax(b, depth + 1, true);
                b[i] = ' ';
                bestScore = std::min(score, bestScore);
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
    } 
    else if (botDifficulty == 2) {
        chosenMove = getWinningOrBlockingMove(botPiece); 
        if (chosenMove == -1) {
            chosenMove = getWinningOrBlockingMove(playerPiece);
        }
        if (chosenMove == -1) {
            chosenMove = getRandomMove();
        }
    } 
    else if (botDifficulty == 3) { 
        int emptyCount = 0;
        for(char c : board) if(c == ' ') emptyCount++;
        
        if (emptyCount == 9) chosenMove = 0;
        else chosenMove = getBestMove();
    }

    if (chosenMove != -1) {
        board[chosenMove] = botPiece;
    }
}

// --- GAME LOOP ---

void TicTacToe::play() {
    bool skipSetup = false;

    // The Master Game Loop
    while (true) {
        
        // 1. Setup Phase
        if (!skipSetup) {
            if (!setupOptions()) return; // If they press Q during setup, return to Main Menu
        }
        
        resetBoard();
        bool gameRunning = true;

        // 2. The Match Loop
        while (gameRunning) {
            drawBoard();

            if (vsBot && currentPlayer == botPiece) {
                Display::printColored("Bot is making a move...\n", Color::CYAN);
                botMove();
            } else {
                std::cout << "Player " << currentPlayer << ", select a spot (1-9): ";
                
                char input = toupper(_getch());
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

                int move = input - '1'; 

                if (move < 0 || move > 8 || board[move] != ' ') {
                    continue; 
                }

                board[move] = currentPlayer;
            }

            // 3. Win/Draw Checking
            if (checkWin(board, currentPlayer)) {
                drawBoard();
                Display::printColored("Player " + std::string(1, currentPlayer) + " WINS!\n\n", Color::GREEN);
                gameRunning = false;
            } 
            else if (checkDraw(board)) {
                drawBoard();
                Display::printColored("It's a DRAW!\n\n", Color::YELLOW);
                gameRunning = false;
            } 
            else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }

        // 4. Post-Game Menu
        std::cout << "[1] Play Again (Same Settings)\n";
        if (vsBot) {
            std::cout << "[2] Change Difficulty / Settings\n";
        } else {
            std::cout << "[2] Change Settings\n";
        }
        std::cout << "[Q] Quit to Main Menu\n\n> ";

        bool validChoice = false;
        while (!validChoice) {
            char input = toupper(_getch());
            if (input == '1') {
                skipSetup = true;  // Bypass setupOptions() on the next loop iteration
                validChoice = true;
            } else if (input == '2') {
                skipSetup = false; // Run setupOptions() on the next loop iteration
                validChoice = true;
            } else if (input == 'Q') {
                return;            // Exit the play() method entirely, returning to Arcade Menu
            }
        }
    }
}