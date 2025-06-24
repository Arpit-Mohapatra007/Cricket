#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>

// Difficulty levels for computer opponents
enum DifficultyLevel { EASY, MODERATE, EXPERT };

// PlayerProfile class
class PlayerProfile {
public:
    std::string name;
    int bat_skill, ball_skill, runs_scored, balls_faced, runs_conceded, balls_bowled, wickets_taken;
    bool is_out, is_selected;
    int overs_bowled;

    PlayerProfile(std::string n = "Unknown", int bat = 50, int ball = 50);
    void resetMatchStats();
    double getStrikeRate() const;
    double getEconomyRate() const;
    void updateBattingSkill();
    void updateBowlingSkill();
};