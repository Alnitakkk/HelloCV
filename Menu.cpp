#include "Menu.h"
#include <iostream>
#include <limits>
#include "Crypto.h"
#include "FileHandler.h"

void Menu::display() {
    int choice;
    do {
        std::cout << "\n=== 文本加密解密工具 ===" << std::endl;
        std::cout << "1. 加密文本" << std::endl;
        std::cout << "2. 解密文本" << std::endl;
        std::cout << "3. 加密文件" << std::endl;
        std::cout << "4. 解密文件" << std::endl;
        std::cout << "5. 退出" << std::endl;
        std::cout << "请输入选项 (1-5): ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        handleInput(choice);
        
    } while (choice != 5);
}

// 添加缺失的handleInput函数实现
void Menu::handleInput(int choice) {
    switch (choice) {
        case 1: {
            std::string text;
            int key;
            std::cout << "请输入要加密的文本: ";
            std::getline(std::cin, text);
            std::cout << "请输入密钥: ";
            std::cin >> key;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string encrypted = Crypto::encrypt(text, key);
            std::cout << "加密结果: " << encrypted << std::endl;
            break;
        }
        case 2: {
            std::string text;
            int key;
            std::cout << "请输入要解密的文本: ";
            std::getline(std::cin, text);
            std::cout << "请输入密钥: ";
            std::cin >> key;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string decrypted = Crypto::decrypt(text, key);
            std::cout << "解密结果: " << decrypted << std::endl;
            break;
        }
        case 3: {
            std::string inputFile, outputFile;
            int key;
            std::cout << "请输入要加密的文件路径: ";
            std::getline(std::cin, inputFile);
            std::cout << "请输入输出文件路径: ";
            std::getline(std::cin, outputFile);
            std::cout << "请输入密钥: ";
            std::cin >> key;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            std::string content = FileHandler::readFile(inputFile);
            std::string encrypted = Crypto::encrypt(content, key);
            FileHandler::writeFile(outputFile, encrypted);
            std::cout << "文件加密完成！" << std::endl;
            break;
        }
        case 4: {
            std::string inputFile, outputFile;
            int key;
            std::cout << "请输入要解密的文件路径: ";
            std::getline(std::cin, inputFile);
            std::cout << "请输入输出文件路径: ";
            std::getline(std::cin, outputFile);
            std::cout << "请输入密钥: ";
            std::cin >> key;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            std::string content = FileHandler::readFile(inputFile);
            std::string decrypted = Crypto::decrypt(content, key);
            FileHandler::writeFile(outputFile, decrypted);
            std::cout << "文件解密完成！" << std::endl;
            break;
        }
        case 5:
            std::cout << "感谢使用，再见！" << std::endl;
            break;
        default:
            std::cout << "无效选项，请重新选择！" << std::endl;
    }
}