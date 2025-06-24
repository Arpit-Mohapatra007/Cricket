#pragma once

#include <string>
#include <random>
#include <set>

// Global random number generators declared as 'extern'
extern std::mt19937 gen;
extern std::uniform_real_distribution<double> dis;
extern std::uniform_int_distribution<int> int_dis;

// Global set of players already assigned to a team
extern std::set<std::string> takenPlayers;

std::string ballsToOvers(int balls);
int getIntegerInput(const std::string& prompt, int min_val, int max_val);