#include "headers/splashscreen.hpp"
#include "headers/startmenu.hpp"
#include <iostream>
//#include "main.h"

int main() {

    RenderStartScreen();

    auto gamemodeselection = StartupUI();

    std::cout << gamemodeselection << std::endl;
    // 0    unintialised
    // 11   single player ultragrid
    // 12   multiplayer ultragrid

    return 0;
}
