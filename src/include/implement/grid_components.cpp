
#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red
#include "ftxui/dom/node.hpp"      // for Render
#include <cmath>
#include <iostream>
#include <ostream>
#include <vector>

#include "./headers/helpers.hpp"


using namespace ftxui;
using namespace std::chrono_literals;



using TicTacToeButton_Options_Grid = std::vector<std::vector<std::vector<int>>>;  //a vector for all x*y buttons to store options format :: [int disabled, int blink]  --> 0/1 (int used so that can add more features in future)

class TicTacToeButton {
private:
    int& gridValue;
    int row;
    int col;
    bool isHovered = false;
    bool isPressed = false;
    bool click_release_edge = false;
    std::vector<int>& options;
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
        bool isdisabled = options[0] == 1;  // 0 = not disabled
        std::cout << isdisabled << " OPTION" << std::endl;

        using namespace ftxui;

        auto buttonComponent = Button(" ", [this, isdisabled] {
            if (!isdisabled && gridValue == 0) {
                gridValue = 1;
            }
        });

        buttonComponent |= CatchEvent([this, buttonComponent, isdisabled](Event event) {
            const bool mouse_in_button = box_.Contain(event.mouse().x, event.mouse().y);

            if (isdisabled && buttonComponent->Focused()) {
                return false;
            }

            if (mouse_in_button != isHovered) {
                buttonComponent->TakeFocus();
                isHovered = mouse_in_button;
                return true;
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

        return Renderer(buttonComponent, [this, buttonComponent, isdisabled] {
            auto content = text(getSymbolString(gridValue)) |
                          center |
                          size(WIDTH, EQUAL, 3);

            if (isdisabled) {
                content = content | dim | bgcolor(ftxui::Color::Grey35);
            }

            if (buttonComponent->Focused() && !isdisabled) {
                if (click_release_edge && gridValue == 0) {
                    gridValue = 1;
                    content = content | bgcolor(ftxui::Color::Red);
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
    TicTacToeButton(int& gridRef, int r, int c, std::vector<int>& opts)
        : gridValue(gridRef), row(r), col(c), options(opts) {
        button = makeButton();
    }

    void TakeFocus_btn() {
        //std::cout << "TicTacToeButton: Attempting to take focus" << std::endl;
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
        std::vector<std::vector<std::unique_ptr<TicTacToeButton>>> buttons;
        int selected_x = 0;  // Shared x coordinate for horizontal navigation
        int selected_y = 0;  // y coordinate for vertical navigation
        int& disabled;
        std::vector<int> buttonOptions;

    public:
        std::vector<std::vector<int>> grid;    // 0 = empty, 1 = X, 2 = O

        SmallGrid(int& d) : disabled(d) , buttonOptions{d} {

            grid = {{0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0}};

            // Initialize buttons
            buttons.resize(3);
            for(int i = 0; i < 3; i++) {
                buttons[i].resize(3);
                for(int j = 0; j < 3; j++) {
                    buttons[i][j] = std::make_unique<TicTacToeButton>(grid[i][j], i, j, buttonOptions);
                }
            }
        }

        std::vector<std::vector<int>>* getGridRef() {
                return &grid;
            }

        void takefocus_sm_grid(int x = 1, int y = 1) {

        //  std::cout << "Taking focus at (" << x << ", " << y << ")  smallgrid" << std::endl;
            buttons[x][y]->TakeFocus_btn();
        }

        ftxui::Component makeGridComponent() {
            using namespace ftxui;
            auto container = Container::Vertical({},&selected_y);

            for (int i = 0; i < 3; i++) {
                auto row = Container::Horizontal({},&selected_x);
                for (int j = 0; j < 3; j++) {
                    // Use the TicTacToeButton class
                    row->Add(buttons[i][j]->GetButton());

                    // Add vertical separator except after last column
                    if (j < 2) {
                        row->Add(Renderer([]{
                            return separatorCharacter("·");
                        }));
                    }
                }

                container->Add(row);

                // Add horizontal separator except after last row
                if (i < 2) {
                    container->Add(Renderer([]{
                        return separatorCharacter("·");
                    }));
                }
            }

            return container;
        }

        // Keep getSymbolString if needed elsewhere in SmallGrid
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
        std::vector<std::vector<std::unique_ptr<SmallGrid>>> grids;
        int selected_x = 0;
        int selected_y = 0;



        largegrid_val_ptr grids_val;
        TicTacToeButton_Options_Grid grid_states;


    public:
        LargeGrid() {

            grids_val.resize(3);
            for(int i = 0; i < 3; i++) {
                grids_val[i].resize(3);
            }


            grids.resize(3);
            for(int i = 0; i < 3; i++) {
                grids[i].resize(3);
                for(int j = 0; j < 3; j++) {
                    int dis = 0;
                    if (i == 1 && j==1){
                        dis = 1;
                    }
                    grids[i][j] = std::make_unique<SmallGrid>(dis);
                    grids_val[i][j] = grids[i][j]->getGridRef();
                }
            }

        }

        int getValue(int bigRow, int bigCol, int smallRow, int smallCol) const {
            return (*grids_val[bigRow][bigCol])[smallRow][smallCol];
        }


        largegrid_val get4DArray() const {
            // Initialize 4D vector with size 3x3x3x3

            largegrid_val values(3,
                std::vector<std::vector<std::vector<int>>>(3,
                    std::vector<std::vector<int>>(3,
                        std::vector<int>(3))));


            // Copy values from pointers to the 4D array
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    // Dereference the pointer to get the 2D grid of the small grid
                    values[i][j] = *grids_val[i][j];
                }
            }
            return values;
        }

        ftxui::Component makeGridComponent() {
            using namespace ftxui;
            auto container = Container::Vertical({}, &selected_y);

            for(int i = 0; i < 3; i++) {
                auto row = Container::Horizontal({}, &selected_x);

                for(int j = 0; j < 3; j++) {
                    row->Add(grids[i][j]->makeGridComponent());

                    // Add vertical separator between SmallGrids
                    if (j < 2) {
                        row->Add(Renderer([]{
                            return separatorDouble();
                        }));
                    }
                }

                container->Add(row);

                // Add horizontal separator between rows
                if (i < 2 ) {
                    container->Add(Renderer([]{
                        return separatorDouble();
                    }));
                }
            }

            return container;
        }

        void takefocus_big(int x = 1, int y = 1,int z = 1,int w = 1) {
            //std::cout << "Taking focus at (" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;
            grids[x][y]->takefocus_sm_grid(y,w);
        }

};
