#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>



#include "./implement/grid_components.cpp"

using namespace ftxui;
using namespace std::chrono_literals;



void ultragrid_start_singleplayer() {
    using namespace ftxui;
    auto screen = ScreenInteractive::Fullscreen();

    LargeGrid grid;

    auto container = grid.makeGridComponent();

    auto renderer = Renderer(container, [&] {
        return vbox({
            text("Tic-Tac-Toe") | bold | center,
            separator(),
            container->Render() | border|hcenter|vcenter,
        });
    });

    screen.Loop(renderer);
}
