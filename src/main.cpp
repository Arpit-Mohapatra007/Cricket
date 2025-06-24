#include "game.h"
#include "ui.h"
#include "utils.h"
#include "player.h"
#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>

int main() {
    sqlite3* db;
    if (sqlite3_open("cricket.db", &db) != SQLITE_OK) { 
        std::cerr << "Database error: " << sqlite3_errmsg(db) << "\n"; 
        return 1; 
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS players (name TEXT PRIMARY KEY, bat_skill INTEGER, ball_skill INTEGER);"
                      "CREATE TABLE IF NOT EXISTS match_logs (match_id INTEGER, delivery INTEGER, bowler TEXT, batsman TEXT, shot_type TEXT, result TEXT);";
    if (sqlite3_exec(db, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
        std::cerr << "Table error: " << sqlite3_errmsg(db) << "\n";
    }

    while (true) {
        std::cout << "\n======|CLI CRICKET TOURNAMENT|======";
        int choice = getIntegerInput("\n1. Start Game\n2. Exit\nChoice: ", 1, 2);

        if (choice == 2) {
            break; // Exit loop
        }

        std::vector<PlayerProfile> teamA, teamB;
        takenPlayers.clear();

        int opp = getIntegerInput("1. vs Player\n2. vs Computer\nChoice: ", 1, 2);

        std::cout << "\nCreate Team A:\n";
        while (teamA.size() < 11) {
            std::cout << "Team A has " << teamA.size() << "/11 players.\n";
            createTeam(teamA, db, std::vector<std::string>(takenPlayers.begin(), takenPlayers.end()));
        }

        if (opp == 1) { // Player vs Player
            std::cout << "\nCreate Team B:\n";
            while (teamB.size() < 11) {
                std::cout << "Team B has " << teamB.size() << "/11 players.\n";
                createTeam(teamB, db, std::vector<std::string>(takenPlayers.begin(), takenPlayers.end()));
            }
            startGame(teamA, teamB, true, db);
        } else { // Player vs Computer
            DifficultyLevel difficulty;
            int diff_choice = getIntegerInput("Select Difficulty Level: \n1. Easy\n2. Moderate\n3. Expert\nChoice:", 1, 3);
            switch (diff_choice) {
                case 1: difficulty = EASY; break;
                case 2: difficulty = MODERATE; break;
                case 3: difficulty = EXPERT; break;
            }
            teamB = createComputerTeam(db, difficulty);
            startGame(teamA, teamB, false, db);
        }
    }

    sqlite3_close(db);
    std::cout << "Goodbye!\n";
    return 0;
}