#ifndef ARCADE_H
#define ARCADE_H

#include <vector>
#include <memory>
#include "Game.h"
#include "Database.h"

/**
 * @class Arcade
 * @brief Główny menedżer i menu wyboru gier dla Terminal Arcade.
 * @details Odpowiada za inicjalizację dostępnych gier, obsługę menu głównego, wyświetlanie najlepszych wyników oraz uruchamianie wybranej gry.
 */
class Arcade {
private:
    std::vector<std::shared_ptr<Game>> games; ///< Lista gier zarejestrowanych w systemie.
    
    std::string inputHistory; ///< Historia naciśniętych klawiszy (do wykrywania cheat code).
    bool rainbowMode = false; ///< Flaga określająca aktywację trybu tęczowego (rainbow mode).

    /**
     * @brief Renderuje menu główne i tabelę wyników wybranej gry.
     * @param selected Indeks aktualnie wybranej pozycji w menu.
     */
    void drawMenu(int selected);

public:
    /**
     * @brief Tworzy obiekt klasy Arcade i rejestruje wszystkie dostępne gry.
     */
    Arcade();

    /**
     * @brief Uruchamia główną pętlę menu i obsługę nawigacji Arcade.
     */
    void run();
};

#endif
