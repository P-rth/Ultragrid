#include "../headers/ultragrid_ai.hpp"

bool UltragridAI::MakeMove(int difficulty, LargeGrid& grid) {
    switch(difficulty) {
        case 0: return MakeBasicMove(grid);
        case 1: return MakeRandomMove(grid);
        case 2: return MakeRandomMove(grid);
        default: return false;
    }
}

bool UltragridAI::MakeBasicMove(LargeGrid& grid) {
    int grid4d[3][3][3][3];
    grid.get4DArray(grid4d);
    bool moveMade = false;
    int nextGrid_x = variables::lastmove[0];
    int nextGrid_y = variables::lastmove[1];

    if (nextGrid_x != -1 && nextGrid_y != -1 &&
        grid.getValue_big(nextGrid_x, nextGrid_y) == 0) {
        for (int x = 0; x < 3 && !moveMade; x++) {
            for (int y = 0; y < 3 && !moveMade; y++) {
                if (grid4d[nextGrid_x][nextGrid_y][x][y] == 0) {
                    grid.makemove(nextGrid_x, nextGrid_y, x, y);
                    moveMade = true;
                }
            }
        }
    }

    if (!moveMade) {
        for (int gx = 0; gx < 3 && !moveMade; gx++) {
            for (int gy = 0; gy < 3 && !moveMade; gy++) {
                if (grid.getValue_big(gx, gy) == 0) {
                    for (int x = 0; x < 3 && !moveMade; x++) {
                        for (int y = 0; y < 3 && !moveMade; y++) {
                            if (grid4d[gx][gy][x][y] == 0) {
                                grid.makemove(gx, gy, x, y);
                                moveMade = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return moveMade;
}

bool UltragridAI::MakeRandomMove(LargeGrid& grid) {
    int grid4d[3][3][3][3];
    grid.get4DArray(grid4d);
    std::vector<std::tuple<int, int, int, int>> availableMoves;
    int nextGrid_x = variables::lastmove[0];
    int nextGrid_y = variables::lastmove[1];

    if (nextGrid_x != -1 && nextGrid_y != -1 &&
        grid.getValue_big(nextGrid_x, nextGrid_y) == 0) {
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                if (grid4d[nextGrid_x][nextGrid_y][x][y] == 0) {
                    availableMoves.push_back(std::make_tuple(nextGrid_x, nextGrid_y, x, y));
                }
            }
        }
    }

    if (availableMoves.empty()) {
        for (int gx = 0; gx < 3; gx++) {
            for (int gy = 0; gy < 3; gy++) {
                if (grid.getValue_big(gx, gy) == 0) {
                    for (int x = 0; x < 3; x++) {
                        for (int y = 0; y < 3; y++) {
                            if (grid4d[gx][gy][x][y] == 0) {
                                availableMoves.push_back(std::make_tuple(gx, gy, x, y));
                            }
                        }
                    }
                }
            }
        }
    }

    if (!availableMoves.empty()) {
        int randomIndex = rand() % availableMoves.size();
        auto [gx, gy, x, y] = availableMoves[randomIndex];
        grid.makemove(gx, gy, x, y);
        return true;
    }
    return false;
}





bool UltragridAI::MakeSmartMove(LargeGrid& grid) { //todo
};
