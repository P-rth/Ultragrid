#include "headers/splashscreen.hpp"
#include "headers/startmenu.hpp"
#include "headers/ultragrid.hpp"
#include <iostream>
//#include "main.h"

int main() {

    //RenderStartScreen();

    //auto gamemodeselection = StartupUI();

    //std::cout << gamemodeselection << std::endl;
    // 0    unintialised
    // 11   single player ultragrid
    // 12   multiplayer ultragrid

    supergrid_start_singleplayer();


    return 0;
}
