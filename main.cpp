// #include <conio.h>
#ifdef __linux__ 
    //linux code goes here
    // #include <ncurses.h>
#elif _WIN32
    // windows code goes here
    #include <windows.h>
    #include <conio.h>
#endif
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> logo = {
    "  ████████ ████████ ████████ ████████  ",
    "                                       ",
    "█   ████         ██  ████             █",
    "█ ██    ██       ██    ██             █",
    "█ █    █ █       ██    ██             █",
    "█ █   █  █   ██████    ██      ████   █",
    "█ █  █   █ ██    ██    ██    ██    ██ █",
    "█ █ █    █ ██    ██    ██    ████████ █",
    "█ ██    ██ ██    ██    ██    ██       █",
    "█   ████     ██████ ████████   ██████ █",
    "                                       ",
    "  ████████ ████████ ████████ ████████  ",
};

const char FULL_BLOCK_CHAR = '\xDB';
const char TABLE_HORIZONTAL_CHAR = '\xB3';
const char TABLE_VERTICAL_CHAR = '\xC4';
const char TABLE_DOWN_LEFT_CHAR = '\xBF';
const char TABLE_UP_RIGHT_CHAR = '\xC0';
const char TABLE_UP_LEFT_CHAR = '\xD9';
const char TABLE_DOWN_RIGHT_CHAR = '\xDA';

void print_logo() {
    for (std::string line: logo) {
        for (char c: line) {
            if (c == '#') {
                std::cout << FULL_BLOCK_CHAR;
            } else {
                std::cout << c;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int read_keypress() {
    int c;
    #ifdef _WIN32
        c = getch();
    #else
        // getch();
        std::cin >> c;
    #endif
    return c;
}

// https://en.wikipedia.org/wiki/Code_page_437
// https://en.wikipedia.org/wiki/ANSI_escape_code

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
    #elif __linux__
        // initscr();
        // timeout(-1);
    #endif
    std::cout << "\e[?1049h"; // Alternate screen on
    std::cout << "\e[H"; // Move cursor to home
    std::cout << "\e[?25l"; // Hides cursor
    print_logo();
    std::cout << "> Play" << std::endl;
    std::cout << "  Quit" << std::endl;
    read_keypress();
    std::cout << "\e[?25h"; // Shows cursor
    std::cout << "\e[?1049l"; // Alternate screen off
}