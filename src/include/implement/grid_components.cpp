
#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red
#include "ftxui/dom/node.hpp"      // for Render
#include <cmath>
#include <vector>

#include "./headers/helpers.hpp"


using namespace ftxui;
using namespace std::chrono_literals;



using TicTacToeButton_Options_Grid = std::vector<std::vector<std::vector<int>>>;  //a vector for all x*y buttons to store things like disabled, win/blink  format :: [bool disabled, bool win, bool blink]



class TicTacToeButton {
    private:

        int& gridValue;
        int row;
        int col;
        bool isHovered = false;
        bool isPressed = false;
        bool click_release_edge = false;
        ftxui::Component button;
        ftxui::Box box_;

        std::string getSymbolString(int value) const {
            switch(value) {
                case 1: return "X";
                case 2: return "O";
                default: return " ";
            }
        };


    public:
        TicTacToeButton(int& gridRef, int r, int c): gridValue(gridRef), row(r), col(c) {}

        ftxui::Component makeButton() {
            using namespace ftxui;

            auto button = Button(" ", [this] {
                if (gridValue == 0) {
                    gridValue = 1;
                }
            });

            button |= CatchEvent([this,button](Event event) {                           //function should be true if event is handled otherwise false

                const bool mouse_in_button = box_.Contain(event.mouse().x, event.mouse().y);

                // Update hover state and focus when changed
                if (mouse_in_button != isHovered) {
                    button->TakeFocus();
                    isHovered = mouse_in_button;
                    return true;
                }

                // Handle different mouse states
                if (event.mouse().button == Mouse::Left) {
                    if (event.mouse().motion == Mouse::Pressed) {
                        if (mouse_in_button) {
                            isPressed = true;
                            return true;
                        }
                        else {
                            isPressed = false;
                        }
                    } else if (event.mouse().motion == Mouse::Released && mouse_in_button) {
                        if (isPressed) {
                            click_release_edge = true;
                            return true;
                        }
                    } else if (event.mouse().motion == Mouse::Released) {
                        isPressed = false;
                    }
                }

                return false;
            });

            return Renderer(button, [this,button] {
                auto content = text(getSymbolString(gridValue)) |
                            center |
                            size(WIDTH, EQUAL, 3);

                // Different visual states
                if (button->Focused()) {  // Check if button has focus
                    if (click_release_edge && gridValue == 0) {
                        gridValue = 1;
                        content = content | bgcolor(ftxui::Color::Red);

                    } else {
                        if (isPressed && gridValue == 0) {
                            content = content | bgcolor(ftxui::Color::OrangeRed1);           //when click is held and not let go
                        } else if (gridValue == 0) {
                            content = content | bgcolor(ftxui::Color::GrayDark);            //when mouse is hovered over button
                        } else if (gridValue == 1){
                            content = content | color(ftxui::Color::Red);
                        } else if (gridValue == 2){
                            content = content | color(ftxui::Color::Blue);
                        }
                    }
                }

                click_release_edge = false;
                return content | reflect(box_);
            });
        }

};



class SmallGrid {
    private:
        std::vector<std::vector<std::unique_ptr<TicTacToeButton>>> buttons;
        int selected_x = 0;  // Shared x coordinate for horizontal navigation
        int selected_y = 0;  // y coordinate for vertical navigation

    public:
        std::vector<std::vector<int>> grid;    // 0 = empty, 1 = X, 2 = O

        SmallGrid() {
            grid = {{0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0}};

            // Initialize buttons
            buttons.resize(3);
            for(int i = 0; i < 3; i++) {
                buttons[i].resize(3);
                for(int j = 0; j < 3; j++) {
                    buttons[i][j] = std::make_unique<TicTacToeButton>(grid[i][j], i, j);
                }
            }
        }

        std::vector<std::vector<int>>* getGridRef() {
                return &grid;
            }

        ftxui::Component makeGridComponent() {
            using namespace ftxui;
            auto container = Container::Vertical({},&selected_y);

            for (int i = 0; i < 3; i++) {
                auto row = Container::Horizontal({},&selected_x);
                for (int j = 0; j < 3; j++) {
                    // Use the TicTacToeButton class
                    row->Add(buttons[i][j]->makeButton());

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
                    grids[i][j] = std::make_unique<SmallGrid>();
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

};
