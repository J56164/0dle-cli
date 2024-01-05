#include <fstream>
#include <string>
#include <iostream>
// #include <experimental/filesystem>
#include <nlohmann/json.hpp>

class Logic {
public:
    Logic(std::string filename) {
        std::ifstream file(filename);
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line;
        }
        // nlohmann::json data =  nlohmann::json::parse(file);
    }
};

// int main() {
//     std::cout << std::experimental::filesystem::current_path() << std::endl;
//     Logic("levels/tutorials/tutorial1.json");
// }