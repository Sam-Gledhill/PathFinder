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
