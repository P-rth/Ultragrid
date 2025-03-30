# Ultragrid Project Documentation

## Project Overview
A terminal-based Ultra Tic-Tac-Toe game using the FTXUI library for terminal UI rendering.

## Directory Structure
```plaintext
Ultragrid/
├── src/
│   ├── include/
│   │   ├── headers/    (Declarations)
│   │   └── implement/  (Implementations)
│   └── main.cpp
├── bin/                (Executables)
├── build/             (Build files)
├── .zed/              (Editor config)
├── CMakeLists.txt
└── build.sh
```

## Class Diagram
```plaintext
Game (Main Application)
├── UI Components
│   ├── SplashScreen
│   │   └── RenderStartScreen()
│   │
│   └── StartMenu
│       └── StartupUI()
│
├── Grid Components
│   ├── TicTacToeButton
│   │   ├── Properties
│   │   │   ├── gridValue: int&
│   │   │   ├── row: int
│   │   │   ├── col: int
│   │   │   ├── isHovered: bool
│   │   │   ├── isPressed: bool
│   │   │   ├── click_release_edge: bool
│   │   │   ├── options: vector<int>&
│   │   │   ├── box_: Box
│   │   │   └── button: Component
│   │   │
│   │   └── Methods
│   │       ├── TakeFocus_btn()
│   │       ├── GetButton(): Component
│   │       ├── GetRow(): int
│   │       └── GetCol(): int
│   │
│   ├── SmallGrid
│   │   ├── Properties
│   │   │   ├── buttons: vector<vector<unique_ptr<TicTacToeButton>>>
│   │   │   ├── selected_x: int
│   │   │   ├── selected_y: int
│   │   │   ├── bigicon: int
│   │   │   ├── buttonOptions: vector<int>*
│   │   │   ├── gridComponent: Component
│   │   │   └── grid: vector<vector<int>>
│   │   │
│   │   └── Methods
│   │       ├── getGridRef(): vector<vector<int>>*
│   │       ├── takefocus_sm_grid(x, y)
│   │       ├── getGridComponent(): Component
│   │       └── setbigicon(bigicon_set)
│   │
│   └── LargeGrid
│       ├── Properties
│       │   ├── grids: vector<vector<unique_ptr<SmallGrid>>>
│       │   ├── selected_x: int
│       │   ├── selected_y: int
│       │   ├── mainComponent: Component
│       │   ├── grids_val: largegrid_val_ptr
│       │   └── grid_options: TicTacToeButton_Options_Grid
│       │
│       └── Methods
│           ├── getValue(bigRow, bigCol, smallRow, smallCol): int
│           ├── get4DArray(): largegrid_val
│           ├── setoptions(bigRow, bigCol, op)
│           ├── setoptions_invert(bigRow, bigCol, op, op_inv)
│           ├── getComponent(): Component
│           ├── setbigicon_big(x, y, icon)
│           └── takefocus_big(x, y, z, w)
│
├── FTXUI Components (External)
│   ├── Screen
│   ├── Component
│   ├── Element
│   └── ScreenInteractive
│
├── Helper Functions
│   ├── ExecuteCommand(command: string)
│   ├── largegrid_to_cout(grid4d: largegrid_val)
│   ├── Split(text: string): Elements
│   └── nonWrappingParagraph(text: string): Element
│
└── Custom Types
    ├── smallgrid_val_ptr
    │   └── vector<vector<int>>*
    │
    ├── largegrid_val_ptr
    │   └── vector<vector<smallgrid_val_ptr>>
    │
    └── largegrid_val
        └── vector<vector<vector<vector<int>>>>

```


## Relationships and Dependencies

```plaintext
Dependency Flow:
LargeGrid
  ├── Contains → SmallGrid[3][3]
  └── Uses → FTXUI::Component

SmallGrid
  ├── Contains → TicTacToeButton[3][3]
  └── Uses → FTXUI::Component

TicTacToeButton
  ├── Uses → FTXUI::Component
  └── References → SmallGrid (parent)

Game
  ├── Uses → LargeGrid
  ├── Uses → UI Components
  └── Uses → FTXUI::ScreenInteractive
```

## Component Interactions

```plaintext
User Input → Game
  └── ScreenInteractive
      ├── LargeGrid
      │   └── Manages game state and grid interactions
      │
      ├── SmallGrid
      │   └── Manages individual 3x3 grid states
      │
      └── TicTacToeButton
          └── Handles individual cell interactions
```

## File Contents and Responsibilities

### 1. Main Components (`grid_components.cpp`)

#### TicTacToeButton
- Individual game cell component
- Handles mouse interactions
- Manages button state and rendering

#### SmallGrid
- 3x3 grid of TicTacToeButtons
- Manages local grid state
- Handles grid component rendering

#### LargeGrid
- 3x3 grid of SmallGrids
- Manages game state
- Coordinates between grids

### 2. Helper Functions (`helpers.hpp`/`helpers.cpp`)
```cpp
void ExecuteCommand(const std::string& command);
void largegrid_to_cout(largegrid_val grid4d);
Elements Split(std::string the_text);
Element nonWrappingParagraph(std::string the_text);
```

### 3. Custom Types
```cpp
using smallgrid_val_ptr = std::vector<std::vector<int>>*;
using largegrid_val_ptr = std::vector<std::vector<smallgrid_val_ptr>>;
using largegrid_val = std::vector<std::vector<std::vector<std::vector<int>>>>;
```

## Build System

#### using  CMakeLists.txt


### Build Script (build.sh)
```bash
mkdir -p build && cd ./build && clear && cmake .. && make && echo "--- build end ---" && ../bin/ttt
```

## Game Flow

1. **Initialization**
   - Splash screen display
   - Menu interface
   - Game mode selection

2. **Game Loop**
   - Player moves
   - Grid state updates
   - Win condition checks
   - UI rendering

3. **UI Components**
   - ASCII art rendering
   - Grid visualization
   - Interactive buttons
   - Menu system

## Dependencies

- **FTXUI**: Terminal UI library
  - Screen management
  - Component rendering
  - Event handling




## Todo/Improvements

1. **Code Organization**
   - Break down large files
   - Implement namespaces
   - Create game logic layer

2. **Design Patterns**
   - Implement MVC pattern
   - Use Builder pattern
   - Add Event system

3. **Error Handling**
   - Add exception classes
   - Implement input validation
   - Add error recovery

4. **Features**
   - Add multiplayer support
   - Implement AI opponent
   - Add game statistics

## Building and Running

1. **Prerequisites**
   - CMake 3.22+
   - C++17 compiler
   - FTXUI library (auto-downloaded)

2. **Build Commands**
   ```bash
   ./build.sh
   ```

3. **Run Game**
   ```bash
   ./bin/ttt
   ```

## Code Style Guidelines

1. **Naming Conventions**
   idk random

2. **Documentation**
   - Header comments
   - Function documentation
   - Class descriptions

3. **Code Organization**
   - Logical file structure
   - Clear component separation
   - Consistent formatting

pls god  organise my code

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make changes
4. Submit pull request

## License
apun ka code he
