#include "colours.h"

std::vector<int> getCellColour(int currentCell){

    std::vector<int> rgbVector;

    if (currentCell==DEFAULT){
        rgbVector = {255,255,255};
    }

    else if (currentCell == WALL)
    {
        rgbVector = {20,20,20};
    }

    else if (currentCell == TARGET)
    {
        rgbVector = {255,0,0};
    }

    else if (currentCell == START){
        rgbVector= {0,0,255};
    }

    else if (currentCell == SEEN){
        rgbVector= {255,0,255};
    }

    else if (currentCell == PATH){
        rgbVector= {255,255,0};
    }

    return rgbVector;

}

std::vector<std::vector<int>> drawCoords(std::vector<std::vector<int>>& grid, const std::vector<std::vector<int>> &coordVector, const int& COLOUR, const std::vector<int> &startCoords, const std::vector<int> &targetCoords){

    for(auto coord: coordVector){

        if(coord == startCoords){
            continue;
        }

        if(coord == targetCoords){
            continue;
        }

        grid[coord[1]][coord[0]] = COLOUR;
    }

    return grid;
}
