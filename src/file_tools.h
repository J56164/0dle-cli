#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

class FileTools {
public:
    struct File {
        std::string filename;
        bool is_directory;

        File(std::string filename, bool is_directory): filename(filename), is_directory(is_directory) {}
    };

    static std::string GetJoinedPath(std::vector<std::string>& path) {
        std::string joined_path = "";
        for (int i = 0; i < path.size(); i++) {
            joined_path += path[i];
            if (i != path.size() - 1) {
                joined_path += "/";
            }
        }
        return joined_path;
    }

    static std::vector<File> GetFiles(std::vector<std::string>& path) {
        std::string find_path = GetJoinedPath(path);
        find_path += "/*";

        std::vector<File> files;
        WIN32_FIND_DATAA data;
        HANDLE handle = FindFirstFileA(find_path.c_str(), &data);
        if (handle != INVALID_HANDLE_VALUE) {
            bool is_succeed = true;
            while (is_succeed) {
                std::string filename = data.cFileName;
                bool is_directory = (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
                if (filename != "." && filename != "..") {
                    files.push_back(File(filename, is_directory));
                }
                is_succeed = FindNextFileA(handle, &data);
            }
        }

        return files;
    }
};