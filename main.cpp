#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

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

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\e[?1049h";
    std::cout << "\e[1m";
    for (std::string line: logo) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;
    std::cout << "> Play" << std::endl;
    std::cout << "  Quit" << std::endl;
    std::cout << "\u2788" << std::endl;
    int i;
    std::cin >> i;
    std::cout << "\e[21m";
    std::cout << "\e[?1049l";
}