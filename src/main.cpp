#include <exception>
#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <PDCurses-3.9/curses.h>
#include <nlohmann/json.hpp>
#include "logic.cpp"

class Game {
public:    
    void start() {
        stdscr = initscr();
        noecho();
        curs_set(false);
        keypad(stdscr, TRUE);
        while (!is_game_ended) {
            next_screen();
        }
        endwin();
    }

private:
    WINDOW* stdscr;
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
    bool is_game_ended = false;

    std::function<void(void)> next_screen = [this](){title_screen();};

    void print_logo() {
        for (int i = 0; i < logo.size(); i++) {
            std::string line = logo[i];
            move(i + 1, 0);
            printw(line.c_str());
        }
        printw("\n");
        printw("\n");
        refresh();
    }

    int handle_choices(std::vector<std::string>& choices) {
        int y, x;
        getyx(stdscr, y, x);
        bool is_chosen = false;
        int choice = 0;
        while (!is_chosen) {
            for (int i = 0; i < choices.size(); i++) {
                if (choice == i) {
                    attr_on(A_STANDOUT, stdscr);
                    printw(">> ");
                } else {
                    printw("   ");
                }
                printw(choices[i].c_str());
                if (choice == i) {
                    attr_off(A_STANDOUT, stdscr);
                }
                move(y + i + 1, x);
            }
            int keypress = get_keypress();
            switch (keypress) {
                case KEY_UP:
                    choice--;
                    if (choice == -1) {
                        choice = choices.size() - 1;
                    }
                    break;
                case KEY_DOWN:
                    choice++;
                    if (choice == choices.size()) {
                        choice = 0;
                    }
                    break;
                case KEY_ENTER: case 10: case 13:
                    is_chosen = true;
                    break;
                case 27: // ESC
                    choice = -1;
                    is_chosen = true;
                    break;
            }
            move(y, x);
        }

        return choice;
    }

    void title_screen() {
        clear();
        move(0, 0);
        print_logo();
        std::vector<std::string> choices = {"Play", "About", "Quit"};
        int choice = handle_choices(choices);
        switch (choice) {
            case -1:
                exit(0);
                break;
            case 0:
                next_screen = [this](){play_screen();};
                break;
            case 1:
                next_screen = [this](){about_screen();};
                break;
            case 2:
                is_game_ended = true;
                break;
        }
    }

    int get_keypress() {
        int keypress = getch();
        if (keypress == 3) {
            exit(0xC000013A);
        }
        return keypress;
    }

    void about_screen() {
        clear();
        move(0, 0);
        printw("About description here.");
        get_keypress();
        next_screen = [this](){title_screen();};
    }

    void play_screen() {
        clear();
        move(0, 0);
        printw("Play screen (not yet implemented).");
        get_keypress();
        next_screen = [this](){title_screen();};
    }
};

int main() {
    Game().start();
}