#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> logo = {
    "  ######## ######## ######## ########  ",
    "                                       ",
    "#   ####           #   ##             #",
    "# ##    ##         #    #             #",
    "# #    # #         #    #             #",
    "# #   #  #   ##### #    #     ######  #",
    "# #  #   # ##     ##    #    ##    ## #",
    "# # #    # #       #    #    ######## #",
    "# ##    ## #       #    #    #        #",
    "#   ####    ###### # #######  ######  #",
    "                                       ",
    "  ######## ######## ######## ########  ",
};

int main() {
    std::cout << "\e[?1049h";
    std::cout << "\e[1m";
    for (std::string line: logo) {
        std::cout << line << std::endl;
    }
    int i;
    std::cin >> i;
    std::cout << "\e[21m";
    std::cout << "\e[?1049l";
}