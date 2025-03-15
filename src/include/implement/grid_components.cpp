#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red
#include "ftxui/dom/node.hpp"      // for Render
#include <cmath>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <iostream>
#include <ostream>
#include <vector>

#include "./headers/helpers.hpp"
#include "./headers/art.hpp"


using namespace ftxui;
using namespace std::chrono_literals;


using TicTacToeButton_Options_Grid = std::vector<std::vector<std::vector<int>>>;

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
            using namespace ftxui;

            auto buttonComponent = Button(" ", [this] {
                bool isdisabled = options[0] == 1;  // Check current disabled state
                if (!isdisabled && gridValue == 0) {
                    gridValue = 1;
                }
            },
            ButtonOption::Animated(Color::Red)
            );

            buttonComponent |= CatchEvent([this, buttonComponent](Event event) {
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
                            size(WIDTH, EQUAL, 3);

                if (isdisabled) {
                    if (buttonComponent->Focused()) {
                        content = content | dim | bgcolor(ftxui::Color::Grey50);
                    } else {
                        content = content | dim | bgcolor(ftxui::Color::Grey30);
                    }
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
        int selected_x = 0;
        int selected_y = 0;
        int bigicon = 0;
        std::vector<int>* buttonOptions;
        ftxui::Component gridComponent;

    public:
        std::vector<std::vector<int>> grid;

        SmallGrid(std::vector<int>* values) : buttonOptions(values) {
            grid = {{0, 0, 0},
                   {0, 0, 0},
                   {0, 0, 0}};

            buttons.resize(3);
            for(int i = 0; i < 3; i++) {
                buttons[i].resize(3);
                for(int j = 0; j < 3; j++) {
                    buttons[i][j] = std::make_unique<TicTacToeButton>(grid[i][j], i, j, *buttonOptions);
                }
            }

            gridComponent = makeGridComponent();
        }

        std::vector<std::vector<int>>* getGridRef() {
            return &grid;
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
                                return separatorCharacter("·");
                            }));
                        }
                    }

                    container->Add(row);

                    if (i < 2) {
                        container->Add(Renderer([]{
                            return separatorCharacter("·");
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
        std::vector<std::vector<std::unique_ptr<SmallGrid>>> grids;
        int selected_x = 0;
        int selected_y = 0;
        Component mainComponent;
        largegrid_val_ptr grids_val;
        TicTacToeButton_Options_Grid grid_options;

    public:
        LargeGrid() {
            grid_options.resize(3);
            for(int i = 0; i < 3; i++) {
                grid_options[i].resize(3);
                for(int j = 0; j < 3; j++) {
                    grid_options[i][j] = {0,0};
                }
            }

            grids_val.resize(3);
            for(int i = 0; i < 3; i++) {
                grids_val[i].resize(3);
            }

            grids.resize(3);
            for(int i = 0; i < 3; i++) {
                grids[i].resize(3);
                for(int j = 0; j < 3; j++) {
                    grids[i][j] = std::make_unique<SmallGrid>(&grid_options[i][j]);
                    grids_val[i][j] = grids[i][j]->getGridRef();
                }
            }

            mainComponent = makeGridComponent();
        }

        int getValue(int bigRow, int bigCol, int smallRow, int smallCol) const {
            return (*grids_val[bigRow][bigCol])[smallRow][smallCol];
        }

        largegrid_val get4DArray() const {
            largegrid_val values(3,
                std::vector<std::vector<std::vector<int>>>(3,
                    std::vector<std::vector<int>>(3,
                        std::vector<int>(3))));

            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    values[i][j] = *grids_val[i][j];
                }
            }
            return values;
        }

        void setoptions(int bigRow, int bigCol, std::vector<int> op) {
            grid_options[bigRow][bigCol] = op;
            //mainComponent->Refresh();
        }

        void setoptions_invert(int bigRow, int bigCol, std::vector<int> op, std::vector<int> op_inv) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (i == bigRow && j == bigCol) {
                        grid_options[i][j] = op;
                    }
                    else {
                        grid_options[i][j] = op_inv;
                    }
                }
            }
            //mainComponent->Refresh();
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

            mainComponent = container;
            return mainComponent;
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
};
