#include "headers/splashscreen.hpp"
#include "headers/startmenu.hpp"
#include "headers/ultragrid.hpp"
#include <iostream>

int main() {

    //RenderStartScreen();

    auto gamemodeselection = StartupUI();

    //std::cout << gamemodeselection << std::endl;
    // 0    unintialised
    // 11   single player ultragrid
    // 12   multiplayer ultragrid
    //


    switch (gamemodeselection) {
        case 12:
            ultragrid_start_multiplayer();
        default:
            std::cout<<"Soon™"<<std::endl;

    }



    return 0;
}
