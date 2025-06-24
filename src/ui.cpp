#include "ui.h"
#include "utils.h"
#include "database.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

void displayBattingScorecard(const std::vector<PlayerProfile>& team) {
    std::cout << "\nBatting Scorecard:\n" << std::left << std::setw(20) << "Name" << std::setw(10) << "Runs" << std::setw(10) << "Balls" << std::setw(15) << "Strike Rate" << std::setw(10) << "Status\n";
    for (const auto& p : team) {
        if (p.balls_faced || p.is_out) {
            std::cout << std::setw(20) << p.name << std::setw(10) << p.runs_scored << std::setw(10) << p.balls_faced 
                 << std::fixed << std::setprecision(2) << std::setw(15) << p.getStrikeRate() 
                 << std::setw(10) << (p.is_out ? "OUT" : "NOT OUT") << "\n";
        }
    }
}

void displayBowlingScorecard(const std::vector<PlayerProfile>& team) {
    std::cout << "\nBowling Scorecard:\n" << std::left << std::setw(20) << "Name" << std::setw(10) << "Overs" << std::setw(10) << "Runs" << std::setw(10) << "Wickets" << std::setw(15) << "Economy\n";
    for (const auto& p : team) {
        if (p.balls_bowled) {
            std::cout << std::setw(20) << p.name << std::setw(10) << ballsToOvers(p.balls_bowled) << std::setw(10) << p.runs_conceded 
                 << std::setw(10) << p.wickets_taken << std::fixed << std::setprecision(2) << std::setw(15) << p.getEconomyRate() << "\n";
        }
    }
}

void createTeam(std::vector<PlayerProfile>& players, sqlite3* db, const std::vector<std::string>& excludeList) {
    int mode = getIntegerInput("\n1. Create a new Player Profile\n2. Select an Existing Player\n3. Remove a Player from the Team\nChoice: ", 1, 3);
    std::string name;
    int bat_skill, ball_skill;

    switch (mode) {
        case 1:{
            std::cout << "Enter new player name: ";
            getline(std::cin, name);
            bat_skill = getIntegerInput("Enter batting skill (0-100): ", 0, 100);
            ball_skill = getIntegerInput("Enter bowling skill (0-100): ", 0, 100);
            insertPlayer(db, PlayerProfile(name, bat_skill, ball_skill));
            players.emplace_back(name, bat_skill, ball_skill);
            std::cout << "Player created and added to the team.\n";
            takenPlayers.insert(name);
            break;
        }
        case 2:{
            auto playersWithSkills = fetchPlayersWithSkills(db, excludeList);
            if (playersWithSkills.empty()) {
                std::cout << "No existing players available to select.\n";
                return;
            }
            std::cout << "Existing players:\n";
            std::cout << std::left << std::setw(5) << "No." << std::setw(25) << "Name" << std::setw(12) << "Bat Skill" << std::setw(12) << "Bowl Skill\n";
            std::cout << std::string(54, '-') << "\n";
            for (size_t i = 0; i < playersWithSkills.size(); ++i) {
                std::cout << std::setw(5) << (i + 1) << std::setw(25) << playersWithSkills[i].first 
                    << std::setw(12) << playersWithSkills[i].second.first 
                    << std::setw(12) << playersWithSkills[i].second.second << "\n";
            }
            size_t idx = getIntegerInput("Select (1-" + std::to_string(playersWithSkills.size()) + "): ", 1, playersWithSkills.size());
            name = playersWithSkills[idx - 1].first;
            bat_skill = playersWithSkills[idx - 1].second.first;
            ball_skill = playersWithSkills[idx - 1].second.second;
            std::cout << "Selected \"" << name << "\" with skills Bat=" << bat_skill << ", Bowl=" << ball_skill << "\n";
            players.emplace_back(name, bat_skill, ball_skill);
            takenPlayers.insert(name);
            break;
        }
        case 3:{
            if (players.empty()) {
                std::cout << "No players to remove.\n";
            } else {
                std::cout << "Current players:\n";
                for (size_t i = 0; i < players.size(); ++i)
                    std::cout << (i+1) << ". " << players[i].name << "\n";
                int rem = getIntegerInput("Remove which? (1-" + std::to_string(players.size()) + "): ", 1, players.size());
                takenPlayers.erase(players[rem-1].name);
                players.erase(players.begin() + (rem-1));
                std::cout << "Removed.\n";
            }
            break;
        }
        default:{
            std::cout << "Invalid choice.\n";
            return;
        }
    }
}

int selectBatsman(std::vector<PlayerProfile>& team, bool isHuman, const std::string& situation) {
    std::vector<int> available;
    for (size_t i = 0; i < team.size(); i++) {
        if (!team[i].is_out && !team[i].is_selected) {
            available.push_back(i);
        }
    }
    
    if (available.empty()) return -1;
    int selected_idx = -1;
    
    if (!isHuman) {
        int best_idx = available[0];
        for (int idx : available) {
            if (team[idx].bat_skill > team[best_idx].bat_skill) {
                best_idx = idx;
            }
        }
        selected_idx = best_idx;
    } else {
        std::cout << "\n" << situation << "\nAvailable batsmen:\n";
        for (size_t i = 0; i < available.size(); i++) {
            int idx = available[i];
            std::cout << (i + 1) << ". " << team[idx].name << "(Bat:" << team[idx].bat_skill << ", Ball:" << team[idx].ball_skill << ")\n";
        }
        int choice = getIntegerInput("Select batsman (1-" + std::to_string(available.size()) + "): ", 1, available.size());
        selected_idx = available[choice - 1];
    }
    if(selected_idx != -1) {team[selected_idx].is_selected = true;}
    return selected_idx;
}

int selectBowler(const std::vector<PlayerProfile>& team, bool isHuman, int current_bowler, int max_overs_per_bowler) {
    std::vector<int> available;
    for (size_t i = 0; i < team.size(); i++) {
        if (i != current_bowler && team[i].overs_bowled < max_overs_per_bowler) {
            available.push_back(i);
        }
    }
    
    if (available.empty()) {
        for (size_t i = 0; i < team.size(); i++) {
            if (i != current_bowler) available.push_back(i);
        }
    }
    
    if (available.empty()) return current_bowler;
    
    if (!isHuman) {
        int best_idx = available[0];
        for (int idx : available) {
            if (team[idx].ball_skill > team[best_idx].ball_skill && team[idx].overs_bowled < max_overs_per_bowler) {
                best_idx = idx;
            }
        }
        return best_idx;
    }
    
    std::cout << "\nSelect bowler for new over:\n";
    for (size_t i = 0; i < available.size(); i++) {
        int idx = available[i];
        std::cout << (i + 1) << ". " << team[idx].name << " (Ball: " << team[idx].ball_skill << ", Overs: " << team[idx].overs_bowled << "/" << max_overs_per_bowler << ")\n";
    }
    
    int choice = getIntegerInput("Select bowler (1-" + std::to_string(available.size()) + "): ", 1, available.size());
    return available[choice - 1];
}

std::pair<int, int> selectOpeningBatsmen(const std::vector<PlayerProfile>& team, bool isHuman) {
    if (!isHuman) {
        std::vector<std::pair<int, int>> skills_idx;
        for (size_t i = 0; i < team.size(); i++) {
            skills_idx.push_back({team[i].bat_skill, (int)i});
        }
        sort(skills_idx.rbegin(), skills_idx.rend());
        return {skills_idx[0].second, skills_idx[1].second};
    }
    
    std::cout << "\nSelect opening batsmen:\n";
    for (size_t i = 0; i < team.size(); i++) {
        std::cout << (i + 1) << ". " << team[i].name << " (Bat: " << team[i].bat_skill << ", Ball: " << team[i].ball_skill << ")\n";
    }
    
    int opener1 = getIntegerInput("Select first opener (1-" + std::to_string(team.size()) + "): ", 1, team.size()) - 1;
    int opener2;
    do {
        opener2 = getIntegerInput("Select second opener (1-" + std::to_string(team.size()) + ") [different from first]: ", 1, team.size()) - 1;
    } while (opener2 == opener1);
    
    return {opener1, opener2};
}

std::vector<PlayerProfile> createComputerTeam(sqlite3* db, DifficultyLevel difficulty) {
    std::vector<PlayerProfile> team;
    std::vector<std::string> defaultNames = { "Ravindra Senanayake", "Thabo Mbekelani", "Oliver Wrenford", "Jaxon Redfern", "Marcus Thompson", "Arjun Patel", "James Rodriguez", "Chen Wei Ming", "Abdul Rahman", "David O'Connor", "Ravi Kumar" };
    
    for (const auto& name : defaultNames) {
        int bat_skill, ball_skill;
        switch (difficulty) {
            case EASY: bat_skill = std::uniform_int_distribution<int>(30, 60)(gen); ball_skill = std::uniform_int_distribution<int>(30, 60)(gen); break;
            case MODERATE: bat_skill = std::uniform_int_distribution<int>(50, 80)(gen); ball_skill = std::uniform_int_distribution<int>(50, 80)(gen); break;
            case EXPERT: bat_skill = std::uniform_int_distribution<int>(70, 100)(gen); ball_skill = std::uniform_int_distribution<int>(70, 100)(gen); break;
        }
        team.emplace_back(name, bat_skill, ball_skill);
        takenPlayers.insert(name);
    }
    
    std::string difficultyStr = (difficulty == EASY) ? "Easy" : (difficulty == MODERATE) ? "Moderate" : "Expert";
    std::cout << "\nComputer Team (" << difficultyStr << " difficulty) selected:\n";
    std::cout << std::left << std::setw(25) << "Name" << std::setw(12) << "Bat Skill" << std::setw(12) << "Bowl Skill\n";
    std::cout << std::string(49, '-') << "\n";
    for (const auto& player : team) {
        std::cout << std::setw(25) << player.name << std::setw(12) << player.bat_skill << std::setw(12) << player.ball_skill << "\n";
    }
    
    return team;
}