#include <iostream>
#include <string>
#include <vector>
#include <curses.h>

const char FULL_BLOCK_CHAR = '\xDB';
const char TABLE_HORIZONTAL_CHAR = '\xB3';
const char TABLE_VERTICAL_CHAR = '\xC4';
const char TABLE_DOWN_LEFT_CHAR = '\xBF';
const char TABLE_UP_RIGHT_CHAR = '\xC0';
const char TABLE_UP_LEFT_CHAR = '\xD9';
const char TABLE_DOWN_RIGHT_CHAR = '\xDA';

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

// https://en.wikipedia.org/wiki/Code_page_437
// https://en.wikipedia.org/wiki/ANSI_escape_code

int main() {
    WINDOW* MAIN_WIN = initscr();
    noecho();
    title_screen();
    endwin();
}