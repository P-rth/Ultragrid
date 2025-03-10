#pragma once

#include <string>
#include <ftxui/component/component.hpp>
#include <cstdlib>

// Function to execute terminal commands safely
void ExecuteCommand(const std::string& command);

//function to print ascii art
namespace ftxui{
Elements Split(std::string the_text);
Element nonWrappingParagraph(std::string the_text);
}
