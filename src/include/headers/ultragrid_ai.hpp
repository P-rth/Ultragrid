#pragma once

#include "game_globals.hpp"
#include "../implement/grid_components.cpp"
#include <vector>
#include <tuple>

class UltragridAI {
public:
    static bool MakeMove(int difficulty, LargeGrid& grid);
private:
    static bool MakeBasicMove(LargeGrid& grid);
    static bool MakeRandomMove(LargeGrid& grid);
    static bool MakeSmartMove(LargeGrid& grid);
};
