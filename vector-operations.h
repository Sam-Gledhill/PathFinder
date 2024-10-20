#include <algorithm>
#include <vector>
#include <math.h>

float euclidianDistance(float x1, float y1, float x2, float y2);
std::vector<int> minEuclidianCoord(const std::vector<std::vector<int>> &queue, const std::vector<int> &targetCoords);
std::vector<std::vector<int>> getAdjacentCoords(int x, int y, int width, int height);
bool coordIn(const std::vector<int> &coord, const std::vector<std::vector<int>> &seenVector);
