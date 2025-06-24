#include "player.h"

PlayerProfile::PlayerProfile(std::string n, int bat, int ball)
    : name(n), bat_skill(bat), ball_skill(ball), runs_scored(0), balls_faced(0),
      runs_conceded(0), balls_bowled(0), wickets_taken(0), is_out(false), is_selected(false), overs_bowled(0) {}

void PlayerProfile::resetMatchStats() {
    runs_scored = balls_faced = runs_conceded = balls_bowled = wickets_taken = 0;
    is_out = false;
    is_selected = false;
    overs_bowled = 0;
}

double PlayerProfile::getStrikeRate() const {
    return balls_faced ? (static_cast<double>(runs_scored) / balls_faced) * 100.0 : 0.0;
}

double PlayerProfile::getEconomyRate() const {
    return balls_bowled ? (static_cast<double>(runs_conceded) / (balls_bowled / 6.0)) : 0.0;
}

void PlayerProfile::updateBattingSkill() {
    double sr = getStrikeRate();
    int change = (sr >= 150) ? 5 : (sr >= 120) ? 3 : (sr >= 100) ? 1 : (sr >= 80) ? 0 : (sr >= 50) ? -1 : -3;
    bat_skill = std::max(0, std::min(100, bat_skill + change));
    if (balls_faced) std::cout << name << " - Strike Rate: " << std::fixed << std::setprecision(2) << sr << "%, Bat Skill: " << bat_skill << " (" << (change >= 0 ? "+" : "") << change << ")\n";
}

void PlayerProfile::updateBowlingSkill() {
    double econ = getEconomyRate();
    int change = (econ <= 4) ? 5 : (econ <= 6) ? 3 : (econ <= 8) ? 1 : (econ <= 10) ? 0 : (econ <= 12) ? -1 : -3;
    ball_skill = std::max(0, std::min(100, ball_skill + change));
    if (balls_bowled) std::cout << name << " - Economy: " << std::fixed << std::setprecision(2) << econ << ", Bowl Skill: " << ball_skill << " (" << (change >= 0 ? "+" : "") << change << ")\n";
}