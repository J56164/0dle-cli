#include <exception>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
#include <PDCurses-3.9/curses.h>
#include <PDCurses-3.9/panel.h>
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

    void title_screen() {
        clear();
        wmove(stdscr, 0, 0);
        print_logo();
        int y, x;
        getyx(stdscr, y, x);
        WINDOW* choices_window = newwin(3, 8, y, x);
        wrefresh(stdscr);
        std::vector<std::string> choices = {"Play", "About", "Quit"};
        int choice = handle_choices(choices, choices_window);
        switch (choice) {
            case -1:
                is_game_ended = true;
                break;
            case 0:
                next_screen = [this](){level_select_screen();};
                break;
            case 1:
                next_screen = [this](){about_screen();};
                break;
            case 2:
                is_game_ended = true;
                break;
        }
    }

    void about_screen() {
        clear();
        wmove(stdscr, 0, 0);
        WINDOW* load_window = newwin(10, 40, 1, 2);
        refresh();
        box(load_window, 0, 0);
        wrefresh(load_window);
        mvwprintw(load_window, 1, 1, "About description here.");
        wrefresh(load_window);
        get_keypress();
        next_screen = [this](){title_screen();};
    }

    void level_select_screen() {
        clear();
        wmove(stdscr, 0, 0);
        WINDOW* load_window = newwin(10, 40, 1, 2);
        refresh();
        box(load_window, 0, 0);
        wrefresh(load_window);
        mvwprintw(load_window, 0, 0, "/tutorials");
        wrefresh(load_window);
        std::vector<std::string> files = {"..", "tutorial1.json", "tutorial2.json", "(I didn't implement this yet)"};
        handle_choices(files, load_window, 1, 1);
        next_screen = [this](){title_screen();};
    }

    void print_logo() {
        for (int i = 0; i < logo.size(); i++) {
            std::string line = logo[i];
            wmove(stdscr, i + 1, 2);
            wprintw(stdscr, line.c_str());
        }
        wprintw(stdscr, "\n");
        wprintw(stdscr, "\n");
        refresh();
    }

    int handle_choices(std::vector<std::string>& choices, WINDOW* curr_window) {
        return handle_choices(choices, curr_window, 0, 0); 
    }

    int handle_choices(std::vector<std::string>& choices, WINDOW* curr_window, int start_y, int start_x) {
        bool is_chosen = false;
        int choice = 0;
        while (!is_chosen) {
            for (int i = 0; i < choices.size(); i++) {
                wmove(curr_window, start_y + i, start_x + 0);
                if (choice == i) {
                    wattr_on(curr_window, A_STANDOUT, curr_window);
                    wprintw(curr_window, ">> ");
                } else {
                    wprintw(curr_window, "   ");
                }
                wprintw(curr_window, choices[i].c_str());
                if (choice == i) {
                    wattr_off(curr_window, A_STANDOUT, curr_window);
                }
            }
            wrefresh(curr_window);

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
            wmove(curr_window, start_y + 0, start_x + 0);
        }

        return choice;
    }

    int get_keypress() {
        int keypress = getch();
        if (keypress == 3) {
            endwin();
            exit(0xC000013A);
        }
        return keypress;
    }
};

int main() {
    Game().start();
}