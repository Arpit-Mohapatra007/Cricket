#pragma once

#include <string>
#include <vector>
#include <utility>
#include <sqlite3.h>

// Forward declaration to avoid circular dependencies
class PlayerProfile;

bool playerExists(sqlite3* db, const std::string& name);
void loadPlayerSkills(sqlite3* db, PlayerProfile& p);
void insertPlayer(sqlite3* db, const PlayerProfile& p);
void updatePlayerSkills(sqlite3* db, const PlayerProfile& p);
void insertMatchLog(sqlite3* db, int match_id, int delivery, const std::string& bowler, const std::string& batsman, const std::string& shot, const std::string& result);
void displayMatchLogs(sqlite3* db, int match_id);
std::vector<std::pair<std::string, std::pair<int, int>>> fetchPlayersWithSkills(sqlite3* db, const std::vector<std::string>& excludeList = {});
std::vector<std::string> fetchAllPlayerNames(sqlite3* db, const std::vector<std::string>& excludeList = {});