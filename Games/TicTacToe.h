#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "../Game.h"
#include <vector>

class TicTacToe : public Game {
private:
    std::vector<char> board;
    char currentPlayer;
    bool vsBot;
    char botPiece;
    char playerPiece;
    int botDifficulty; 

    void resetBoard();
    void drawBoard();
    
    bool checkWin(const std::vector<char>& b, char player);
    bool checkDraw(const std::vector<char>& b);

    void botMove();
    int getRandomMove();
    int getWinningOrBlockingMove(char piece);
    int getBestMove();
    int minimax(std::vector<char>& b, int depth, bool isMaximizing);

    bool setupOptions(); 

public:
    TicTacToe();
    void play() override;
};

#endif