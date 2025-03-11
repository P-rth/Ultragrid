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
