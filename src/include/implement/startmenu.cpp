#include "../headers/startmenu.hpp"
#include "../headers/helpers.hpp"
#include  "../headers/art.hpp"



using namespace ftxui;
#include <chrono>

#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include <cstdlib> // For system()


#include "ftxui/component/animation.hpp"       // for BackOut, Duration
#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for MenuOption, UnderlineOption
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red

#include "ftxui/dom/node.hpp"      // for Render
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>


using namespace ftxui;
using namespace std::chrono_literals;



//create the main ui for the startup menu
int StartupUI() {
    int shift = 0;
    auto screen = ScreenInteractive::Fullscreen();
    std::string displayedart = art;
    int return_mode = 0;



    // MENU _-----------------------------------------------------------
    int menu_selected = 1;

    const std::vector<std::string> menu_entries = {
        "NxN",

        "UltraGrid",

        "Settings",
    };

    auto option = MenuOption().HorizontalAnimated();
    option.underline.SetAnimationDuration(200ms);
    option.underline.color_active = Color::Red;
    option.underline.color_inactive = Color::Blue;
    option.underline.follower_delay = 50ms;
    option.on_enter = screen.ExitLoopClosure();

    option.elements_prefix = [] { return filler(); };
    option.elements_postfix = [] { return filler(); };
    option.elements_infix = [] { return separator(); };
    option.focused_entry = 1;
    option.entries_option.transform = [](const EntryState& state) {
        auto element = text(state.label);
        if (state.focused) {
            element = element | hcenter|flex_grow | inverted;
        }
        else{
            element = element| hcenter |flex_grow | size(WIDTH, EQUAL, 15) ;
        }
        return element;
    };

    auto menu = Menu(&menu_entries, &menu_selected, option);




    // -- Button -----------------------------------------------------------------


    std::string button_label = "Quit";
    //std::function<void()> on_button_clicked_;
    auto button = Button(&button_label, screen.ExitLoopClosure());




    // ultragridstart ----------------------------------------------------------------
    ButtonOption buttonoptions = ButtonOption::Animated();
    buttonoptions.animated_colors.background.Set(Color::Grey11, Color::White);


    buttonoptions.transform = [&shift](const EntryState& s) {
        auto element = text(s.label)| hcenter |vcenter;

        if (s.focused) {
            element = element |

            color(LinearGradient()
            .Angle(45+shift)
            .Stop(Color::DeepPink1)
            .Stop(Color::DeepSkyBlue1));
        }
        return element;
    };

    auto start_multiplayer = Button("Start Multiplayer", [&]{return_mode = 12; screen.ExitLoopClosure()();}, buttonoptions)
    | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 3) | hcenter |flex_grow;

    auto start_singleplayer = Button("Start Singleplayer", [&]{return_mode = 11; screen.ExitLoopClosure()();}, buttonoptions)
    | size(WIDTH, EQUAL, 20) | size(HEIGHT, EQUAL, 3) | hcenter |flex_grow;



    auto start_buttons = Container::Horizontal({
    start_singleplayer,
    start_multiplayer
    }) | flex_grow;

    auto start_container = Container::Vertical({
    start_buttons,
    });

    auto lorel_ipsum =
    Renderer(start_container,[&] {
        return vbox({
            paragraphAlignCenter("Ultragrid - Play on a 3×3 grid of smaller boards, where each move determines the next board your opponent must play on"),
            text(""),
            paragraphAlignCenter(" Win by claiming three boards in a line "),
            filler(),
            start_container->Render(),
            filler(),

        }) | border;
    });


    // -- Layout
    // -----------------------------------------------------------------
    auto tab_content = Container::Tab(
        {
            button,
            lorel_ipsum,
        },
        &menu_selected);



    auto main_container = Container::Vertical({
        menu,
        tab_content,
    }) ;


    auto main_renderer = Renderer(main_container, [&] {


        return vbox({
            vbox({
            nonWrappingParagraph(displayedart) | hcenter,
            separatorEmpty(),
            nonWrappingParagraph(art2) | hcenter,
            separatorEmpty(),
            menu->Render(),
            })
            | color(LinearGradient()
            .Angle(45+shift)
            .Stop(Color::DeepPink1)
            .Stop(Color::DeepSkyBlue1)),

            tab_content->Render() | flex,


        }) | size(HEIGHT, EQUAL, 25) | size(WIDTH, EQUAL, 90) | hcenter | vcenter;


        // return vbox({
        //     filler(),
        //     hbox({
        //         filler(),
        //         //menucontent,
        //         filler(),
        //     }),
        //     filler(),
        // });


    });


// -- Refresh UI -----------------------------------------------------------

    std::atomic<bool> refresh_ui_continue = true;

    std::thread refresh_ui([&] {
      while (refresh_ui_continue) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(0.05s);
        // The |shift| variable belong to the main thread. `screen.Post(task)`
        // will execute the update on the thread where |screen| lives (e.g. the
        // main thread). Using `screen.Post(task)` is threadsafe.
        screen.Post([&] { shift++; });
        screen.Post([&] {
            if (menu_selected == 1) {
                displayedart = art;
            } else if (menu_selected == 0) {
                displayedart = art_nxn;
            } else if (menu_selected == 2) {
                displayedart = art_settings;
            }

        });
        // After updating the state, request a new frame to be drawn. This is done
        // by simulating a new "custom" event to be handled.
        screen.Post(Event::Custom);
      }
    });



// -- Loop -----------------------------------------------------------

    screen.Loop(main_renderer);
    refresh_ui_continue = false;
    refresh_ui.join();

    return return_mode;                //nxn 0 ultragrid 1 settings 2
}
