#include <iostream>
#include <string>
#include <vector>
#include <PDCurses-3.9/curses.h>
#include <nlohmann/json.hpp>

std::vector<std::string> logo = {
    "  ######## ######## ######## ########  \n",
    "                                       \n",
    "#   ####         ##  ####             #\n",
    "# ##    ##       ##    ##             #\n",
    "# #    # #       ##    ##             #\n",
    "# #   #  #   ######    ##      ####   #\n",
    "# #  #   # ##    ##    ##    ##    ## #\n",
    "# # #    # ##    ##    ##    ######## #\n",
    "# ##    ## ##    ##    ##    ##       #\n",
    "#   ####     ###### ########   ###### #\n",
    "                                       \n",
    "  ######## ######## ######## ########  \n",
};

void print_logo() {
    for (std::string line: logo) {
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

int main() {
    WINDOW* MAIN_WIN = initscr();
    noecho();
    title_screen();
    endwin();
}