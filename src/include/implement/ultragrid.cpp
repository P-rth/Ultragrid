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

    LargeGrid gridcmp;
    Component grid = gridcmp.getComponent();
    Component gridholder = Container::Vertical({grid});


    // Create a vertical container for all components
    auto main_container = Container::Vertical({
        gridholder,
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
            grid = gridcmp.getComponent();              //update the main big grid
            gridholder->Add(grid);
        }),

        Button("disable", [&gridcmp,&grid, &gridholder]() {
            std::cout << "disable" << std::endl;
            gridcmp.setoptions(0, 0, std::vector<int> {1});
            // Update the container
            grid->Detach();
            grid = gridcmp.getComponent();              //update the main big grid
            gridholder->Add(grid);
        })

    });

    auto renderer = Renderer(main_container, [&] {
        return vbox({
            text("Tic-Tac-Toe") | bold | center,
            separator(),
            main_container->Render() | border | hcenter | vcenter,
        });
    });

    screen.Loop(renderer);

    largegrid_val grid4d = gridcmp.get4DArray();
    largegrid_to_cout(grid4d);
}
