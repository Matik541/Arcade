#ifndef INPUT_H
#define INPUT_H

#include <ctype.h>
#include <iostream>
#include "Display.h"

/**
 * @file Input.h
 * @brief Moduł obsługi wejścia klawiatury w konsoli.
 * @details Zapewnia przenośność sterowania (Windows/Linux) oraz bezblokowe sprawdzanie bufora wejściowego.
 */

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    
    /**
     * @brief Sprawdza nieblokująco, czy w buforze wejściowym konsoli znajduje się znak.
     * @return true Jeśli naciśnięto klawisz.
     */
    inline bool hasInput() {
        Display::endFrame();
        return _kbhit() != 0;
    }

#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <fcntl.h>

    /**
     * @brief Sprawdza nieblokująco, czy w buforze wejściowym konsoli znajduje się znak (dla POSIX).
     * @return true Jeśli naciśnięto klawisz.
     */
    inline bool hasInput() {
        Display::endFrame();
        struct termios oldt, newt;
        int ch;
        int oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        
        ch = getchar();
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        
        if (ch != EOF) {
            ungetc(ch, stdin);
            return true;
        }
        return false;
    }

    /**
     * @brief Pobiera pojedynczy znak z wejścia bez oczekiwania na klawisz Enter i bez wypisywania go na ekranie (dla POSIX).
     * @return Kod pobranego znaku.
     */
    inline int _getch() {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif

/**
 * @brief Pobiera wejście użytkownika, mapując klawisze strzałek na odpowiedniki W/A/S/D.
 * @return Wielka litera klawisza lub odpowiednik mapowany na W/A/S/D.
 */
inline int getInput() {
    Display::endFrame();
    int ch = _getch();

    if (ch == 0 || ch == 224) {
        ch = _getch();
        if (ch == 72) return 'W'; // Strzałka w górę -> W
        if (ch == 80) return 'S'; // Strzałka w dół -> S
        if (ch == 75) return 'A'; // Strzałka w lewo -> A
        if (ch == 77) return 'D'; // Strzałka w prawo -> D
    }
    else if (ch == 27) {
        ch = _getch();
        if (ch == 91) { 
            ch = _getch();
            if (ch == 65) return 'W'; // Strzałka w górę -> W
            if (ch == 66) return 'S'; // Strzałka w dół -> S
            if (ch == 68) return 'A'; // Strzałka w lewo -> A
            if (ch == 67) return 'D'; // Strzałka w prawo -> D
        }
        return 27; // ESC
    }
    return toupper(ch);
}

#endif