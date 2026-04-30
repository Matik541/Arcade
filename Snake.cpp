#include "Snake.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

Snake::Snake() : Game("Snake", "Eat food to grow. Don't hit the walls or yourself!", true, true) {
    srand(static_cast<unsigned int>(time(0)));
}

bool Snake::setupOptions() {
    int sel = 0;
    std::string diffs[] = {
        "Sluggish (Slow speed, 20x10 board)", 
        "Classic (Medium speed, 25x15 board) <- DEFAULT & SCORED", 
        "Viper (Fast speed, 30x15 board)"
    };
    
    while (true) {
        Display::clearScreen();
        Display::printColored("--- SNAKE SETUP ---\n\n", Color::YELLOW);
        std::cout << "Select Difficulty:\n";
        
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
            if (sel == 0) { width = 20; height = 10; speedMs = 150; isScored = false; }
            if (sel == 1) { width = 25; height = 15; speedMs = 100; isScored = true; }
            if (sel == 2) { width = 30; height = 15; speedMs = 60; isScored = false; }
            break;
        }
    }
    return true;
}

void Snake::resetGame() {
    snake.clear();
    // Start with a snake of length 3 in the middle
    int startX = width / 2;
    int startY = height / 2;
    snake.push_back({startX, startY});
    snake.push_back({startX - 1, startY});
    snake.push_back({startX - 2, startY});

    dirX = 1; // Start moving Right
    dirY = 0;
    score = 0;

    spawnFood();
}

void Snake::spawnFood() {
    bool valid = false;
    while (!valid) {
        foodX = rand() % width;
        foodY = rand() % height;
        valid = true;
        // Make sure food doesn't spawn inside the snake's body
        for (const auto& segment : snake) {
            if (segment.first == foodX && segment.second == foodY) {
                valid = false;
                break;
            }
        }
    }
}

void Snake::drawBoard(bool gameOver) {
    Display::clearScreen();
    Display::printColored("=== SNAKE ===\n\n", Color::YELLOW);
    std::cout << "Score: " << score << "\n";
    std::cout << "Controls: [WASD] Steer | [Q] Quit\n\n";

    std::cout << "+ ";
    for (int i = 0; i < width; i++) std::cout << "— ";
    std::cout << "+\n";

    for (int y = 0; y < height; y++) {
        std::cout << "| ";
        for (int x = 0; x < width; x++) {
            
            bool isSnake = false;
            bool isHead = false;

            for (size_t i = 0; i < snake.size(); i++) {
                if (snake[i].first == x && snake[i].second == y) {
                    isSnake = true;
                    if (i == 0) isHead = true;
                    break;
                }
            }

            if (isHead) {
                Display::printColored("X ", gameOver ? Color::RED : Color::MAGENTA);
            } else if (isSnake) {
                Display::printColored("O ", gameOver ? Color::RED : Color::GREEN);
            } else if (x == foodX && y == foodY) {
                Display::printColored("ó ", Color::RED); 
            } else {
                std::cout << "  ";
            }
        }
        std::cout << "|\n";
    }

    std::cout << "+ ";
    for (int i = 0; i < width; i++) std::cout << "— ";
    std::cout << "+\n";
}

void Snake::play() {
    bool skipSetup = false;

    while (true) {
        if (!skipSetup && !setupOptions()) return;

        resetGame();
        bool gameRunning = true;

        while (gameRunning) {
            
            // 1. NON-BLOCKING INPUT
            if (hasInput()) {
                int input = getInput();
                
                if (input == 'Q') {
                    std::cout << "\nAre you sure you want to quit to menu? (Y/N): ";
                    while (true) {
                        int ans = getInput();
                        if (ans == 'Y') return;
                        if (ans == 'N') break;
                    }
                }
                
                // Change direction (Prevent 180-degree instant death turns)
                if (input == 'W' && dirY != 1)  { dirX = 0;  dirY = -1; }
                if (input == 'S' && dirY != -1) { dirX = 0;  dirY = 1; }
                if (input == 'A' && dirX != 1)  { dirX = -1; dirY = 0; }
                if (input == 'D' && dirX != -1) { dirX = 1;  dirY = 0; }
            }

            // 2. MOVEMENT LOGIC
            int newHeadX = snake.front().first + dirX;
            int newHeadY = snake.front().second + dirY;

            // Check Wall Collisions
            if (newHeadX < 0 || newHeadX >= width || newHeadY < 0 || newHeadY >= height) {
                gameRunning = false;
            }

            // Check Self Collisions
            for (size_t i = 0; i < snake.size(); i++) {
                if (newHeadX == snake[i].first && newHeadY == snake[i].second) {
                    gameRunning = false;
                }
            }

            if (!gameRunning) break; // Break out before updating the head so we can draw the death state

            // Push new head
            snake.push_front({newHeadX, newHeadY});

            // Check Food Collision
            if (newHeadX == foodX && newHeadY == foodY) {
                score += 10;
                spawnFood();
                // Notice we DON'T pop_back here. This is how the snake grows!
            } else {
                snake.pop_back(); // Remove tail if no food eaten
            }

            // 3. RENDER AND DELAY
            drawBoard(false);
            
            // The magic that makes it a real-time game
            std::this_thread::sleep_for(std::chrono::milliseconds(speedMs));
        }

        // --- GAME OVER ---
        drawBoard(true); // Draw red snake to show death
        Display::printColored("\nCRASH! Game Over.\n\n", Color::RED);

        if (isScored && db != nullptr && score > 0) {
            std::cout << "Enter your name for the Scoreboard (no spaces): ";
            std::string playerName;
            std::cin >> playerName;
            db->saveScore(name, playerName, score, isHigherBetter());
            Display::printColored("Score Saved!\n\n", Color::GREEN);
            while((getchar()) != '\n'); 
        }

        // --- INTERACTIVE END MENU ---
        int endSel = 0;
        std::string endOpts[] = {"Play Again (Same Setup)", "Change Setup", "Quit to Main Menu"};
        bool madeChoice = false; 

        while (!madeChoice) {
            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 3; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE] Select | [Q] Quit\n> ";

            int choice = getInput();
            if (choice == 'Q') return;
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 2) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { skipSetup = true; madeChoice = true; }
                if (endSel == 1) { skipSetup = false; madeChoice = true; }
                if (endSel == 2) { return; }
            }
            if (!madeChoice) drawBoard(true); // Re-render if they scroll
        }
    }
}