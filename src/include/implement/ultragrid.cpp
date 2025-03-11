#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/string.hpp>
#include <string>
#include "./headers/helpers.hpp"


#include "./implement/grid_components.cpp"

using namespace ftxui;
using namespace std::chrono_literals;



void ultragrid_start_singleplayer() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    LargeGrid grid;

    auto container = grid.makeGridComponent();

    container -> Add(Button("EXIT",screen.ExitLoopClosure()));


    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Tic-Tac-Toe") | bold | center,
            separator(),
            container->Render() | border|hcenter|vcenter,

        });
    });

    screen.Loop(renderer);

    largegrid_val grid4d = grid.get4DArray();
    largegrid_to_cout(grid4d);



    std::cout << "hello "<< std::endl;

}
