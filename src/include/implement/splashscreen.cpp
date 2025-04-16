#include "../headers/splashscreen.hpp"
#include "../headers/helpers.hpp"

#include  "../headers/art.hpp"


using namespace ftxui;
using namespace std::chrono_literals;

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <string>


#include <iostream>



void RenderStartScreen() {
    Dimensions dim = Terminal::Size();
    dim.dimy -= 1;
    auto start_screen = Screen::Create(dim);


    auto document1 = hbox({
        filler(),

        vbox({


            filler(),
            nonWrappingParagraph(art),
            text(" "),
            nonWrappingParagraph(art2),
            filler()

        }),

        filler(),

        text("v1")
    });

    ftxui::Render(start_screen, document1);
    std::string out_screen_string = start_screen.ToString();


    std::cout << std::flush;
    std::string command = "echo -e \"" + out_screen_string + "\" | tte scattered";
    system(command.c_str());
    std::cout << std::flush;
    std::cout << "Press Enter to continue";
    std::cin.get();
    std::cout << "\033[2J\033[1;1H";
}
