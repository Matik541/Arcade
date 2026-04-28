#include "Battleship.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Battleship::Battleship() : Game("Battleship", "Sink the enemy fleet before they sink yours!", false, false) {
    srand(static_cast<unsigned int>(time(0)));
}

bool Battleship::setupOptions() {
    int sel = 0;
    std::string diffs[] = {
        "Easy (Random Firing)",
        "Mid (Hunts adjacent cells after a hit)",
        "Hard (Math Checkerboard + Smart Hunting)"
    };

    while (true) {
        Display::clearScreen();
        Display::printColored("--- BATTLESHIP SETUP ---\n\n", Color::YELLOW);
        std::cout << "Select Enemy Intelligence:\n";
        for (int i = 0; i < 3; i++) {
            if (i == sel) Display::printColored("> " + diffs[i] + "\n", Color::GREEN);
            else std::cout << "  " + diffs[i] << "\n";
        }
        std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select | [Q] Quit\n> ";

        int input = getInput();
        if (input == 'Q') return false;
        if (input == 'W' && sel > 0) sel--;
        if (input == 'S' && sel < 2) sel++;
        if (input == ' ' || input == '\r' || input == '\n') {
            botDifficulty = sel + 1;
            break;
        }
    }

    sel = 0;
    std::string placeOpts[] = { "Manual Placement (Interactive)", "Auto Placement (Fast)" };

    while (true) {
        Display::clearScreen();
        Display::printColored("--- BATTLESHIP SETUP ---\n\n", Color::YELLOW);
        std::cout << "Fleet Placement Method:\n";
        for (int i = 0; i < 2; i++) {
            if (i == sel) Display::printColored("> " + placeOpts[i] + "\n", Color::GREEN);
            else std::cout << "  " + placeOpts[i] << "\n";
        }
        std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select | [Q] Quit\n> ";

        int input = getInput();
        if (input == 'Q') return false;
        if (input == 'W' && sel > 0) sel--;
        if (input == 'S' && sel < 1) sel++;
        if (input == ' ' || input == '\r' || input == '\n') {
            manualPlacement = (sel == 0);
            break;
        }
    }
    return true;
}

bool Battleship::canPlaceShip(const std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal) {
    if (horizontal) {
        if (x + length > 10) return false;
        for (int i = 0; i < length; i++) if (board[y][x + i] != 0) return false;
    }
    else {
        if (y + length > 10) return false;
        for (int i = 0; i < length; i++) if (board[y + i][x] != 0) return false;
    }
    return true;
}

void Battleship::placeShip(std::vector<std::vector<int>>& board, int x, int y, int length, bool horizontal) {
    for (int i = 0; i < length; i++) {
        if (horizontal) board[y][x + i] = 1;
        else board[y + i][x] = 1;
    }
}

void Battleship::autoPlaceShips(std::vector<std::vector<int>>& board) {
    int shipSizes[] = { 5, 4, 3, 3, 2 };
    for (int size : shipSizes) {
        bool placed = false;
        while (!placed) {
            int x = rand() % 10;
            int y = rand() % 10;
            bool horizontal = rand() % 2 == 0;
            if (canPlaceShip(board, x, y, size, horizontal)) {
                placeShip(board, x, y, size, horizontal);
                placed = true;
            }
        }
    }
}

void Battleship::manualPlaceShips() {
    int shipSizes[] = { 5, 4, 3, 3, 2 };
    std::string shipNames[] = { "Carrier (5)", "Battleship (4)", "Cruiser (3)", "Submarine (3)", "Destroyer (2)" };

    for (int i = 0; i < 5; i++) {
        int len = shipSizes[i];
        bool horizontal = true;
        int cx = 0, cy = 0;
        bool placing = true;

        while (placing) {
            Display::clearScreen();
            Display::printColored("=== SHIP PLACEMENT ===\n\n", Color::YELLOW);
            std::cout << "Placing: " << shipNames[i] << "\n";
            std::cout << "[WASD] Move | [R] Rotate | [SPACE] Confirm\n\n";

            std::cout << "   A B C D E F G H I J\n";
            for (int y = 0; y < 10; y++) {
                std::cout << y << " ";
                for (int x = 0; x < 10; x++) {
                    // Check if coordinate is part of the hovering ghost ship
                    bool isGhost = false;
                    if (horizontal && y == cy && x >= cx && x < cx + len) isGhost = true;
                    if (!horizontal && x == cx && y >= cy && y < cy + len) isGhost = true;

                    if (isGhost) {
                        bool valid = canPlaceShip(playerBoard, cx, cy, len, horizontal);
                        Display::printColored(" @", valid ? Color::GREEN : Color::RED);
                    }
                    else {
                        int cell = playerBoard[y][x];
                        if (cell == 0) std::cout << " ~";
                        else if (cell == 1) Display::printColored(" #", Color::CYAN);
                    }
                }
                std::cout << "\n";
            }

            int input = getInput();
            if (input == 'W' && cy > 0) cy--;
            if (input == 'S') {
                if (horizontal && cy < 9) cy++;
                else if (!horizontal && cy + len - 1 < 9) cy++;
            }
            if (input == 'A' && cx > 0) cx--;
            if (input == 'D') {
                if (horizontal && cx + len - 1 < 9) cx++;
                else if (!horizontal && cx < 9) cx++;
            }
            if (input == 'R') {
                horizontal = !horizontal;
                // Clamp back into bounds if rotation pushes it off board
                if (horizontal && cx + len > 10) cx = 10 - len;
                if (!horizontal && cy + len > 10) cy = 10 - len;
            }
            if (input == ' ' || input == '\r' || input == '\n') {
                if (canPlaceShip(playerBoard, cx, cy, len, horizontal)) {
                    placeShip(playerBoard, cx, cy, len, horizontal);
                    placing = false;
                }
            }
        }
    }
}

void Battleship::resetBoards() {
    playerBoard.assign(10, std::vector<int>(10, 0));
    botBoard.assign(10, std::vector<int>(10, 0));
    botFired.assign(10, std::vector<bool>(10, false));
    botTargetQueue.clear();

    pCursorX = 0;
    pCursorY = 0;
    actionMessage = "Awaiting your orders, Commander.";

    autoPlaceShips(botBoard);

    if (manualPlacement) manualPlaceShips();
    else autoPlaceShips(playerBoard);
}

void Battleship::drawBoards() {
    Display::clearScreen();
    Display::printColored("=== BATTLESHIP ===\n\n", Color::YELLOW);

    // Perfectly aligned headers using A-J
    Display::printColored("       YOUR FLEET                      ENEMY FLEET\n", Color::CYAN);
    std::cout << "   A B C D E F G H I J           A  B  C  D  E  F  G  H  I  J\n";

    for (int y = 0; y < 10; y++) {
        // --- DRAW PLAYER BOARD (Left) ---
        std::cout << y << " ";
        for (int x = 0; x < 10; x++) {
            std::cout << " ";
            int cell = playerBoard[y][x];
            if (cell == 0) std::cout << "~";
            else if (cell == 1) Display::printColored("#", Color::CYAN);
            else if (cell == 2) Display::printColored("O", Color::RESET);
            else if (cell == 3) Display::printColored("X", Color::RED);
        }

        // 8 Spaces + 1 digit + 1 Space = perfectly bridges the gap to the next board
        std::cout << "        " << y << " ";

        // --- DRAW BOT BOARD (Right) ---
        for (int x = 0; x < 10; x++) {
            if (x == pCursorX && y == pCursorY) Display::printColored("[", Color::CYAN);
            else std::cout << " ";

            int cell = botBoard[y][x];
            if (cell == 0 || cell == 1) Display::printColored("~", Color::BLUE);
            else if (cell == 2) Display::printColored("O", Color::RESET);
            else if (cell == 3) Display::printColored("X", Color::GREEN);

            if (x == pCursorX && y == pCursorY) Display::printColored("]", Color::CYAN);
            else std::cout << " ";
        }
        std::cout << "\n";
    }

    // Communication feed below boards
    Display::printColored("\n>>> " + actionMessage + "\n", Color::YELLOW);
    std::cout << "\n[WASD] Move Radar | [SPACE] Fire | [Q] Quit\n\n";
}

std::string Battleship::botTurn() {
    int targetX = -1, targetY = -1;

    while (!botTargetQueue.empty()) {
        auto t = botTargetQueue.back();
        botTargetQueue.pop_back();
        if (t.first >= 0 && t.first < 10 && t.second >= 0 && t.second < 10 && !botFired[t.second][t.first]) {
            targetX = t.first;
            targetY = t.second;
            break;
        }
    }

    if (targetX == -1) {
        std::vector<std::pair<int, int>> validTargets;
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                if (!botFired[y][x]) {
                    if (botDifficulty < 3 || (x + y) % 2 == 0) {
                        validTargets.push_back({ x, y });
                    }
                }
            }
        }

        if (validTargets.empty()) {
            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    if (!botFired[y][x]) validTargets.push_back({ x, y });
                }
            }
        }

        if (!validTargets.empty()) {
            int r = rand() % validTargets.size();
            targetX = validTargets[r].first;
            targetY = validTargets[r].second;
        }
    }

    if (targetX != -1) {
        botFired[targetY][targetX] = true;
        std::string coord = std::string(1, 'A' + targetX) + std::to_string(targetY);

        if (playerBoard[targetY][targetX] == 1) {
            playerBoard[targetY][targetX] = 3;
            if (botDifficulty >= 2) {
                botTargetQueue.push_back({ targetX, targetY - 1 });
                botTargetQueue.push_back({ targetX, targetY + 1 });
                botTargetQueue.push_back({ targetX - 1, targetY });
                botTargetQueue.push_back({ targetX + 1, targetY });
            }
            return "Enemy fired at " + coord + " and HIT YOUR SHIP!";
        }
        else if (playerBoard[targetY][targetX] == 0) {
            playerBoard[targetY][targetX] = 2;
            return "Enemy fired at " + coord + " and missed.";
        }
    }
    return "Enemy is reloading...";
}

bool Battleship::checkWin(const std::vector<std::vector<int>>& board) {
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (board[y][x] == 1) return false;
        }
    }
    return true;
}

void Battleship::play() {
    bool skipSetup = false;

    while (true) {
        if (!skipSetup && !setupOptions()) return;

        resetBoards();
        bool gameRunning = true;
        bool playerWon = false;

        while (gameRunning) {
            drawBoards();

            int input = getInput();

            if (input == 'Q') {
                std::cout << "Are you sure you want to quit to menu? (Y/N): ";
                bool confirmQuit = false;
                while (true) {
                    int ans = getInput();
                    if (ans == 'Y') { confirmQuit = true; break; }
                    if (ans == 'N') { break; }
                }
                if (confirmQuit) return;
                continue;
            }

            if (input == 'W' && pCursorY > 0) pCursorY--;
            if (input == 'S' && pCursorY < 9) pCursorY++;
            if (input == 'A' && pCursorX > 0) pCursorX--;
            if (input == 'D' && pCursorX < 9) pCursorX++;

            if (input == ' ' || input == '\r' || input == '\n') {
                int targetCell = botBoard[pCursorY][pCursorX];

                if (targetCell == 2 || targetCell == 3) continue; // Already shot here

                std::string pCoord = std::string(1, 'A' + pCursorX) + std::to_string(pCursorY);

                if (targetCell == 1) {
                    botBoard[pCursorY][pCursorX] = 3;
                    actionMessage = "You fired at " + pCoord + "... DIRECT HIT!";
                }
                else {
                    botBoard[pCursorY][pCursorX] = 2;
                    actionMessage = "You fired at " + pCoord + "... Miss.";
                }

                if (checkWin(botBoard)) {
                    playerWon = true;
                    gameRunning = false;
                    continue;
                }

                // Append bot's action to the message feed
                actionMessage += "\n>>> " + botTurn();

                if (checkWin(playerBoard)) {
                    playerWon = false;
                    gameRunning = false;
                }
            }
        }

        // --- END GAME MENU ---
        drawBoards();

        if (playerWon) Display::printColored("VICTORY! You destroyed the enemy fleet.\n\n", Color::GREEN);
        else Display::printColored("DEFEAT! Your fleet was sent to the bottom of the ocean.\n\n", Color::RED);

        int endSel = 0;
        std::string endOpts[] = { "Play Again (Same Setup)", "Change Setup", "Quit to Main Menu" };
        bool madeChoice = false;

        while (!madeChoice) {
            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select\n> ";

            int choice = getInput();
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 2) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { skipSetup = true; madeChoice = true; }
                if (endSel == 1) { skipSetup = false; madeChoice = true; }
                if (endSel == 2) { return; }
            }
            if (!madeChoice) drawBoards();
        }
    }
}