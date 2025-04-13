#include "../headers/game_globals.hpp"

namespace variables {
    int currentPlayer = 1;
    int lastmove[2] = {-1,-1};
    int difficulty = 1;
}

namespace callbacks {
    std::function<void()> onUpdate;
}
