#include "game_mechanics.h"
#include "utils.h" // For random number generators
#include <iostream>
#include <algorithm>

// --- Batting Implementation ---
double Batting::applyBowlingModifier(double limit, BowlingStyle style, bool high) {
    double mod = (style == FAST_BOWLING) ? (high ? 0.1 : -0.1) : (style == SLOW_BOWLING) ? (high ? -0.1 : 0.1) : 0.0;
    return std::max(0.0, std::min(1.0, limit + mod));
}

int Batting::defensiveShot(BowlingStyle style) {
    choice = dis(gen);
    double dot = applyBowlingModifier(0.7, style, false), single = applyBowlingModifier(0.9, style, false);
    if (choice <= dot) { std::cout << "Defensive - 0 runs\n"; return 0; }
    else if (choice <= single) { std::cout << "Defensive - 1 run\n"; return 1; }
    else { std::cout << "Defensive - 2 runs\n"; return 2; }
}

int Batting::normalShot(BowlingStyle style) {
    choice = dis(gen);
    double single = applyBowlingModifier(0.1, style, false), two = applyBowlingModifier(0.5, style, false),
           three = applyBowlingModifier(0.7, style, false), four = applyBowlingModifier(0.9, style, true);
    if (choice <= single) { std::cout << "Normal - 1 run\n"; return 1; }
    else if (choice <= two) { std::cout << "Normal - 2 runs\n"; return 2; }
    else if (choice <= three) { std::cout << "Normal - 3 runs\n"; return 3; }
    else if (choice <= four) { std::cout << "Normal - FOUR!\n"; return 4; }
    else { std::cout << "Normal - OUT!\n"; return -1; }
}

int Batting::loftedShot(BowlingStyle style) {
    choice = dis(gen);
    double three = applyBowlingModifier(0.1, style, false), four = applyBowlingModifier(0.3, style, true),
           six = applyBowlingModifier(0.7, style, true);
    if (choice <= three) { std::cout << "Lofted - 3 runs\n"; return 3; }
    else if (choice <= four) { std::cout << "Lofted - FOUR!\n"; return 4; }
    else if (choice <= six) { std::cout << "Lofted - SIX!\n"; return 6; }
    else { std::cout << "Lofted - OUT!\n"; return -1; }
}

// --- Bowling Implementation ---
bool Bowling::fastBowling(int& extras) { choice = dis(gen); style = FAST_BOWLING; if (choice <= 0.1) { std::cout << "Fast - No Ball!\n"; extras += 1; return true; } std::cout << "Fast ball\n"; return false; }
bool Bowling::slowBowling(int& extras) { choice = dis(gen); style = SLOW_BOWLING; if (choice <= 0.05) { std::cout << "Slow - No Ball!\n"; extras += 1; return true; } std::cout << "Slow ball\n"; return false; }
bool Bowling::normalBowling(int& extras) { choice = dis(gen); style = NORMAL_BOWLING; if (choice <= 0.07) { std::cout << "Normal - No Ball!\n"; extras += 1; return true; } std::cout << "Normal ball\n"; return false; }
BowlingStyle Bowling::getCurrentStyle() const { return style; }