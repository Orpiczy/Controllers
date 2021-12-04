cmake_minimum_required(VERSION 3.16)
project(FileSystemController)

set(CMAKE_CXX_STANDARD 20)

add_executable(FileSystemController main.cpp
        FileSystem/SimpleLogger.hpp FileSystem/SimpleLogger.cpp FileSystem/FileSystemController.cpp FileSystem/FileSystemController.hpp)