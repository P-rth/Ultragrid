#pragma once

#include <string>
#include <ftxui/component/component.hpp>
//#include <ftxui/dom/elements.hpp>
#include <vector>


#include <sstream>
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig, FlexboxConfig::JustifyContent, FlexboxConfig::JustifyContent::Center, FlexboxConfig::JustifyContent::FlexEnd, FlexboxConfig::JustifyContent::SpaceBetween
#include <iostream>
#include <cstdlib>

// Function to execute terminal commands safely
void ExecuteCommand(const std::string& command);

//function to print ascii art
namespace ftxui{
Elements Split(std::string the_text);
Element nonWrappingParagraph(std::string the_text);
}