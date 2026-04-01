#include "../include/file.h"
std::string proceedFile(std::string filePath){

    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    return content;

}