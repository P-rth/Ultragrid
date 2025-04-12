// Compiler default libs
#include <cstdlib>
#include <iostream>
#include <string>
#include <random>

// FTXUI libs
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/string.hpp>

// Game libs
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

        void SetupUI();


        UltragridGameManagerMultiplayer(ftxui::ScreenInteractive& s,int& gs) : screen(s), gamestatus(gs), grid(s) {
        }


    protected:
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
        virtual void UpdateUI();

        // New helper functions
        bool CheckAndUpdateSmallGrids() {
            bool gridUpdated = false;
            int grid4d[3][3][3][3];
            grid.get4DArray(grid4d);

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
                        grid.setbigicon_big(gx, gy, result);
                        gridUpdated = true;
                    }
                }
            }
            return gridUpdated;
        }

        bool CheckGameEnd() {
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
                return true;
            }
            return false;
        }

        void UpdateValidGrids() {
            int enabled[1] = {0};
            if (variables::lastmove[0] != -1 && variables::lastmove[1] != -1) {
                int nextGrid_x = variables::lastmove[0];
                int nextGrid_y = variables::lastmove[1];

                if (grid.getValue_big(nextGrid_x, nextGrid_y) == 0) {
                    grid.set_valid_grid(nextGrid_x, nextGrid_y);
                } else {
                    for (int i = 0; i < 3; i++) {
                        for (int j = 0; j < 3; j++) {
                            if (grid.getValue_big(i, j) == 0) {
                                grid.setoptions(i, j, enabled);
                            }
                        }
                    }
                }
            }
        }

        void RefreshGridComponent(bool gridUpdated) {
            if (gridUpdated) {
                screen.Post([&]{
                    gridcmp->Detach();
                    gridcmp = grid.makeGridComponent();
                    gridholder->Add(gridcmp);
                });
            }
        }

        void SwitchPlayer() {
            variables::currentPlayer = (variables::currentPlayer == 1) ? 2 : 1;
        }
    };


void UltragridGameManagerMultiplayer::SetupUI() {
    grid.SetUpdateCallback([this]{  // Use [this] to capture the class instance
        this->UpdateUI();  // Call UpdateUI() directly on this instance
    });

    variables::currentPlayer = 1;

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
    bool gridUpdated = CheckAndUpdateSmallGrids();

    if (CheckGameEnd()) {
        return;
    }

    UpdateValidGrids();
    RefreshGridComponent(gridUpdated);
    SwitchPlayer();
}



void ultragrid_start_multiplayer() {

    auto screen = ScreenInteractive::Fullscreen();
    int game_status = 0;                             //gamestatus 0 = incomplete/abort; 1 = X win; 2 = O win; 3 = Tie;
    UltragridGameManagerMultiplayer game(screen,game_status);
    game.SetupUI();

    screen.Loop(game.renderer);

    std::cout << game_status << std::endl;


}




class UltragridGameManagerSingleplayer : public UltragridGameManagerMultiplayer {
protected:
    bool MakeAIMove() {
        int grid4d[3][3][3][3];
        grid.get4DArray(grid4d);
        bool moveMade = false;
        int nextGrid_x = variables::lastmove[0];
        int nextGrid_y = variables::lastmove[1];

        // Try specific grid first
        if (nextGrid_x != -1 && nextGrid_y != -1 &&
            grid.getValue_big(nextGrid_x, nextGrid_y) == 0) {
            for (int x = 0; x < 3 && !moveMade; x++) {
                for (int y = 0; y < 3 && !moveMade; y++) {
                    if (grid4d[nextGrid_x][nextGrid_y][x][y] == 0) {
                        grid.makemove(nextGrid_x, nextGrid_y, x, y);
                        variables::lastmove[0] = x;
                        variables::lastmove[1] = y;
                        moveMade = true;
                    }
                }
            }
        }

        // Try any available grid if needed
        if (!moveMade) {
            for (int gx = 0; gx < 3 && !moveMade; gx++) {
                for (int gy = 0; gy < 3 && !moveMade; gy++) {
                    if (grid.getValue_big(gx, gy) == 0) {
                        for (int x = 0; x < 3 && !moveMade; x++) {
                            for (int y = 0; y < 3 && !moveMade; y++) {
                                if (grid4d[gx][gy][x][y] == 0) {
                                    grid.makemove(gx, gy, x, y);
                                    variables::lastmove[0] = x;
                                    variables::lastmove[1] = y;
                                    moveMade = true;
                                }
                            }
                        }
                    }
                }
            }
        }
        return moveMade;
    }

public:
    UltragridGameManagerSingleplayer(ftxui::ScreenInteractive& s, int& gs)
        : UltragridGameManagerMultiplayer(s, gs) {
    }

    void UpdateUI() override {
        bool gridUpdated = CheckAndUpdateSmallGrids();

        if (CheckGameEnd()) {
            return;
        }

        SwitchPlayer();

        if (variables::currentPlayer == 2) {
            MakeAIMove();
            this->UpdateUI();
            return;
        }

        UpdateValidGrids();
        RefreshGridComponent(gridUpdated);

    }
};

void ultragrid_start_singleplayer() {

    auto screen = ScreenInteractive::Fullscreen();
    int game_status = 0;                             //gamestatus 0 = incomplete/abort; 1 = X win; 2 = O win; 3 = Tie;
    UltragridGameManagerSingleplayer game(screen,game_status);
    game.SetupUI();

    screen.Loop(game.renderer);

    std::cout << game_status << std::endl;

    int grid4d[3][3][3][3];  // Declare the array first
    game.grid.get4DArray(grid4d);  // Pass the array to be filled
    largegrid_to_cout(grid4d);

}
