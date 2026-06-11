#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include "Game.h"
#include <vector>

/**
 * @struct Bomb
 * @brief Struktura reprezentująca bombę podłożoną na mapie.
 */
struct Bomb {
    int x, y;      ///< Współrzędne bomby (X, Y).
    int ticksLeft; ///< Czas do wybuchu w cyklach gry.
    int radius;    ///< Promień rażenia wybuchu (liczba pól).
    int owner;     ///< Właściciel bomby: 1 (P1) lub 2 (P2).
};

/**
 * @struct Explosion
 * @brief Struktura reprezentująca aktywny płomień wybuchu bomby na planszy.
 */
struct Explosion {
    int x, y;      ///< Współrzędne wybuchu (X, Y).
    int ticksLeft; ///< Pozostały czas wyświetlania płomienia.
};

/**
 * @class Bomberman
 * @brief Klasa implementująca lokalną grę PvP Bomberman dla dwóch graczy.
 * @details P1 porusza się za pomocą WASD (bomba spacją), a P2 za pomocą IJKL (bomba enterem). Gra toczy się na generowanej mapie.
 */
class Bomberman : public Game {
private:
    int width = 15;  ///< Szerokość planszy gry.
    int height = 15; ///< Wysokość planszy gry.
    
    std::vector<std::vector<int>> map; ///< Siatka mapy: 0=puste pole, 1=ściana niezniszczalna, 2=blok zniszczalny.
    
    int p1X, p1Y;       ///< Współrzędne Gracza 1.
    bool p1Alive;       ///< Czy Gracz 1 żyje.
    int p1MaxBombs;     ///< Maksymalna liczba bomb, które P1 może podłożyć jednocześnie.
    int p1BombsActive;  ///< Aktualna liczba aktywnych bomb Gracza 1.
    
    int p2X, p2Y;       ///< Współrzędne Gracza 2.
    bool p2Alive;       ///< Czy Gracz 2 żyje.
    int p2MaxBombs;     ///< Maksymalna liczba bomb, które P2 może podłożyć jednocześnie.
    int p2BombsActive;  ///< Aktualna liczba aktywnych bomb Gracza 2.

    std::vector<Bomb> bombs;            ///< Lista aktywnych bomb na mapie.
    std::vector<Explosion> explosions;  ///< Lista aktywnych wybuchów na mapie.

    /**
     * @brief Generuje losową mapę z uwzględnieniem bezpiecznych stref startowych dla graczy.
     */
    void generateMap();

    /**
     * @brief Przygotowuje stan graczy, mapę i czyści listy bomb/wybuchów przed nową grą.
     */
    void resetGame();

    /**
     * @brief Obsługuje proces wybuchu bomby pod wybranym indeksem i niszczenie bloków/graczy.
     * @param index Indeks bomby w wektorze bombs.
     */
    void explodeBomb(int index);

    /**
     * @brief Sprawdza, czy na danej pozycji znajduje się już podłożona bomba.
     * @return true Jeśli na pozycji (x, y) leży bomba.
     */
    bool hasBombAt(int x, int y);

    /**
     * @brief Sprawdza, czy na danej pozycji występuje aktywny wybuch.
     * @return true Jeśli na pozycji (x, y) trwa wybuch.
     */
    bool hasExplosionAt(int x, int y);

    /**
     * @brief Rysuje mapę gry, graczy, bomby oraz efekty wybuchów w terminalu.
     */
    void drawBoard();

public:
    /**
     * @brief Konstruktor gry Bomberman.
     */
    Bomberman();

    /**
     * @brief Uruchamia pętlę meczu Bomberman PvP.
     */
    void play() override;
};

#endif