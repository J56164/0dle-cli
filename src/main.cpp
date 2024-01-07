#include <exception>
#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <PDCurses-3.9/curses.h>
#include <PDCurses-3.9/panel.h>
#include <nlohmann/json.hpp>
#include "level.h"
#include "state.h"
#include "file_tools.h"
#include "window_tools.h"

class Game {
public:
    Game(): main_window(MyWindow(initscr())) {
        noecho();
        curs_set(false);
        keypad(stdscr, TRUE);
        set_tabsize(2);
        setNextScreen(std::bind(&titleScreen, this));
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
    std::function<void(void)> next_screen;

    void setNextScreen(std::function<void(void)> screen) {
        next_screen = screen;
    }

    int handleChoices(std::vector<std::string>& choices, MyWindow& curr_window) {
        return handleChoices(choices, curr_window, 0, 0); 
    }

    int handleChoices(std::vector<std::string>& choices, MyWindow& curr_window, int start_y, int start_x) {
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

    class TitleScreen {
    public:
        TitleScreen(Game& game) : game(game) {}

        void show() {
            game.main_window.clear();
            MyWindow logo_window(game.logo.size(), game.logo[0].size(), 1, 2);
            logo_window.setParent(game.main_window);
            logo_window.moveCursor(0, 0);
            logo_window.printLines(game.logo);
            logo_window.refresh();
            MyWindowBordered choices_window(5, 10, game.logo.size() + 3, 2);
            choices_window.setParent(game.main_window);
            std::vector<std::string> choices = {"Play", "About", "Quit"};
            int choice = game.handleChoices(choices, choices_window.contentWindow);
            switch (choice) {
                case 0:
                    game.setNextScreen(std::bind(&levelSelectScreen, &game));
                    break;
                case 1:
                    game.setNextScreen(std::bind(&aboutScreen, &game));
                    break;
                case -1: case 2:
                    game.is_game_ended = true;
                    break;
            }
        }

    private:
        Game& game;
    };

    class AboutScreen {
    public:
        AboutScreen(Game& game) : game(game) {}

        void show() {
            game.main_window.clear();
            game.main_window.moveCursor(0, 0);
            MyWindowBordered about_window(20, 80, 1, 2);
            about_window.setParent(game.main_window);
            about_window.printContent("About description here.");
            about_window.refresh();
            game.get_keypress();
            game.setNextScreen(std::bind(&titleScreen, &game));
        }

    private:
        Game& game;
    };

    class LevelSelectScreen {
    public:
        LevelSelectScreen(Game& game) : game(game) {}

        void show() {
            game.main_window.clear();
            game.main_window.moveCursor(0, 0);
            MyWindowBordered description_window(4, 80, 1, 2);
            description_window.setParent(game.main_window);
            std::vector<std::string> descriptions = {
                "Level Select",
                "Up/Down to navigate, Enter to select, Escape to go back to title screen"
            };
            description_window.printLinesContent(descriptions);
            description_window.refresh();
            MyWindowBordered load_window(20, 80, 6, 2);
            load_window.setParent(game.main_window);
            while (true) {
                load_window.print(FileTools::getJoinedPath(game.curr_path));
                load_window.refresh();
                std::vector<FileTools::File> files = FileTools::getFiles(game.curr_path);
                std::vector<std::string> filenames(files.size() + 1);
                filenames[0] = "..";
                for (int i = 0; i < files.size(); i++) {
                    filenames[i + 1] = files[i].filename;
                }
                int choice = game.handleChoices(filenames, load_window.contentWindow);
                if (choice == -1) {
                    game.curr_path = {"levels"};
                    game.setNextScreen(std::bind(&titleScreen, &game));
                    return;
                }
                int file_index = choice - 1;
                if (file_index == -1) {
                    if (game.curr_path.size() == 1) {
                        game.setNextScreen(std::bind(&titleScreen, &game));
                        return;
                    } else {
                        game.curr_path.pop_back();
                    }
                } else if (files[file_index].is_directory == false) {
                    game.curr_path.push_back(files[file_index].filename);
                    std::string joined_path = FileTools::getJoinedPath(game.curr_path);
                    game.setNextScreen(std::bind(&playScreen, &game, joined_path));
                    game.curr_path.pop_back();
                    return;
                } else {
                    game.curr_path.push_back(files[file_index].filename);
                }
                load_window.clear();
            }
        }

    private:
        Game& game;
    };

    class PlayScreen {
    public:
        PlayScreen(Game& game, std::string& joined_path)
        : game(game), level(Level::fromFile(joined_path)), state(State(level)),
        statusWindow(MyWindowBordered(5, 80, 1, 2)), playWindow(MyWindowBordered(20, 80, 7, 2)) {}

        void show() {
            game.main_window.clear();
            game.main_window.moveCursor(0, 0);
            statusWindow.setParent(game.main_window);
            updateStatus();
            playWindow.setParent(game.main_window);
            showDescription();
            while (true) {
                int keypress = game.get_keypress();
                switch (keypress) {
                    case 'd':
                        showDescription();
                        break;
                    case 'c':
                        showCurrencies();
                        break;
                    case 'a':
                        showActions();
                        break;
                    case 'g':
                        showGoals();
                        break;
                    case '<':
                        break;
                    case '>':
                        break;
                    case KEY_UP:
                        break;
                    case KEY_DOWN:
                        break;
                    case 27: // ESC
                        game.setNextScreen(std::bind(&levelSelectScreen, &game));
                        return;
                }
            }
        }

        void updateStatus() {
            statusWindow.clearContent();
            statusWindow.printContent(level.name);
            statusWindow.refresh();
        }
        
        void showDescription() {
            playWindow.clearContent();
            playWindow.printContent(level.description);
            playWindow.refresh();
        }
        
        void showCurrencies() {
            playWindow.clearContent();
            for (int currency_id = 0; currency_id < level.currencyCount; currency_id++) {
                Currency& currency = level.currencies[currency_id];
                int amount = state.currencyAmounts[currency_id];
                playWindow.printContent("- " + currency.name + " [" + std::to_string(amount) + "]\n");
                if (!currency.effects.empty()) {
                    printEffects(currency.effects, 1);
                }
                playWindow.printContent("\n");
            }
            playWindow.refresh();
        }
        
        void showActions() {
            playWindow.clearContent();
            for (int action_id = 0; action_id < level.actionCount; action_id++) {
                Action& action = level.actions[action_id];
                playWindow.printContent("[ ] " + action.name + "\n");
                if (!action.effects.empty()) {
                    printEffects(action.effects, 1);
                }
                if (!action.requirements.empty()) {
                    printRequirements(action.requirements, 1);
                }
                playWindow.printContent("\n");
            }
            playWindow.refresh();
        }

        void showGoals() {
            playWindow.clearContent();
            for (Goal& goal: level.goals) {
                printGoal(goal, 0);
            }
            playWindow.refresh();
        }

        void printEffects(std::vector<Effect>& effects, int tabs) {
            playWindow.printContent(std::string(tabs, '\t'));
            playWindow.printContent("- Effects:\n");
            for (Effect& effect: effects) {
                printEffect(effect, tabs + 1);
            }
        }

        void printEffect(Effect& effect, int tabs) {
            playWindow.printContent(std::string(tabs, '\t'));
            playWindow.printContent("- " + effect.name + " += " + std::to_string(effect.increment) + "\n");
        }

        void printRequirements(std::vector<Requirement>& requirements, int tabs) {
            playWindow.printContent(std::string(tabs, '\t'));
            playWindow.printContent("- Requirements:\n");
            for (Requirement& requirement: requirements) {
                printRequirement(requirement, tabs + 1);
            }
        }

        void printRequirement(Requirement& requirement, int tabs) {
            playWindow.printContent(std::string(tabs, '\t'));
            if (state.isRequirementFulfilled(requirement)) {
                playWindow.printContent("[/] ");
            } else {
                playWindow.printContent("[x] ");
            }
            int currency_id = level.currencyNameToId[requirement.name];
            int amount = state.currencyAmounts[currency_id];
            playWindow.printContent(requirement.name + " (" + std::to_string(amount) + ") ");
            std::string symbol = compareTypeToSymbol[requirement.type];
            playWindow.printContent(symbol + " " + std::to_string(requirement.amount));
        }

        void printGoal(Goal& goal, int tabs) {
            playWindow.printContent(std::string(tabs, '\t'));
            playWindow.printContent("- " + goal.name + "\n");
            if (!goal.subGoals.empty()) {
                playWindow.printContent(std::string(tabs + 1, '\t'));
                playWindow.printContent("- Subgoals:\n");
                for (Goal& subgoal: goal.subGoals) {
                    printGoal(subgoal, tabs + 2);
                }
            }
            if (!goal.requirements.empty()) {
                playWindow.printContent(std::string(tabs + 1, '\t'));
                playWindow.printContent("- Requirements:\n");
                for (Requirement& requirement: goal.requirements) {
                    printRequirement(requirement, tabs + 2);
                }
            }
        }
    
    private:
        Game& game;
        Level level;
        State state;
        MyWindowBordered statusWindow;
        MyWindowBordered playWindow;
        std::unordered_map<std::string, std::string> compareTypeToSymbol = {
            {"greater-equal", ">="},
            {"greater", ">"},
            {"less-equal", "<="},
            {"less", "<"},
            {"equal", "=="},
            {"not-equal", "!="},
        };
    };

    void titleScreen() {
        TitleScreen titleScreen(*this);
        titleScreen.show();
    }

    void aboutScreen() {
        AboutScreen aboutScreen(*this);
        aboutScreen.show();
    }

    void levelSelectScreen() {
        LevelSelectScreen levelSelectScreen(*this);
        levelSelectScreen.show();
    }

    void playScreen(std::string& joined_path) {
        PlayScreen playScreen(*this, joined_path);
        playScreen.show();
    }
};

int main() {
    Game* game = new Game();
    game->start();
    delete game;
}