#include <cstdlib>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>
#include <iostream>
#include <string>
#include "./headers/helpers.hpp"
#include "./headers/game_globals.hpp"



#include "./implement/grid_components.cpp"

using namespace ftxui;
using namespace std::chrono_literals;


class UltragridGameManagerMultiplayer {
    public:
        // Core game functions
        void StartGame();
        int* ActiveSmallgrid();
        int& gamestatus; //gamestatus 0 = incomplete/abort; 1 = X win; 2 = O win; 3 = Tie;

        Component renderer;

        // UI
        ftxui::Component GetUI();

        LargeGrid grid;


        UltragridGameManagerMultiplayer(ftxui::ScreenInteractive& s,int& gs) : screen(s), gamestatus(gs), grid(s) {
            grid.SetUpdateCallback([this]{  // Use [this] to capture the class instance
                this->UpdateUI();  // Call UpdateUI() directly on this instance
            });
            variables::currentPlayer = 1;
            SetupUI();
        }


    private:
        // Game state
        ftxui::ScreenInteractive& screen;

        Component gridcmp;
        Component gridholder;



        Component options;
        Component main_container;

        // UI Components
        ftxui::Component mainContainer;
        ftxui::Component gridComponent;
        ftxui::Component infoPanel;

        // UI setup
        void SetupUI();
        void UpdateUI();
    };


void UltragridGameManagerMultiplayer::SetupUI() {
    options = Container::Vertical({
        Button("EXIT", screen.ExitLoopClosure()),
    });

    gridcmp = grid.getComponent();
    gridholder = Container::Vertical({gridcmp});

    main_container = Container::Horizontal({
        gridholder,
        Renderer([&]{return separatorEmpty(); }),
        options,
    });

    renderer = Renderer(main_container, [&] {
        return vbox({
            text("Tic-Tac-Toe") | bold | center,
            separator(),
            vbox({
                filler(),
                main_container->Render() | hcenter,
                filler(),
            }) |flex | border,
        }) | flex_grow;
    });
}


//MAIN GAME LOGIC
void UltragridGameManagerMultiplayer::UpdateUI() {
    int enabled[1] = {0}; //put 0 in buttonoptions disabled
    //std::cout << "Updating UI..." << std::endl;

    int grid4d[3][3][3][3];
    grid.get4DArray(grid4d);

    bool gridUpdated = false;

    // Check and update small grids
    for (int gx = 0; gx < 3; gx++) {
        for (int gy = 0; gy < 3; gy++) {
            int board[3][3];
            for (int x = 0; x < 3; x++){
                for (int y = 0; y < 3; y++){
                    board[x][y] = grid4d[gx][gy][x][y];
                }
            }

            int result = checkBoard(board);
            if (result == 1 || result == 2 || result == 3) {
                grid.setbigicon_big(gx,gy,result);
                gridUpdated = true;
            }
        }
    }

    // Check for win in the larger grid
    int bigBoard[3][3];
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            bigBoard[x][y] = grid.getValue_big(x, y);
        }
    }

    int finalResult = checkBoard(bigBoard);
    if (finalResult == 1 || finalResult == 2 || finalResult == 3) {
        gamestatus = finalResult;
        screen.ExitLoopClosure()();
    }


    // Set valid grids based on last move
    if (variables::lastmove[0] != -1 && variables::lastmove[1] != -1) {
        int nextGrid_x = variables::lastmove[0];
        int nextGrid_y = variables::lastmove[1];

        // Check if the target grid is already won
        if (grid.getValue_big(nextGrid_x, nextGrid_y) == 0) {
            // Grid is still playable - enable only this grid
            grid.set_valid_grid(nextGrid_x, nextGrid_y);
        } else {
            // Enable all unfinished grids if target grid is completed
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid.getValue_big(i, j) == 0) {
                        grid.setoptions(i, j, enabled);
                    }
                }
            }
        }
    }

    // Update UI if needed
    if (gridUpdated) {
        gridcmp->Detach();
        gridcmp = grid.makeGridComponent();
        gridholder->Add(gridcmp);
        std::cout << "UI...UPDATED" << std::endl;
    }

    // Switch players
    variables::currentPlayer = (variables::currentPlayer == 1) ? 2 : 1;
}



void ultragrid_start_multiplayer() {

    auto screen = ScreenInteractive::Fullscreen();
    int game_status = 0;                             //gamestatus 0 = incomplete/abort; 1 = X win; 2 = O win; 3 = Tie;
    UltragridGameManagerMultiplayer game(screen,game_status);

    screen.Loop(game.renderer);

    std::cout << game_status << std::endl;

    int grid4d[3][3][3][3];  // Declare the array first
    game.grid.get4DArray(grid4d);  // Pass the array to be filled
    largegrid_to_cout(grid4d);
}
