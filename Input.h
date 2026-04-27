#ifndef INPUT_H
#define INPUT_H

// Check for both 32-bit and 64-bit Windows
#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
// If not Windows, we assume Linux/Mac (Codespaces)
#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>

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

#endif