#include "Arcade.h"
#include <iostream>
#include <csignal>
#include <string_view>

/**
 * @file main.cpp
 * @brief Punkt wejściowy aplikacji Terminal Arcade.
 * @details Konfiguruje terminal do pracy w trybie alternatywnego bufora i kodowania UTF-8, po czym uruchamia główną pętlę Arcade.
 */

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

// Zadeklarowana w tests.cpp
void runArcadeTests();

/**
 * @brief Przywraca standardowy stan terminala (widoczny kursor, główny bufor).
 * @param signum Kod sygnału wywołującego przywrócenie (0 przy normalnym zamknięciu).
 */
void restoreTerminal(int signum) {
    std::cout << "\033[?25h\033[?1049l" << std::flush;
    if (signum != 0) {
        std::exit(signum);
    }
}

/**
 * @brief Główna funkcja programu.
 * @param argc Liczba argumentów linii komend.
 * @param argv Tablica argumentów linii komend.
 */
int main(int argc, char* argv[]) {
    // Sprawdzenie, czy uruchomiono z flagą --test
    if (argc > 1 && std::string_view(argv[1]) == "--test") {
        runArcadeTests();
        return 0;
    }

    // Obsługa sygnałów zamknięcia w celu sprzątania terminala
    std::signal(SIGINT, restoreTerminal);
    std::signal(SIGTERM, restoreTerminal);

    #if defined(_WIN32) || defined(_WIN64)
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (consoleHandle != INVALID_HANDLE_VALUE) {
            DWORD consoleMode = 0;
            if (GetConsoleMode(consoleHandle, &consoleMode)) {
                SetConsoleMode(consoleHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
    #endif

    // Przełączenie na bufor alternatywny i ukrycie kursora
    std::cout << "\033[?1049h\033[?25l" << std::flush;

    Arcade myArcade;
    myArcade.run();

    // Przywrócenie terminala przy normalnym zakończeniu
    restoreTerminal(0);
    return 0;
}