#include "cli.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

std::vector<std::string> splitExtensions(const std::string& ext_str)
{
    std::vector<std::string> exts;
    std::istringstream ss(ext_str);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        token.erase(std::remove_if(token.begin(), token.end(), [](unsigned char character) {
            return std::isspace(character) != 0;
        }), token.end());
        if (!token.empty())
        {
            exts.push_back(token);
        }
    }
    return exts;
}

void printHelp(const char* prog_name)
{
    std::cout << "Usage: " << prog_name << " <path> [options]\n\n";
    std::cout << "Arguments:\n";
    std::cout << "  <path>            File or directory to analyze\n\n";
    std::cout << "Options:\n";
    std::cout << "  --ext <ext>       Only analyze the specified extensions, separated by commas (e.g. cpp,h,c)\n";
    std::cout << "  --show-files      Show detailed statistics for each file\n";
    std::cout << "  --help            Show this help message\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << prog_name << " src/\n";
    std::cout << "  " << prog_name << " main.cpp --show-files\n";
    std::cout << "  " << prog_name << " . --ext cpp,h --show-files\n";
}
