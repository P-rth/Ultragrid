cmake_minimum_required(VERSION 3.22)

project(TicTacTournament LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Debug) # Set Debug mode


# --- Fetch FTXUI ---
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        main
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# --------------------

# Collect source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)


add_executable(TicTacTournament ${SRC_FILES})

# Include directories
target_include_directories(TicTacTournament PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

# Link libraries
target_link_libraries(TicTacTournament

  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(TicTacTournament PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_target_properties(TicTacTournament PROPERTIES OUTPUT_NAME "ttt")
