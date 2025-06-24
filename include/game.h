#pragma once

#include <vector>
#include <sqlite3.h>

class PlayerProfile;

int simulateInnings(std::vector<PlayerProfile>& battingTeam, std::vector<PlayerProfile>& bowlingTeam, int overs, bool batHuman, bool bowlHuman, int match_id, sqlite3* db);
void startGame(std::vector<PlayerProfile>& teamA, std::vector<PlayerProfile>& teamB, bool isBHuman, sqlite3* db);