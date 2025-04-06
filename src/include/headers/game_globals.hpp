
#pragma once
#include <functional>

namespace variables {
    extern int currentPlayer;
}

namespace callbacks {
    extern std::function<void()> onUpdate;
}
