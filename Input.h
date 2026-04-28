#ifndef INPUT_H
#define INPUT_H

#include <ctype.h>

// Check for both 32-bit and 64-bit Windows
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
// If not Windows, we assume Linux/Mac (Codespaces)
#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>

    // Linux/Mac implementation of _getch()
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

// Smart input handler that translates arrows to WASD
inline int getInput() {
    int ch = _getch();

    // Windows Arrow Keys (Returns 0 or 224, followed by the key code)
    if (ch == 0 || ch == 224) {
        ch = _getch();
        if (ch == 72) return 'W'; 
        if (ch == 80) return 'S'; 
        if (ch == 75) return 'A'; 
        if (ch == 77) return 'D'; 
    }
    // Linux/Codespaces Arrow Keys (Returns ESC [ \033 ], followed by A, B, C, or D)
    else if (ch == 27) {
        ch = _getch();
        if (ch == 91) { 
            ch = _getch();
            if (ch == 65) return 'W'; 
            if (ch == 66) return 'S'; 
            if (ch == 68) return 'A'; 
            if (ch == 67) return 'D'; 
        }
        return 27; // Return raw ESC if it wasn't an arrow
    }

    return toupper(ch);
}

#endif