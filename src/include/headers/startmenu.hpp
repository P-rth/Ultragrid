#pragma once

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


// Function to wrap components with labels

Component Wrap(std::string name, Component component);

// Function to create the main UI layout
int StartupUI();

