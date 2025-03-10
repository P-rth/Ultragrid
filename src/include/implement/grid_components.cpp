#include "ftxui/component/component.hpp"       // for Menu, Renderer, Vertical
#include "ftxui/component/mouse.hpp"              // for ftxui
#include "ftxui/dom/elements.hpp"  // for text, Element, operator|, borderEmpty, inverted
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Red
#include "ftxui/dom/node.hpp"      // for Render


using namespace ftxui;
using namespace std::chrono_literals;


class TicTacToeButton {
    private:
        int& gridValue;
        int row;
        int col;
        bool isHovered = false;
        bool isPressed = false;
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
    TicTacToeButton(int& gridRef, int r, int c)
            : gridValue(gridRef), row(r), col(c) {}

        ftxui::Component makeButton() {
            using namespace ftxui;

            auto button = Button(" ", [this] {
                if (gridValue == 0) {
                    gridValue = 1;
                }
            });

            button |= CatchEvent([this,button](Event event) {
                if (!event.is_mouse()) {
                    // Handle keyboard input (Enter/Space) when focused
                    if (button->Focused()) {
                        if (event == Event::Return) {
                            if (gridValue == 0) {
                                gridValue = 1;
                                return true;
                            }
                        }
                    }
                    return false;
                }

                const bool mouse_in_button = box_.Contain(event.mouse().x, event.mouse().y);

                // Handle different mouse states
                if (event.mouse().button == Mouse::Left) {
                    if (event.mouse().motion == Mouse::Pressed) {
                        if (mouse_in_button) {
                            isPressed = true;
                            button->TakeFocus(); // Take focus when clicked
                            return true;
                        }
                    } else if (event.mouse().motion == Mouse::Released) {
                        if (isPressed) {
                            isPressed = false;
                            return true;
                        }
                    }
                }

                // Update hover state
                if (mouse_in_button != isHovered) {
                    isHovered = mouse_in_button;
                    return true;
                }
                return false;
            });

            return Renderer(button, [this,button] {
                auto content = text(getSymbolString(gridValue)) |
                            center |
                            size(WIDTH, EQUAL, 3);

                // Different visual states
                if (button->Focused()) {  // Check if button has focus
                    if (isPressed && gridValue == 0) {
                        gridValue = 1;
                        content = content | bgcolor(ftxui::Color::Red);
                    } else {
                        content = content | inverted;  // Highlight focused button
                    }
                } else if (isPressed && gridValue == 0) {
                    content = content | bgcolor(ftxui::Color::Red);
                } else if (isHovered && gridValue == 0) {
                    content = content | bgcolor(ftxui::Color::GrayDark);
                }

                return content | reflect(box_);
            });
        }

};



class SmallGrid {
    private:
        std::vector<std::vector<int>> grid;    // 0 = empty, 1 = X, 2 = O
        std::vector<std::vector<std::unique_ptr<TicTacToeButton>>> buttons;

    public:
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

        ftxui::Component makeGridComponent() {
            using namespace ftxui;
            auto container = Container::Vertical({});

            for (int i = 0; i < 3; i++) {
                auto row = Container::Horizontal({});
                for (int j = 0; j < 3; j++) {
                    // Use the TicTacToeButton class
                    row->Add(buttons[i][j]->makeButton());

                    // Add vertical separator except after last column
                    if (j < 2) {
                        row->Add(Renderer([]{
                            return separator();
                        }));
                    }
                }

                container->Add(row);

                // Add horizontal separator except after last row
                if (i < 2) {
                    container->Add(Renderer([]{
                        return separator();
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
