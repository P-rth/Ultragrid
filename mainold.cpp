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


 
// Display a component nicely with a title on the left.
Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 10) | size(HEIGHT, EQUAL, 5),
               separator(),
               component->Render()  ,
           }) | size(HEIGHT, EQUAL,5);
  });
}
 
int main() {
  Dimensions dim = Terminal::Size(); // Correct way
  dim.dimy -= 1;
  
  auto screen = ScreenInteractive::Fullscreen();

  // -- Menu
  // ----------------------------------------------------------------------
  const std::vector<std::string> menu_entries = {
      "Menu 1",
      "Menu 2",
      "Menu 3",
      "Menu 4",
  };

  auto option = MenuOption().HorizontalAnimated();
  option.underline.SetAnimationDuration(100ms);
  option.underline.color_active = Color::Red;
  option.underline.color_inactive = Color::Blue;
  option.underline.follower_delay = 100ms;
  option.on_enter = screen.ExitLoopClosure();


  int menu_selected = 0;
  auto menu = Menu(&menu_entries, &menu_selected, option);
  menu = Wrap("Menu", menu);
 
 
  // -- Button -----------------------------------------------------------------
  std::string button_label = "Quit";
  //std::function<void()> on_button_clicked_;
  auto button = Button(&button_label, screen.ExitLoopClosure());
  button = Wrap("Button", button);
 
  
  // A large text ----------------------------------------------------------------

  auto lorel_ipsum = Renderer([&menu_selected] {
    return vbox({
        text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. "),
         text("value = " + std::to_string(menu_selected)),
 
    });
  });
  lorel_ipsum = Wrap("Lorel Ipsum", lorel_ipsum) ;
 

  // -- Layout
  // -----------------------------------------------------------------
  auto layout = Container::Vertical({
      menu,
      button,
      lorel_ipsum,
  });
 
  auto component = Renderer(layout, [&] 
    {


    auto menuwin =  vbox({
               menu->Render(),
               separator(),
               button->Render(),
               separator(),
               lorel_ipsum->Render(),
           });
           
    menuwin = menuwin | size(WIDTH, EQUAL, 40) | size(HEIGHT, EQUAL, 20) | border;

    auto document = vbox({
        filler(),

        hbox({
            filler(),
            menuwin,
            filler()}
        ),

        filler()
    });

    return document;

  });
  



auto document1 = hbox({
  filler(),

  vbox({
      filler(),
      text("HELLO world"),
      filler()}
  ),

  filler(),
  text("v1")
});


auto start_screen = Screen::Create(dim);
Render(start_screen, document1);

std::string a = start_screen.ToString();
std::cout << std::flush;
std::string commad = "echo -n \"" + a + "\" | tte --ignore-terminal-dimensions scattered";

system(commad.c_str());
std::cout << "Press Enter to continue";
std::cin.get();
std::cout << "\033[2J\033[1;1H";
//std::cin;

// Render the component and display it
//auto document = component->Render();
//std::cout << menu_selected << std::endl ;


//screen.RequestAnimationFrame()
//std::cout << test.ToString();
//system("clear && echo  | tte slide");

screen.Loop(component);


}