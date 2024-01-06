#include <curses.h>
#include <string>
#include <vector>

class MyWindow {
public:
    MyWindow(int height, int width, int y, int x) {
        window = newwin(height, width, y, x);
    }

    MyWindow(WINDOW* window) {
        this->window = window;
    }

    ~MyWindow() {
        delwin(window);
    }

    void setParent(MyWindow& parent) {
        parent_ptr = &parent;
        parent.refresh();
    }

    int getCurrentY() {
        return getcury(window);
    }

    int getCurrentX() {
        return getcurx(window);
    }

    void refresh() {
        wrefresh(window);
    }

    void print(std::string text) {
        wprintw(window, text.c_str());
    }

    void printLines(std::vector<std::string>& lines) {
        printLines(lines, 0, 0);
    }

    void printLines(std::vector<std::string>& lines, int y, int x) {
        int curr_y = getCurrentY();
        int curr_x = getCurrentX();
        for (std::string& line: lines) {
            print(line);
            curr_y++;
            moveCursor(curr_y, curr_x);
        }
    }

    void moveCursor(int y, int x) {
        wmove(window, y, x);
    }

    void clear() {
        wclear(window);
    }

    void clearLine() {
        wclrtoeol(window);
    }

    void setAttribute(attr_t attribute, bool value) {
        if (value == true) {
            wattr_on(window, attribute, NULL);
        } else {
            wattr_off(window, attribute, NULL);
        }
    }

protected:
    WINDOW* window;
    MyWindow* parent_ptr = nullptr;
};

class MyWindowBordered : public MyWindow {
public:
    MyWindow contentWindow;

    MyWindowBordered(int height, int width, int y, int x) : MyWindow(height, width, y, x), contentWindow(height - 2, width - 2, y + 1, x + 1) {
        contentWindow.setParent(*this);
        box(window, 0, 0);
    }

    void setParent(MyWindow& parent) {
        MyWindow::setParent(parent);
        refresh();
    }

    void refresh() {
        MyWindow::refresh();
        contentWindow.refresh();
    }

    void printContent(std::string text) {
        contentWindow.print(text);
    }

    void printLinesContent(std::vector<std::string>& lines) {
        contentWindow.printLines(lines);
    }

    void printLinesContent(std::vector<std::string>& lines, int y, int x) {
        contentWindow.printLines(lines, y, x);
    }

    void clear() {
        MyWindow::clear();
        box(window, 0, 0);
    }

    void clearContent() {
        contentWindow.clear();
    }
};