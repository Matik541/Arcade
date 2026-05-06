#include "Bomberman.h"
#include "Display.h"
#include "Input.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

Bomberman::Bomberman() : Game("Bomberman PvP", "Blow up the terrain and trap your opponent!", false, false) {
    srand(static_cast<unsigned int>(time(0)));
}

void Bomberman::generateMap() {
    map.assign(height, std::vector<int>(width, 0));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Borders
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                map[y][x] = 1;
            } 
            // Checkerboard indestructible pillars
            else if (x % 2 == 0 && y % 2 == 0) {
                map[y][x] = 1;
            } 
            // Random destructible boxes (60% chance)
            else {
                if (rand() % 100 < 60) map[y][x] = 2;
            }
        }
    }

    // Clear starting corners so players don't spawn trapped
    map[1][1] = 0; map[1][2] = 0; map[2][1] = 0;
    map[height - 2][width - 2] = 0; map[height - 2][width - 3] = 0; map[height - 3][width - 2] = 0;
}

void Bomberman::resetGame() {
    generateMap();
    
    p1X = 1; p1Y = 1;
    p1Alive = true;
    p1MaxBombs = 1;
    p1BombsActive = 0;
    
    p2X = width - 2; p2Y = height - 2;
    p2Alive = true;
    p2MaxBombs = 1;
    p2BombsActive = 0;

    bombs.clear();
    explosions.clear();
}

bool Bomberman::hasBombAt(int x, int y) {
    for (const auto& b : bombs) {
        if (b.x == x && b.y == y) return true;
    }
    return false;
}

bool Bomberman::hasExplosionAt(int x, int y) {
    for (const auto& e : explosions) {
        if (e.x == x && e.y == y) return true;
    }
    return false;
}

void Bomberman::explodeBomb(int index) {
    Bomb b = bombs[index];
    
    // Free up the player's bomb count
    if (b.owner == 1) p1BombsActive--;
    if (b.owner == 2) p2BombsActive--;
    
    bombs.erase(bombs.begin() + index);

    // Create center explosion (lasts for 5 ticks)
    explosions.push_back({b.x, b.y, 5});

    // Raycast outward in 4 directions
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};

    for (int dir = 0; dir < 4; dir++) {
        for (int r = 1; r <= b.radius; r++) {
            int nx = b.x + dx[dir] * r;
            int ny = b.y + dy[dir] * r;

            if (map[ny][nx] == 1) break; // Stopped by solid wall
            
            if (map[ny][nx] == 2) {
                map[ny][nx] = 0; // Destroy wooden box
                explosions.push_back({nx, ny, 5});
                break; // Explosion stops at the box it destroyed
            }

            // Chain Reaction: Did we hit another bomb?
            for (size_t i = 0; i < bombs.size(); i++) {
                if (bombs[i].x == nx && bombs[i].y == ny) {
                    bombs[i].ticksLeft = 1; // Force immediate detonation next tick
                }
            }

            explosions.push_back({nx, ny, 5});
        }
    }
}

void Bomberman::drawBoard() {
    Display::clearScreen();
    Display::printColored("=== TERMINAL BOMBERMAN ===\n\n", Color::YELLOW);
    
    Display::printColored("P1 (BLUE ): WASD to Move, SPACE to Bomb\n", Color::BLUE);
    Display::printColored("P2 (GREEN): IJKL to Move, O to Bomb\n", Color::GREEN);
    std::cout << "[Q] Quit Match\n\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            // Priority 1: Players
            if (p1Alive && x == p1X && y == p1Y) {
                Display::printColored("P1", Color::BLUE);
            } else if (p2Alive && x == p2X && y == p2Y) {
                Display::printColored("P2", Color::GREEN);
            } 
            // Priority 2: Explosions
            else if (hasExplosionAt(x, y)) {
                Display::printColored("><", Color::RED);
            } 
            // Priority 3: Bombs
            else if (hasBombAt(x, y)) {
                Display::printColored("()", Color::RED);
            } 
            // Priority 4: Map Terrain
            else {
                if (map[y][x] == 1) Display::printColored("██", Color::RESET);      // Solid wall
                else if (map[y][x] == 2) Display::printColored("▒▒", Color::YELLOW); // Destructible
                else std::cout << "  "; // Empty space
            }
        }
        std::cout << "\n";
    }
}

void Bomberman::play() {
    bool skipSetup = false; // We don't have setup options for this game yet, but we keep the loop structure

    while (true) {
        resetGame();
        bool gameRunning = true;

        while (gameRunning) {
            
            // 1. INPUT PROCESSING (Consume all buffered inputs so there is no lag)
            while (hasInput()) {
                int in = getInput();
                
                if (in == 'Q') {
                    std::cout << "\nQuit match? (Y/N): ";
                    while (true) {
                        int ans = getInput();
                        if (ans == 'Y') return;
                        if (ans == 'N') break;
                    }
                }

                // --- PLAYER 1 (WASD / SPACE) ---
                if (p1Alive) {
                    int nx = p1X, ny = p1Y;
                    if (in == 'W') ny--;
                    if (in == 'S') ny++;
                    if (in == 'A') nx--;
                    if (in == 'D') nx++;
                    
                    if (map[ny][nx] == 0 && !hasBombAt(nx, ny)) {
                        p1X = nx; p1Y = ny;
                    }

                    if (in == ' ' && p1BombsActive < p1MaxBombs && !hasBombAt(p1X, p1Y)) {
                        bombs.push_back({p1X, p1Y, 30, 2, 1}); // 30 ticks = ~1.5 seconds fuse
                        p1BombsActive++;
                    }
                }

                // --- PLAYER 2 (IJKL / O) ---
                if (p2Alive) {
                    int nx = p2X, ny = p2Y;
                    if (in == 'I') ny--; // UP
                    if (in == 'K') ny++; // DOWN
                    if (in == 'J') nx--; // LEFT
                    if (in == 'L') nx++; // RIGHT

                    if (map[ny][nx] == 0 && !hasBombAt(nx, ny)) {
                        p2X = nx; p2Y = ny;
                    }

                    if ((in == 'O') && p2BombsActive < p2MaxBombs && !hasBombAt(p2X, p2Y)) {
                        bombs.push_back({p2X, p2Y, 30, 2, 2});
                        p2BombsActive++;
                    }
                }
            }

            // 2. TICK UPDATES
            // Update Explosions (fade away)
            for (int i = 0; i < explosions.size(); i++) {
                explosions[i].ticksLeft--;
                if (explosions[i].ticksLeft <= 0) {
                    explosions.erase(explosions.begin() + i);
                    i--; // Adjust index
                }
            }

            // Update Bombs (countdown to detonation)
            for (int i = 0; i < bombs.size(); i++) {
                bombs[i].ticksLeft--;
                if (bombs[i].ticksLeft <= 0) {
                    explodeBomb(i);
                    i--; // Adjust index
                }
            }

            // 3. COLLISION / DEATH CHECK
            if (hasExplosionAt(p1X, p1Y)) p1Alive = false;
            if (hasExplosionAt(p2X, p2Y)) p2Alive = false;

            if (!p1Alive || !p2Alive) gameRunning = false;

            // 4. RENDER & DELAY (50ms per frame = 20 Frames Per Second)
            drawBoard();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // --- POST GAME ---
        drawBoard(); // Draw the final state showing who blew up
        
        std::cout << "\n";
        if (!p1Alive && !p2Alive) Display::printColored("MUTUAL DESTRUCTION! It's a draw!\n\n", Color::YELLOW);
        else if (!p1Alive) Display::printColored("PLAYER 2 WINS!\n\n", Color::RED);
        else if (!p2Alive) Display::printColored("PLAYER 1 WINS!\n\n", Color::CYAN);

        int endSel = 0;
        std::string endOpts[] = {"Play Again", "Quit to Main Menu"};
        bool madeChoice = false; 

        while (!madeChoice) {
            std::cout << "\n--- POST GAME ---\n";
            for (int i = 0; i < 2; i++) {
                if (i == endSel) Display::printColored("> " + endOpts[i] + "\n", Color::GREEN);
                else std::cout << "  " << endOpts[i] << "\n";
            }
            std::cout << "\n[W/S] Navigate | [SPACE/ENTER] Select | [Q] Quit\n> ";

            int choice = getInput();
            if (choice == 'Q') return;
            if (choice == 'W' && endSel > 0) endSel--;
            if (choice == 'S' && endSel < 1) endSel++;
            if (choice == ' ' || choice == '\r' || choice == '\n') {
                if (endSel == 0) { madeChoice = true; }
                if (endSel == 1) { return; }
            }
            if (!madeChoice) drawBoard(); 
        }
    }
}