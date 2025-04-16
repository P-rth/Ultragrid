#include "headers/splashscreen.hpp"
#include "headers/startmenu.hpp"
#include "headers/ultragrid.hpp"
#include <iostream>
#include <cstring>

void help(){
    std::cout << "Invalid arguments" << std::endl;
        std::cout << "to enable splash screen use -" << std::endl;
        std::cout << "./ttt splash" << std::endl;
        std::cout << "terminal text effects should be installed - \n https://github.com/ChrisBuilds/terminaltexteffects" << std::endl;
        exit(1);
}



int main(int argc, char* argv[]) {
    if(argc == 2) {  // Check for exactly one argument
        if(strcmp(argv[1], "splash") == 0) {  // Check argv[1], not argv[0]
            RenderStartScreen();
        }
        else {
            help();
        }
    }


    auto gamemodeselection = StartupUI();

    //std::cout << gamemodeselection << std::endl;
    // 0    unintialised
    // 11   single player ultragrid
    // 12   multiplayer ultragrid
    //


    switch (gamemodeselection) {
        case 11:
            ultragrid_start_singleplayer();
            break;
        case 12:
            ultragrid_start_multiplayer();
            break;
        default:
            std::cout<<"Soon™"<<std::endl;
            break;

    }



    return 0;
}
