#pragma once

enum BowlingStyle { NORMAL_BOWLING, FAST_BOWLING, SLOW_BOWLING };

class Batting {
    double choice;
    double applyBowlingModifier(double limit, BowlingStyle style, bool high);
public:
    int defensiveShot(BowlingStyle style);
    int normalShot(BowlingStyle style);
    int loftedShot(BowlingStyle style);
};

class Bowling {
    double choice;
    BowlingStyle style = NORMAL_BOWLING;
public:
    bool fastBowling(int& extras);
    bool slowBowling(int& extras);
    bool normalBowling(int& extras);
    BowlingStyle getCurrentStyle() const;
};