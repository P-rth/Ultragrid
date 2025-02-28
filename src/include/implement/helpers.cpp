#include "../headers/helpers.hpp"


void ExecuteCommand(const std::string& command) {
    int ret_code = system(command.c_str());
    if (ret_code != 0) {
        std::cerr << "Error executing command: " << command << std::endl;
    }
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