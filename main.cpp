#include "Arcade.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif


int main() {

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

    Arcade myArcade;
    myArcade.run();
    return 0;
}