#include "../headers/helpers.hpp"


#include <sstream>
#include "ftxui/dom/flexbox_config.hpp"  // for FlexboxConfig, FlexboxConfig::JustifyContent, FlexboxConfig::JustifyContent::Center, FlexboxConfig::JustifyContent::FlexEnd, FlexboxConfig::JustifyContent::SpaceBetween
#include <iostream>


void ExecuteCommand(const std::string& command) {
    int ret_code = system(command.c_str());
    if (ret_code != 0) {
        std::cerr << "Error executing command: " << command << std::endl;
    }
}


void largegrid_to_cout(largegrid_val grid4d){
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            for(int k = 0; k < 3; k++) {
                for(int l = 0; l < 3; l++) {
                    std::cout << grid4d[i][j][k][l] << " ";
                }
                std::cout << std::endl;
            }
            std::cout <<"**********************"<< std::endl;
        }
    }
};

int checkBoard(int board[3][3]) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0]; // Row win
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i]; // Column win
    }

    // Check diagonals
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    // Check for empty (still playing)
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == 0)
                return 0; // Still playing

    return 3; // Tie
}



namespace ftxui{
Elements Split(std::string the_text) {
    Elements output;
    std::stringstream ss(the_text);
    std::string word;

    while (ss >> word)  // This correctly extracts words separated by spaces/newlines.
      output.push_back(text(word));

    return output;
  }



Element nonWrappingParagraph(std::string the_text) {
Elements lines;
for (auto& line : Split(std::move(the_text)))  // Missing parenthesis was fixed
    lines.push_back(line);  // `line` is already an `Element`, no need for `text(line)`

    static const auto config =
        FlexboxConfig().SetGap(1, 0).Set(FlexboxConfig::JustifyContent::Center);


    return flexbox({vbox(std::move(lines))}, config);

}


Element paragraphAlignCenter(const std::string& the_text) {
    static const auto config =
        FlexboxConfig().SetGap(1, 0).Set(FlexboxConfig::JustifyContent::Center);
    return flexbox(Split(the_text), config);
  }


}
