#include "game.h"
#include "player.h"
#include "game_mechanics.h"
#include "ui.h"
#include "utils.h"
#include "database.h"
#include <iostream>
#include <algorithm>
#include <ctime>

int simulateInnings(std::vector<PlayerProfile>& battingTeam, std::vector<PlayerProfile>& bowlingTeam, int overs, bool batHuman, bool bowlHuman, int match_id, sqlite3* db) {
    Batting batting;
    Bowling bowling;
    int score = 0, wickets = 0, extras = 0, balls = 0, maxBalls = overs * 6, delivery = 0;
    bool freeHit = false;
    
    int max_overs_per_bowler;
    if (overs <= 5) max_overs_per_bowler = 2;
    else if (overs <= 10) max_overs_per_bowler = 3;
    else if (overs <= 20) max_overs_per_bowler = 4;
    else max_overs_per_bowler = std::max(1, static_cast<int>(overs * 0.2));

    for (auto& p : battingTeam) p.resetMatchStats();
    for (auto& p : bowlingTeam) p.resetMatchStats();

    auto openers = selectOpeningBatsmen(battingTeam, batHuman);
    battingTeam[openers.first].is_selected = true; 
    battingTeam[openers.second].is_selected = true;
    int batsman1 = openers.first, batsman2 = openers.second;
    int striker = batsman1;
    
    int bowler = selectBowler(bowlingTeam, bowlHuman, -1, max_overs_per_bowler);
    
    std::cout << "\nOpening batsmen: " << battingTeam[batsman1].name << " and " << battingTeam[batsman2].name << "\n";
    std::cout << "Opening bowler: " << bowlingTeam[bowler].name << "\n\n";

    while (balls < maxBalls && wickets < battingTeam.size() - 1 && striker != -1) {
        delivery++;
       
        std::cout << "\nBowler: " << bowlingTeam[bowler].name << ", Facing: " << battingTeam[striker].name << "\n";
        
        int bowlChoice = bowlHuman ? getIntegerInput("Bowling choice (1-Normal, 2-Fast, 3-Slow): ", 1, 3) - 1 : int_dis(gen) % 3;

        bool noBall = false;
        std::string shot_type, result;
        switch (bowlChoice) {
            case 0: noBall = bowling.normalBowling(extras); shot_type = "Normal"; break;
            case 1: noBall = bowling.fastBowling(extras); shot_type = "Fast"; break;
            case 2: noBall = bowling.slowBowling(extras); shot_type = "Slow"; break;
        }
        if (noBall) { 
            bowlingTeam[bowler].runs_conceded += 1; 
            freeHit = true; 
            result = "No Ball"; 
            insertMatchLog(db, match_id, delivery, bowlingTeam[bowler].name, battingTeam[striker].name, shot_type, result);
        }

        int batChoice = batHuman ? getIntegerInput("Batting choice (1-Defensive, 2-Normal, 3-Lofted): ", 1, 3) : int_dis(gen) % 3 + 1;

        int runs = 0;
        switch (batChoice) {
            case 1: runs = batting.defensiveShot(bowling.getCurrentStyle()); shot_type = "Defensive"; break;
            case 2: runs = batting.normalShot(bowling.getCurrentStyle()); shot_type = "Normal"; break;
            case 3: runs = batting.loftedShot(bowling.getCurrentStyle()); shot_type = "Lofted"; break;
        }

        if (runs == -1 && !noBall && !freeHit) {
            wickets++;
            bowlingTeam[bowler].wickets_taken++;
            battingTeam[striker].is_out = true;
            result = "OUT";
            insertMatchLog(db, match_id, delivery, bowlingTeam[bowler].name, battingTeam[striker].name, shot_type, result);
            
            int new_batsman_idx = selectBatsman(battingTeam, batHuman, "Wicket fallen! Select new batsman:");
            if (new_batsman_idx == -1) break;
            
            if (striker == batsman1) batsman1 = new_batsman_idx;
            else batsman2 = new_batsman_idx;
            striker = new_batsman_idx;
        } else {
            if (runs >= 0) { 
                score += runs; 
                battingTeam[striker].runs_scored += runs; 
                bowlingTeam[bowler].runs_conceded += runs; 
                result = std::to_string(runs) + " runs";
                if (runs % 2 == 1) { std::swap(batsman1, batsman2); striker = batsman1; }
            }
            if (!noBall) { 
                battingTeam[striker].balls_faced++; 
                bowlingTeam[bowler].balls_bowled++;
                balls++; 
                freeHit = false; 
                insertMatchLog(db, match_id, delivery, bowlingTeam[bowler].name, battingTeam[striker].name, shot_type, result);

                if (balls % 6 == 0 && balls < maxBalls) {
                    bowlingTeam[bowler].overs_bowled++;
                    bowler = selectBowler(bowlingTeam, bowlHuman, bowler, max_overs_per_bowler);
                    std::cout << "\nNew over!\nBowler: " << bowlingTeam[bowler].name << "\n";
                    std::swap(batsman1, batsman2);
                    striker = batsman1;
                }
            }
        }
        std::cout << "Score: " << score + extras << "/" << wickets << " (" << ballsToOvers(balls) << ")\n";
    }

    int finalScore = score + extras;
    std::cout << (wickets >= battingTeam.size() - 1 ? "Team all out for " : "Innings end: ") << finalScore << " runs.\n";

    displayBattingScorecard(battingTeam);
    displayBowlingScorecard(bowlingTeam);
    std::cout << "\n--- PERFORMANCE ---\nBatting:\n"; for (auto& p : battingTeam) p.updateBattingSkill();
    std::cout << "\nBowling:\n"; for (auto& p : bowlingTeam) p.updateBowlingSkill();
    for (auto& p : battingTeam) updatePlayerSkills(db, p);
    for (auto& p : bowlingTeam) updatePlayerSkills(db, p);

    return finalScore;
}

void startGame(std::vector<PlayerProfile>& teamA, std::vector<PlayerProfile>& teamB, bool isBHuman, sqlite3* db) {
    if (teamA.size() < 2 || teamB.size() < 2) { std::cout << "Both teams need at least 2 players.\n"; return; }

    int toss = getIntegerInput("Toss (1-Heads, 2-Tails): ", 1, 2);
    bool won = (toss == 1 && int_dis(gen) <= 5) || (toss == 2 && int_dis(gen) > 5);
    std::cout << (won ? "You won" : "You lost") << " the toss!\n";

    int overs = getIntegerInput("Overs (1-20): ", 1, 20);
    bool batFirst = won ? (getIntegerInput("1-Bat, 2-Bowl: ", 1, 2) == 1) : int_dis(gen) <= 5;
    int match_id = time(nullptr);
    
    int scoreA, scoreB;
    if (batFirst) {
        std::cout << "\n=== TEAM A BATTING ===\n";
        scoreA = simulateInnings(teamA, teamB, overs, true, isBHuman, match_id, db);
        std::cout << "\nTarget: " << scoreA + 1 << "\n";
        std::cout << "\n=== TEAM B BATTING ===\n";
        scoreB = simulateInnings(teamB, teamA, overs, isBHuman, true, match_id, db);
    } else {
        std::cout << "\n=== TEAM B BATTING ===\n";
        scoreB = simulateInnings(teamB, teamA, overs, isBHuman, true, match_id, db);
        std::cout << "\nTarget: " << scoreB + 1 << "\n";
        std::cout << "\n=== TEAM A BATTING ===\n";
        scoreA = simulateInnings(teamA, teamB, overs, true, isBHuman, match_id, db);
    }

    std::cout << "\n=== MATCH RESULT ===\n";
    std::cout << "Team A: " << scoreA << "\nTeam B: " << scoreB << "\n";
    std::cout << (scoreA > scoreB ? "Team A Wins!" : scoreB > scoreA ? "Team B Wins!" : "Tie!") << "\n";
    displayMatchLogs(db, match_id);
}