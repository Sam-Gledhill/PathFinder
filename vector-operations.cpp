#include "vector-operations.h"

float euclidianDistance(float x1, float y1, float x2, float y2){
    float dx = x2-x1;
    float dy = y2-y1;

    return sqrt(dx*dx + dy*dy);
}

std::vector<int> minEuclidianCoord(const std::vector<std::vector<int>> &queue, const std::vector<int> &targetCoords){
    //returns coordinate closest to the target


    std::vector<float> euclidianVector;

    for(auto coord: queue){
        euclidianVector.push_back(euclidianDistance(coord[0],coord[1],targetCoords[0],targetCoords[1]));
    }

    auto it = std::min_element(euclidianVector.begin(),euclidianVector.end());

    size_t minIndex = it-euclidianVector.begin();

    return queue[minIndex];
}


std::vector<std::vector<int>> getAdjacentCoords(int x, int y, int width, int height){

    std::vector<std::vector<int>> adjacentCoords;

    if(x-1 >= 0){
        adjacentCoords.push_back({x-1,y});
    }

    if(x+1 < width){
        adjacentCoords.push_back({x+1,y});
    }

    if(y-1 >=0){
        adjacentCoords.push_back({x,y-1});
    }

    if(y+1 < height){
        adjacentCoords.push_back({x,y+1});
    }

    return adjacentCoords;

}

bool coordIn(const std::vector<int> &coord, const std::vector<std::vector<int>> &seenVector){
    for(auto x: seenVector){
        if(coord==x){
            return true;
        }
    }
    return false;
}
