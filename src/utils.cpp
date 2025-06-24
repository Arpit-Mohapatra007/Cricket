#include "utils.h"
#include <iostream>
#include <limits>
#include <string>

// Definitions for global random number generators
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<double> dis(0.0, 1.0);
std::uniform_int_distribution<int> int_dis(1, 10);

// Definition for global set of taken players
std::set<std::string> takenPlayers;

std::string ballsToOvers(int balls) {
    return std::to_string(balls / 6) + "." + std::to_string(balls % 6);
}

int getIntegerInput(const std::string& prompt, int min_val, int max_val) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (value < min_val || value > max_val) {
            std::cout << "Input out of range. Please enter a number between " << min_val << " and " << max_val << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}