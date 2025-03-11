#pragma once

#include <string>
#include <ftxui/component/component.hpp>
#include <cstdlib>

using smallgrid_val_ptr = std::vector<std::vector<int>>*;
using largegrid_val_ptr = std::vector<std::vector<smallgrid_val_ptr>>;
using largegrid_val = std::vector<std::vector<std::vector<std::vector<int>>>>;


// Function to execute terminal commands safely
void ExecuteCommand(const std::string& command);

//print largegrid to cout for debugging purposes
void largegrid_to_cout(largegrid_val grid4d);

//function to print ascii art
namespace ftxui{
Elements Split(std::string the_text);
Element nonWrappingParagraph(std::string the_text);
}
