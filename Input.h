#ifndef INPUT_H
#define INPUT_H

#include <ctype.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <conio.h>
    
    // Windows non-blocking check
    inline bool hasInput() {
        return _kbhit() != 0;
    }

#else
    #include <termios.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <fcntl.h>

    // Linux/Codespaces non-blocking check
    inline bool hasInput() {
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
            ungetc(ch, stdin); // Put it back so getInput() can read it
            return true;
        }
        return false;
    }

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

inline int getInput() {
    int ch = _getch();

    if (ch == 0 || ch == 224) {
        ch = _getch();
        if (ch == 72) return 'W'; 
        if (ch == 80) return 'S'; 
        if (ch == 75) return 'A'; 
        if (ch == 77) return 'D'; 
    }
    else if (ch == 27) {
        ch = _getch();
        if (ch == 91) { 
            ch = _getch();
            if (ch == 65) return 'W'; 
            if (ch == 66) return 'S'; 
            if (ch == 68) return 'A'; 
            if (ch == 67) return 'D'; 
        }
        return 27; 
    }
    return toupper(ch);
}

#endif