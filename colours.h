#include <iostream>
#include <vector>


const int DEFAULT = 0;
const int WALL = 1;
const int TARGET = 2;
const int START = 3;
const int SEEN = 4;
const int PATH = 5;

std::vector<int> getCellColour(int currentCell);
std::vector<std::vector<int>> drawCoords(std::vector<std::vector<int>>& grid, const std::vector<std::vector<int>> &coordVector, const int& COLOUR, const std::vector<int> &startCoords, const std::vector<int> &targetCoords);
