#include "FileHandler.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::string FileHandler::readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void FileHandler::writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法创建文件: " + filePath);
    }
    
    file << content;
}
