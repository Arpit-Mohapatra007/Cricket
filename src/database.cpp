#include "database.h"
#include "player.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

bool playerExists(sqlite3* db, const std::string& name) {
    sqlite3_stmt* stmt;
    const char* query = "SELECT 1 FROM players WHERE name = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return exists;
}

void loadPlayerSkills(sqlite3* db, PlayerProfile& p) {
    sqlite3_stmt* stmt;
    const char* query = "SELECT bat_skill, ball_skill FROM players WHERE name = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        p.bat_skill = sqlite3_column_int(stmt, 0);
        p.ball_skill = sqlite3_column_int(stmt, 1);
    }
    sqlite3_finalize(stmt);
}

void insertPlayer(sqlite3* db, const PlayerProfile& p) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO players (name, bat_skill, ball_skill) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, p.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, p.bat_skill);
    sqlite3_bind_int(stmt, 3, p.ball_skill);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void updatePlayerSkills(sqlite3* db, const PlayerProfile& p) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE players SET bat_skill = ?, ball_skill = ? WHERE name = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, p.bat_skill);
    sqlite3_bind_int(stmt, 2, p.ball_skill);
    sqlite3_bind_text(stmt, 3, p.name.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void insertMatchLog(sqlite3* db, int match_id, int delivery, const std::string& bowler, const std::string& batsman, const std::string& shot, const std::string& result) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO match_logs (match_id, delivery, bowler, batsman, shot_type, result) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, match_id);
    sqlite3_bind_int(stmt, 2, delivery);
    sqlite3_bind_text(stmt, 3, bowler.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, batsman.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, shot.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, result.c_str(), -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void displayMatchLogs(sqlite3* db, int match_id) {
    sqlite3_stmt* stmt;
    const char* query = "SELECT delivery, bowler, batsman, shot_type, result FROM match_logs WHERE match_id = ?;";
    sqlite3_prepare_v2(db, query, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, match_id);
    std::cout << "\nMatch Log:\n" << std::left << std::setw(10) << "Delivery" << std::setw(20) << "Bowler" << std::setw(20) << "Batsman" << std::setw(15) << "Shot" << std::setw(15) << "Result\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << std::setw(10) << sqlite3_column_int(stmt, 0) << std::setw(20) << sqlite3_column_text(stmt, 1) 
             << std::setw(20) << sqlite3_column_text(stmt, 2) << std::setw(15) << sqlite3_column_text(stmt, 3) 
             << std::setw(15) << sqlite3_column_text(stmt, 4) << "\n";
    }
    sqlite3_finalize(stmt);
}

std::vector<std::pair<std::string, std::pair<int, int>>> fetchPlayersWithSkills(sqlite3* db, const std::vector<std::string>& excludeList) {
    std::vector<std::pair<std::string, std::pair<int, int>>> players;
    sqlite3_stmt* stmt;
    const char* query = "SELECT name, bat_skill, ball_skill FROM players;";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return players;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (std::find(excludeList.begin(), excludeList.end(), name) == excludeList.end()) {
            int bat_skill = sqlite3_column_int(stmt, 1);
            int ball_skill = sqlite3_column_int(stmt, 2);
            players.push_back({name, {bat_skill, ball_skill}});
        }
    }
    sqlite3_finalize(stmt);
    return players;
}

std::vector<std::string> fetchAllPlayerNames(sqlite3* db, const std::vector<std::string>& excludeList) {
    // This function is not used in the final refactored code, but kept for completeness.
    // It can be removed if not needed.
    std::vector<std::string> names;
    sqlite3_stmt* stmt;
    const char* query = "SELECT name FROM players;";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) return names;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (std::find(excludeList.begin(), excludeList.end(), name) == excludeList.end())
            names.push_back(name);
    }
    sqlite3_finalize(stmt);
    return names;
}