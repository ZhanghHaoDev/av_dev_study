#include "file_io_module.h"

#include <iostream>
#include <fstream>

#include "SDL.h"

file_io_module::file_io_module(){
    SDL_Init(SDL_INIT_EVENTS);
}

file_io_module::~file_io_module(){
    SDL_Quit();
}

void file_io_module::sdl_file_io_module() {
    // 文件路径
    const std::string file_path = "../assets/sdl_file_test.txt";

    // 写入文件
    std::ofstream outfile(file_path);
    if (outfile.is_open()) {
        outfile << "Hello, SDL file I/O!" << std::endl;
        outfile.close();
        std::cout << "Successfully wrote to the file." << std::endl;
    } else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }

    // 读取文件
    std::ifstream infile(file_path);
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            std::cout << "Read from file: " << line << std::endl;
        }
        infile.close();
    } else {
        std::cerr << "Failed to open the file for reading." << std::endl;
    }
}