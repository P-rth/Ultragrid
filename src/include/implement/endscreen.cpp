#include "../headers/endscreen.hpp"
#include "../headers/helpers.hpp"
#include "../headers/art.hpp"
#include "../headers/startmenu.hpp"
#include "./headers/ultragrid.hpp"

#include <iostream>
#include <atomic>
#include <thread>
#include <functional>
#include <string>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "ftxui/component/animation.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/dom/node.hpp"
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/terminal.hpp>

using namespace ftxui;
using namespace std::chrono_literals;

void EndScreenUI(int gameMode, int winner) {
    int shift = 0;
    auto screen = ScreenInteractive::Fullscreen();

    // Winner message and ASCII art
    std::string winner_message;
    std::string winner_art;
    if (winner == 1) {
        winner_message = "Player 1 Wins!";
        winner_art = bigx;
    } else if (winner == 2) {
        winner_message = "Player 2 Wins!";
        winner_art = bigo;
    } else if (winner == 3) {
        winner_message = "It's a Draw!";
        winner_art = bigtie;
    } else {
        winner_message = "Game Stopped";
        winner_art = art;
    }

    // Game mode message
    std::string mode_message;
    if (gameMode == 11) {
        mode_message = "UltraGrid - Singleplayer";
    } else if (gameMode == 12) {
        mode_message = "UltraGrid - Multiplayer";
    } else {
        mode_message = "NxN Mode";
    }

    // Button styling
    ButtonOption button_option = ButtonOption::Animated();
    button_option.animated_colors.background.Set(Color::Grey11, Color::NavajoWhite1);

    button_option.transform = [&shift](const EntryState& s) {
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

    // Buttons
    auto buttons = Container::Horizontal({
        Button("Play Again", [&] {
            screen.ExitLoopClosure()();
            if (gameMode == 11) {
                ultragrid_start_singleplayer();
            } else if (gameMode == 12) {
                ultragrid_start_multiplayer();
            }
        }, button_option) | size(HEIGHT, EQUAL, 3) |size(WIDTH, EQUAL, 15) |hcenter,

        Button("Main Menu", [&] {
            screen.ExitLoopClosure()();
            auto gamemodeselection = StartupUI();
            if (gamemodeselection == 11) {
                ultragrid_start_singleplayer();
            } else if (gamemodeselection == 12) {
                ultragrid_start_multiplayer();
            }
        }, button_option) | size(HEIGHT, EQUAL, 3) | size(WIDTH, EQUAL, 15) |hcenter,


        Button("Quit", screen.ExitLoopClosure(), button_option) | size(HEIGHT, EQUAL, 3) | size(WIDTH, EQUAL, 15) |hcenter,
    });

    // Container setup
    auto container = Container::Vertical({
        buttons,
    });

    // Main renderer with animated elements
    auto renderer = Renderer(container, [&] {


        std::vector<Element> elements;

        // Animated art with gradient
        elements.push_back(filler());
        elements.push_back(
            nonWrappingParagraph(winner_art) |
            color(LinearGradient()
                .Angle(shift)
                .Stop(Color::DeepPink1)
                .Stop(Color::Green3Bis)
                .Stop(Color::DeepSkyBlue1))
        );
        elements.push_back(separatorEmpty());
        elements.push_back(text(winner_message) | bold | hcenter);
        elements.push_back(separatorEmpty());
        elements.push_back(text(mode_message) | hcenter);
        elements.push_back(separatorEmpty());
        elements.push_back(buttons->Render() |hcenter);
        elements.push_back(filler());

        return vbox(std::move(elements)) |
               size(HEIGHT, EQUAL, 20) |
               size(WIDTH, EQUAL, 80) |
               border |
               center;

    });

    // Animation thread
    std::atomic<bool> refresh_ui_continue = true;
    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
            std::this_thread::sleep_for(0.05s);
            screen.Post([&] { shift += 2; });
            screen.Post(Event::Custom);
        }
    });

    // Main loop
    screen.Loop(renderer);

    // Cleanup
    refresh_ui_continue = false;
    refresh_ui.join();
}
