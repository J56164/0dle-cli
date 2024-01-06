#include <exception>
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
#include <PDCurses-3.9/curses.h>
#include <PDCurses-3.9/panel.h>
#include <nlohmann/json.hpp>
#include "level.h"
#include "file_tools.h"
#include "window_tools.h"

class Game {
public:
    Game(): main_window(MyWindow(initscr())) {
        noecho();
        curs_set(false);
        keypad(stdscr, TRUE);
    }

    ~Game() {
        endwin();
    }

    void start() {
        while (!is_game_ended) {
            next_screen();
        }
    }

private:
    MyWindow main_window;
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
    std::vector<std::string> curr_path = {"levels"};
    std::function<void(void)> next_screen = [this](){title_screen();};

    void title_screen() {
        main_window.clear();
        MyWindow logo_window(logo.size(), logo[0].size(), 1, 2);
        logo_window.setParent(main_window);
        logo_window.moveCursor(0, 0);
        logo_window.printLines(logo);
        logo_window.refresh();
        MyWindowBordered choices_window(5, 10, logo.size() + 3, 2);
        choices_window.setParent(main_window);
        std::vector<std::string> choices = {"Play", "About", "Quit"};
        int choice = handle_choices(choices, choices_window.contentWindow);
        switch (choice) {
            case 0:
                next_screen = [this](){level_select_screen();};
                break;
            case 1:
                next_screen = [this](){about_screen();};
                break;
            case -1: case 2:
                is_game_ended = true;
                break;
        }
    }

    void about_screen() {
        main_window.clear();
        main_window.moveCursor(0, 0);
        MyWindowBordered about_window(10, 40, 1, 2);
        about_window.setParent(main_window);
        about_window.printContent("About description here.");
        about_window.refresh();
        get_keypress();
        next_screen = [this](){title_screen();};
    }

    void level_select_screen() {
        main_window.clear();
        main_window.moveCursor(0, 0);
        MyWindowBordered description_window(4, 80, 1, 2);
        description_window.setParent(main_window);
        std::vector<std::string> descriptions = {
            "Level Select",
            "Up/Down to navigate, Enter to select, Escape to go back to title screen"
        };
        description_window.printLinesContent(descriptions);
        description_window.refresh();
        MyWindowBordered load_window(20, 80, 6, 2);
        load_window.setParent(main_window);
        while (true) {
            load_window.print(FileTools::GetJoinedPath(curr_path));
            load_window.refresh();
            std::vector<FileTools::File> files = FileTools::GetFiles(curr_path);
            std::vector<std::string> filenames(files.size() + 1);
            filenames[0] = "..";
            for (int i = 0; i < files.size(); i++) {
                filenames[i + 1] = files[i].filename;
            }
            int choice = handle_choices(filenames, load_window.contentWindow);
            if (choice == -1) {
                curr_path = {"levels"};
                next_screen = [this](){title_screen();};
                return;
            }
            int file_index = choice - 1;
            if (file_index == -1) {
                if (curr_path.size() == 1) {
                    next_screen = [this](){title_screen();};
                    return;
                } else {
                    curr_path.pop_back();
                }
            } else if (files[file_index].is_directory == false) {
                curr_path.push_back(files[file_index].filename);
                std::string joined_path = FileTools::GetJoinedPath(curr_path);
                next_screen = [this, joined_path](){play_screen(joined_path);};
                curr_path.pop_back();
                return;
            } else {
                curr_path.push_back(files[file_index].filename);
            }
            load_window.clear();
        }
    }

    void play_screen(std::string joined_filepath) {
        main_window.clear();
        main_window.moveCursor(0, 0);
        MyWindowBordered status_window(4, 80, 1, 2);
        status_window.setParent(main_window);
        std::vector<std::string> descriptions = {
            "Play screen here.",
            joined_filepath
        };
        status_window.printLinesContent(descriptions);
        status_window.refresh();
        MyWindowBordered play_window(20, 80, 6, 2);
        play_window.setParent(main_window);
        get_keypress();
        next_screen = [this](){level_select_screen();};
    }

    int handle_choices(std::vector<std::string>& choices, MyWindow& curr_window) {
        return handle_choices(choices, curr_window, 0, 0); 
    }

    int handle_choices(std::vector<std::string>& choices, MyWindow& curr_window, int start_y, int start_x) {
        bool is_chosen = false;
        int choice = 0;
        while (!is_chosen) {
            for (int i = 0; i < choices.size(); i++) {
                curr_window.moveCursor(start_y + i, start_x);
                curr_window.clearLine();
                if (choice == i) {
                    curr_window.setAttribute(A_STANDOUT, true);
                    curr_window.print(">> " + choices[i]);
                    curr_window.setAttribute(A_STANDOUT, false);
                } else {
                    curr_window.print("   " + choices[i]);
                }
            }
            curr_window.refresh();

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
            curr_window.moveCursor(start_y, start_x);
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
    Game* game = new Game();
    game->start();
    delete game;
}