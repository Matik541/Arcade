#include "TicTacToe.h"
#include "Display.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>

TicTacToe::TicTacToe() : Game("Tic-Tac-Toe", "Classic 3x3 game. Get 3 in a row to win!", false) {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator for the bot
}

void TicTacToe::resetBoard() {
    board.assign(9, ' ');
    currentPlayer = 'X'; // X always starts
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
        std::cout << "\n\nDo you want to play First (X) or Second (O)?\n";
        std::cout << "[1] First (X)\n";
        std::cout << "[2] Second (O)\n";
        std::cout << "[Q] Quit to Menu\n\n> ";

        while (true) {
            char input = toupper(_getch());
            if (input == 'Q') return false;
            if (input == '1') {
                botPiece = 'O';
                break;
            }
            if (input == '2') {
                botPiece = 'X';
                break;
            }
        }
    }
    return true;
}

void TicTacToe::drawBoard() {
    Display::clearScreen();
    Display::printColored("=== TIC-TAC-TOE ===\n\n", Color::YELLOW);

    if (vsBot) std::cout << "Mode: vs BOT (" << botPiece << ")\n";
    else std::cout << "Mode: Player vs Player\n";

    std::cout << "Press [Q] at any time to quit.\n\n";

    for (int i = 0; i < 9; i++) {
        // Print the cell content or the slot number
        if (board[i] == 'X') {
            Display::printColored(" X ", Color::RED);
        }
        else if (board[i] == 'O') {
            Display::printColored(" O ", Color::BLUE);
        }
        else {
            std::cout << " " << (i + 1) << " ";
        }

        // Print grid separators
        if (i % 3 != 2) std::cout << "|";
        else if (i != 8) std::cout << "\n---+---+---\n";
    }
    std::cout << "\n\n";
}

bool TicTacToe::checkWin(char player) {
    // All 8 possible winning lines
    int winLines[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Cols
        {0, 4, 8}, {2, 4, 6}             // Diagonals
    };

    for (int i = 0; i < 8; i++) {
        if (board[winLines[i][0]] == player &&
            board[winLines[i][1]] == player &&
            board[winLines[i][2]] == player) {
            return true;
        }
    }
    return false;
}

bool TicTacToe::checkDraw() {
    for (char c : board) {
        if (c == ' ') return false;
    }
    return true;
}

void TicTacToe::botMove() {
    // Simple bot: Pick a random empty spot
    std::vector<int> emptySpots;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') emptySpots.push_back(i);
    }

    if (!emptySpots.empty()) {
        int randomChoice = emptySpots[rand() % emptySpots.size()];
        board[randomChoice] = botPiece;
    }
}

void TicTacToe::play() {
    if (!setupOptions()) return; // Exit if player pressed Q during setup

    resetBoard();
    bool gameRunning = true;

    while (gameRunning) {
        drawBoard();

        // Check if it's the bot's turn
        if (vsBot && currentPlayer == botPiece) {
            Display::printColored("Bot is thinking...\n", Color::CYAN);
            // Simulate a tiny delay for realism (optional, requires <thread> and <chrono>)
            botMove();
        }
        else {
            std::cout << "Player " << currentPlayer << ", select a spot (1-9): ";

            char input = toupper(_getch());
            if (input == 'Q') return; // Instantly quit to menu

            int move = input - '1'; // Convert char '1'-'9' to int 0-8

            // Validate move
            if (move < 0 || move > 8 || board[move] != ' ') {
                continue; // Ignore invalid input and refresh loop
            }

            board[move] = currentPlayer;
        }

        // Check for win or draw
        if (checkWin(currentPlayer)) {
            drawBoard();
            Display::printColored("Player " + std::string(1, currentPlayer) + " WINS!\n\n", Color::GREEN);
            std::cout << "Press any key to return to menu...";
            _getch();
            gameRunning = false;
        }
        else if (checkDraw()) {
            drawBoard();
            Display::printColored("It's a DRAW!\n\n", Color::YELLOW);
            std::cout << "Press any key to return to menu...";
            _getch();
            gameRunning = false;
        }
        else {
            // Swap turns
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
}