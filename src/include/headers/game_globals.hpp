
#pragma once
#include <functional>

namespace variables {
    extern int currentPlayer;
    extern int lastmove[2];
}

namespace callbacks {
    extern std::function<void()> onUpdate;
}
