cmake_minimum_required(VERSION 3.15)
project(CryptoTool)
set(CMAKE_CXX_STANDARD 11)
add_executable(CryptoTool main.cpp Crypto.cpp FileHandler.cpp Menu.cpp)