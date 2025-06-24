#pragma once

#include "player.h" // For PlayerProfile and DifficultyLevel
#include <vector>
#include <string>
#include <utility>
#include <sqlite3.h>

void displayBattingScorecard(const std::vector<PlayerProfile>& team);
void displayBowlingScorecard(const std::vector<PlayerProfile>& team);
void createTeam(std::vector<PlayerProfile>& players, sqlite3* db, const std::vector<std::string>& excludeList = {});
int selectBatsman(std::vector<PlayerProfile>& team, bool isHuman, const std::string& situation = "");
int selectBowler(const std::vector<PlayerProfile>& team, bool isHuman, int current_bowler, int max_overs_per_bowler);
std::pair<int, int> selectOpeningBatsmen(const std::vector<PlayerProfile>& team, bool isHuman);
std::vector<PlayerProfile> createComputerTeam(sqlite3* db, DifficultyLevel difficulty);