#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <vector>
#include <string>


#include <iostream>
#include <sstream>
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig, FlexboxConfig::JustifyContent, FlexboxConfig::JustifyContent::Center, FlexboxConfig::JustifyContent::FlexEnd, FlexboxConfig::JustifyContent::SpaceBetween


using namespace ftxui;

// Function to render initial start screen
void RenderStartScreen();
