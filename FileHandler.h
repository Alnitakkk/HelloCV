#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <string>

class FileHandler {
public:
    static std::string readFile(const std::string& filePath);
    static void writeFile(const std::string& filePath, const std::string& content);
};
#endif