// Compiler default libs
#include <cstdlib>
#include <ftxui/dom/linear_gradient.hpp>
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
#include "./headers/ultragrid.hpp"
#include "./headers/ultragrid_ai.hpp"

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

        void SetupUI(bool is_singleplayer);

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

void UltragridGameManagerMultiplayer::SetupUI(bool is_sp) {
    grid.SetUpdateCallback([this]{
        this->UpdateUI();
    });

    variables::currentPlayer = 1;

    // Store is_sp as class member if needed
    bool is_singleplayer = is_sp;

    // Create buttons
    auto exit_button = Button("Exit", screen.ExitLoopClosure());
    auto restart_button = Button("Restart", [this, is_singleplayer] {
        screen.ExitLoopClosure()();
        if (is_singleplayer) {
            ultragrid_start_singleplayer();
        } else {
            ultragrid_start_multiplayer();
        }
    });

    auto diffculty = Slider("", &variables::difficulty, 0, 2, 1);

    // Create a title component
    auto title = Renderer([] {
        return text("Ultragrid") | bold | color(LinearGradient().Stop(Color::DeepPink1).Stop(Color::DeepSkyBlue1));
    });

    // Create player info component
    auto player_info = Renderer([this, is_singleplayer,diffculty] {
        if(is_singleplayer){
            auto a = nonWrappingParagraph(happy);
            return vbox({
                text("Singleplayer ") | color(Color::Green),
                filler(),
                a,
                filler(),
            }) | flex;
        }
        else{
            auto a = nonWrappingParagraph("hi");
            if(variables::currentPlayer == 1 ){
                a = nonWrappingParagraph(smallx) | color(Color::Red);
            }
            else {
                a = nonWrappingParagraph(smallo) | color(Color::Blue);
            }
            return vbox({
                text("Current Player: ") | color(Color::Green),
                filler(),
                a,
                filler()
            }) | flex;
        }
    });

    gridcmp = grid.getComponent();
    gridholder = Container::Vertical({gridcmp});

    // Create components vector for vertical container
    Components vertical_components = {
        title,
        Renderer([]{return separator();}),
        player_info,
        diffculty,
        Renderer([]{return filler();}),
        exit_button,
        restart_button,
    };

    // Create components vector for horizontal container
    Components horizontal_components = {
        gridholder,
        Renderer([]{return separatorEmpty();}),
        Renderer([]{return separatorEmpty();}),
        Container::Vertical(vertical_components) | size(WIDTH, LESS_THAN, 30)
    };

    main_container = Container::Horizontal(horizontal_components);

    renderer = Renderer(main_container, [&] {
        return vbox({
            filler(),
            main_container->Render() | hcenter,
            filler(),
        }) | flex;
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
    game.SetupUI(false);  //is_singleplayer = false

    screen.Loop(game.renderer);

    std::cout << game_status << std::endl;

}

class UltragridGameManagerSingleplayer : public UltragridGameManagerMultiplayer {
protected:

public:
    UltragridGameManagerSingleplayer(ftxui::ScreenInteractive& s, int& gs)
        : UltragridGameManagerMultiplayer(s, gs) {
    }

    bool MakeAIMove();

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

bool UltragridGameManagerSingleplayer::MakeAIMove() {
    return UltragridAI::MakeMove(variables::difficulty, grid);
}

void ultragrid_start_singleplayer() {

    auto screen = ScreenInteractive::Fullscreen();
    int game_status = 0;                             //gamestatus 0 = incomplete/abort; 1 = X win; 2 = O win; 3 = Tie;
    UltragridGameManagerSingleplayer game(screen,game_status);

    game.SetupUI(true);  //is_singleplayer = true

    screen.Loop(game.renderer);

    std::cout << game_status << std::endl;

    int grid4d[3][3][3][3];  // Declare the array first
    game.grid.get4DArray(grid4d);  // Pass the array to be filled
    largegrid_to_cout(grid4d);

}
