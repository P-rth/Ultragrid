#pragma once

#include <string>
#include <ftxui/component/component.hpp>
#include <cstdlib>

using smallgrid_val_ptr = int (*)[3][3];
using largegrid_val_ptr = int (*)[3][3][3][3];
using largegrid_val = int[3][3][3][3];


// Function to execute terminal commands safely
void ExecuteCommand(const std::string& command);

//print largegrid to cout for debugging purposes
void largegrid_to_cout(largegrid_val grid4d);

void print_grid(int grid[3][3]);

int checkBoard(int board[3][3]);

//function to print ascii art
namespace ftxui{
Elements Split(std::string the_text);
Element nonWrappingParagraph(std::string the_text);
}
