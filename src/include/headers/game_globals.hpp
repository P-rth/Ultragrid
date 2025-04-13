
#pragma once
#include <functional>

namespace variables {
    extern int currentPlayer;
    extern int lastmove[2];
    extern int difficulty;
}

namespace callbacks {
    extern std::function<void()> onUpdate;
}

enum class CustomEvent {
    RefreshComponent,
    BurronCallback,
};
