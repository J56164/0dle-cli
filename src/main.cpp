#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <PDCurses-3.9/curses.h>
#include <nlohmann/json.hpp>
#include "logic.cpp"

class Game {
public:    
    void start() {
        main_win = initscr();
        cbreak();
        noecho();
        title_screen();
        endwin();
    }

private:
    WINDOW* main_win;
    std::vector<std::string> logo = {
        "  ████████ ████████ ████████ ████████  \n",
        "                                       \n",
        "█   ████         ██  ████             █\n",
        "█ ██    ██       ██    ██             █\n",
        "█ █    █ █       ██    ██             █\n",
        "█ █   █  █   ██████    ██      ████   █\n",
        "█ █  █   █ ██    ██    ██    ██    ██ █\n",
        "█ █ █    █ ██    ██    ██    ████████ █\n",
        "█ ██    ██ ██    ██    ██    ██       █\n",
        "█   ████     ██████ ████████   ██████ █\n",
        "                                       \n",
        "  ████████ ████████ ████████ ████████  \n",
    };

    void print_logo() {
        for (int i = 0; i < logo.size(); i++) {
            std::string line = logo[i];
            move(i + 1, 2);
            printw(line.c_str());
        }
        refresh();
    }

    int read_keypress() {
        int c = getch();
        return c;
    }

    void title_screen() {
        print_logo();
        printw("\n");
        printw("> Play\n");
        printw("  Quit\n");
        while (true) {
            int keypress = read_keypress();
        }
    }
};



int main() {
    Game().start();
}