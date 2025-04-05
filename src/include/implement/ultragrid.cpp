#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>
#include <string>
#include "./headers/helpers.hpp"


#include "./implement/grid_components.cpp"

using namespace ftxui;
using namespace std::chrono_literals;



void ultragrid_start_singleplayer() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    LargeGrid gridcmp;                             //gridcomponent
    Component grid = gridcmp.getComponent();
    Component gridholder = Container::Vertical({grid});


    Component options = Container::Vertical({
        Button("EXIT", screen.ExitLoopClosure()),

        Button("focus", [&gridcmp]() {
            std::cout << "focus" << std::endl;
            gridcmp.takefocus_big(0,0,0,0);
        }),

        Button("bigicon", [&gridcmp,&grid, &gridholder]() {
            std::cout << "bigicon" << std::endl;
            gridcmp.setbigicon_big(2,2,2);
            // Update the container
            grid->Detach();
            grid = gridcmp.makeGridComponent();              //update the main big grid
            gridholder->Add(grid);
        }),

        Button("disable", [&gridcmp,&grid, &gridholder]() {
            std::cout << "disable" << std::endl;
            int op[1] = {0};
            int op_inv[1] = {1};
            gridcmp.setoptions_invert(0, 0, op, op_inv);

        })

    });



    // Create a vertical container for all components
    auto main_container = Container::Horizontal({
        gridholder | borderDouble,
        options,

    });


    auto renderer = Renderer(main_container, [&] {
        return vbox({
            text("Tic-Tac-Toe") | bold | center,
            separator(),
            main_container->Render() | flex,
        });
    });



    screen.Loop(renderer);

    int grid4d[3][3][3][3];  // Declare the array first
    gridcmp.get4DArray(grid4d);  // Pass the array to be filled
    largegrid_to_cout(grid4d);
}
