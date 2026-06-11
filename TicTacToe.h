#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "Game.h"
#include <vector>

class TicTacToe : public Game {
private:
    std::vector<char> board;
    char currentPlayer;
    bool vsBot;
    char botPiece;
    char playerPiece;
    int botDifficulty; 

    // Resets the board and the active player for a new round.
    void resetBoard();
    // Draws the current board and control hints.
    void drawBoard();
    
    // Checks whether the requested player has a winning line.
    bool checkWin(const std::vector<char>& b, char player);
    // Checks whether the board is full with no empty cells left.
    bool checkDraw(const std::vector<char>& b);

    // Executes one bot move using the selected difficulty.
    void botMove();
    // Returns a random legal move for the bot.
    int getRandomMove();
    // Finds a move that either wins immediately or blocks the opponent.
    int getWinningOrBlockingMove(char piece);
    // Evaluates the board using the minimax search.
    int getBestMove();
    // Recursively scores future positions for the hard bot.
    int minimax(std::vector<char>& b, int depth, bool isMaximizing);

    // Builds the mode, difficulty, and play-order setup menus.
    bool setupOptions(); 

public:
    // Sets up the Tic-Tac-Toe game state and metadata.
    TicTacToe();
    // Runs the full Tic-Tac-Toe match loop.
    void play() override;
};

#endif