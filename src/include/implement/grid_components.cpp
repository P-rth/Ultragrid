#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red
#include "ftxui/dom/node.hpp"      // for Render
#include <cmath>
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include <iostream>
#include <ostream>

#include "ftxui/component/event.hpp"  // for Event definition
#include "./headers/helpers.hpp"
#include "./headers/art.hpp"


using namespace ftxui;
using namespace std::chrono_literals;

using TicTacToeButton_Options = int[2];
using TicTacToeButton_Options_Grid = int[3][3][2];


#include "../headers/game_globals.hpp"

class TicTacToeButton {
    private:
        int& gridValue;
        int row;
        int col;
        bool isHovered = false;
        bool isPressed = false;
        bool click_release_edge = false;
        TicTacToeButton_Options& options;
        ftxui::Box box_;
        ftxui::Component button;

        std::string getSymbolString(int value) const {
            switch(value) {
                case 1: return "X";
                case 2: return "O";
                default: return " ";
            }
        }

        ftxui::Component makeButton() {
            using namespace ftxui;

            auto buttonComponent = Button(" ", [this] {
                bool isdisabled = options[0] == 1;  // Check current disabled state
                if (!isdisabled && gridValue == 0) {
                    gridValue = variables::currentPlayer;
                    if (callbacks::onUpdate) {
                        callbacks::onUpdate();
                    }
                }
            }
            );

            buttonComponent |= ftxui::CatchEvent([this, buttonComponent](ftxui::Event event) {
                bool isdisabled = options[0] == 1;  // Check current disabled state
                const bool mouse_in_button = box_.Contain(event.mouse().x, event.mouse().y);

                if (isdisabled && mouse_in_button) {
                    return false;
                }

                if (mouse_in_button != isHovered) {
                    buttonComponent->TakeFocus();
                    isHovered = mouse_in_button;
                }

                if (event.mouse().button == Mouse::Left) {
                    if (event.mouse().motion == Mouse::Pressed) {
                        if (mouse_in_button) {
                            isPressed = true;
                            return true;
                        }
                        else {
                            isPressed = false;
                        }
                    }
                    else if (event.mouse().motion == Mouse::Released && mouse_in_button) {
                        if (isPressed) {
                            click_release_edge = true;
                            return true;
                        }
                    }
                    else if (event.mouse().motion == Mouse::Released) {
                        isPressed = false;
                    }
                }

                return false;
            });

            return Renderer(buttonComponent, [this, buttonComponent] {
                bool isdisabled = options[0] == 1;  // Check current disabled state

                auto content = text(getSymbolString(gridValue)) |
                            center |
                            size(WIDTH, EQUAL, 3)|
                            size(HEIGHT, EQUAL, 1);

                if (isdisabled) {
                    if (buttonComponent->Focused()) {
                        content = content | dim | bgcolor(ftxui::Color::Grey50);
                    } else {
                        content = content | dim | bgcolor(ftxui::Color::Grey30);
                    }
                }

                if (buttonComponent->Focused() && !isdisabled) {
                    if (click_release_edge && gridValue == 0) {
                        gridValue = variables::currentPlayer;
                        std::cout << "Player " << gridValue << " clicked!" << std::endl;
                        if (callbacks::onUpdate) {
                            callbacks::onUpdate();
                        }
                        if (gridValue == 1) {
                            content = content | bgcolor(ftxui::Color::Red);
                        }
                        else if (gridValue == 2) {
                            content = content | bgcolor(ftxui::Color::Blue);
                        }
                    }
                    else {
                        if (isPressed && gridValue == 0) {
                            content = content | bgcolor(ftxui::Color::OrangeRed1);
                        }
                        else if (gridValue == 0) {
                            content = content | bgcolor(ftxui::Color::GrayDark);
                        }
                        else if (gridValue == 1) {
                            content = content | color(ftxui::Color::Red);
                        }
                        else if (gridValue == 2) {
                            content = content | color(ftxui::Color::Blue);
                        }
                    }
                }

                click_release_edge = false;
                return content | reflect(box_);
            });
        }

    public:
        TicTacToeButton(int& gridRef, int r, int c, TicTacToeButton_Options& opts)
            : gridValue(gridRef), row(r), col(c), options(opts) {
            button = makeButton();
        }

        void TakeFocus_btn() {
            if (!button) {
                std::cout << "TicTacToeButton: Button component is null!" << std::endl;
                return;
            }
            button->TakeFocus();
        }

        ftxui::Component GetButton() const {
            return button;
        }

        int GetRow() const { return row; }
        int GetCol() const { return col; }
};

class SmallGrid {
    private:
        TicTacToeButton* buttons[3][3];
        int selected_x = 0;
        int selected_y = 0;
        int bigicon = 0;
        TicTacToeButton_Options* buttonOptions;
        ftxui::Component gridComponent;

    public:
        int grid[3][3];

        SmallGrid(TicTacToeButton_Options* values) : buttonOptions(values) {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    grid[i][j] = 0;
                    buttons[i][j] = new TicTacToeButton(grid[i][j], i, j, *buttonOptions);
                }
            }

            gridComponent = makeGridComponent();
        }

        ~SmallGrid() {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    delete buttons[i][j];
                }
            }
        }

        int (*getGridRef())[3] {
            return &grid[0];
        }

        void takefocus_sm_grid(int x = 1, int y = 1) {
            buttons[x][y]->TakeFocus_btn();
        }

        ftxui::Component getGridComponent() {
            return gridComponent;
        }

        void setbigicon(int bigicon_set) {
            this->bigicon = bigicon_set;
            gridComponent = makeGridComponent();
        }

    private:
        ftxui::Component makeGridComponent() {
            using namespace ftxui;
            Component container;

            if (bigicon == 0) {
                container = Container::Vertical({}, &selected_y);

                for (int i = 0; i < 3; i++) {
                    auto row = Container::Horizontal({}, &selected_x);
                    for (int j = 0; j < 3; j++) {
                        row->Add(buttons[i][j]->GetButton());

                        if (j < 2) {
                            row->Add(Renderer([]{
                                return separator();
                            }));
                        }
                    }

                    container->Add(row);

                    if (i < 2) {
                        container->Add(Renderer([]{
                            return separator();
                        }));
                    }
                }
            }
            else if (bigicon == 1) {
                container = Renderer([&]{ return nonWrappingParagraph(bigx); });
            }
            else if (bigicon == 2) {
                container = Renderer([&]{ return nonWrappingParagraph(bigx); });
            }

            return container;
        }

        std::string getSymbolString(int value) const {
            switch(value) {
                case 1: return "X";
                case 2: return "O";
                default: return " ";
            }
        }
};

class LargeGrid {
    private:

        SmallGrid* grids[3][3];
        int selected_x = 0;
        int selected_y = 0;
        Component mainComponent;
        int grids_val[3][3][3][3];
        TicTacToeButton_Options_Grid grid_options;


    public:
        LargeGrid() {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    grid_options[i][j][0] = 0;
                    grid_options[i][j][1] = 0;
                    grids[i][j] = new SmallGrid(&grid_options[i][j]);
                }
            }

            mainComponent = makeGridComponent();
        }

        ~LargeGrid() {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    delete grids[i][j];
                }
            }
        }

        int getValue(int bigRow, int bigCol, int smallRow, int smallCol) const {
            auto* smallGrid = grids[bigRow][bigCol];
            int (*gridRef)[3] = smallGrid->getGridRef();
            return gridRef[smallRow][smallCol];
        }



        void get4DArray(int (*out)[3][3][3]) const {
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    auto* smallGrid = grids[i][j];
                    if (!smallGrid) {
                        throw std::runtime_error("Null grid reference");
                    }

                    int (*gridRef)[3] = smallGrid->getGridRef();
                    for(int k = 0; k < 3; k++) {
                        for(int l = 0; l < 3; l++) {
                            out[i][j][k][l] = gridRef[k][l];
                        }
                    }
                }
            }
        }

        void setoptions(int bigRow, int bigCol, TicTacToeButton_Options op) {
            grid_options[bigRow][bigCol][0] = op[0];
            grid_options[bigRow][bigCol][1] = op[1];
        }

        void setoptions_invert(int bigRow, int bigCol, TicTacToeButton_Options op, TicTacToeButton_Options op_inv) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (i == bigRow && j == bigCol) {
                        grid_options[i][j][0] = op[0];
                        grid_options[i][j][1] = op[1];
                    }
                    else {
                        grid_options[i][j][0] = op_inv[0];
                        grid_options[i][j][1] = op_inv[1];
                    }
                }
            }
        }

        Component makeGridComponent() {
            using namespace ftxui;
            auto container = Container::Vertical({}, &selected_y);

            for(int i = 0; i < 3; i++) {
                auto row = Container::Horizontal({}, &selected_x);

                for(int j = 0; j < 3; j++) {
                    row->Add(grids[i][j]->getGridComponent());

                    if (j < 2) {
                        row->Add(Renderer([]{
                            return separatorDouble();
                        }));
                    }
                }

                container->Add(row);

                if (i < 2) {
                    container->Add(Renderer([]{
                        return separatorDouble();
                    }));
                }
            }

            mainComponent =container;
            return container | border;
        }

        ftxui::Component getComponent() {
            return mainComponent;
        }

        void setbigicon_big(int x = 0, int y = 0, int icon = 0) {
            grids[x][y]->setbigicon(icon);
            makeGridComponent();
        }

        void takefocus_big(int x = 1, int y = 1,int z = 1,int w = 1) {
            grids[x][y]->takefocus_sm_grid(y,w);
        }

        void setCurrentPlayer(int player) {
            if (player == 1 || player == 2) {
                variables::currentPlayer = player;
            }
            else{
                throw std::invalid_argument("Invalid player");
            }
        }

        void SetUpdateCallback(std::function<void()> callback) {
                callbacks::onUpdate = callback;
            }
};
