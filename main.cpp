#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <vector>
#include <exception>
#include <algorithm>
#include <climits>

const int DEFAULT = 0;
const int WALL = 1;
const int TARGET = 2;
const int START = 3;
const int SEEN = 4;
const int PATH = 5;

const int START_X=2,START_Y=2;
const int TARGET_X=22,TARGET_Y=20;

float euclidianDistance(float x1, float y1, float x2, float y2){
    float dx = x2-x1;
    float dy = y2-y1;

    return sqrt(dx*dx + dy*dy);
}

std::vector<int> minEuclidianCoord(const std::vector<std::vector<int>> &queue){

    std::vector<float> euclidianVector;

    for(auto coord: queue){
        euclidianVector.push_back(euclidianDistance(coord[0],coord[1],TARGET_X,TARGET_Y));
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

std::vector<std::vector<int>> initialiseGrid(int columns, int rows, int defaultVal){

    std::vector<int> subArray(columns,defaultVal);
    std::vector<std::vector<int>> grid(rows,subArray);

    grid[START_Y][START_X] = START;
    grid[TARGET_Y][TARGET_X] = TARGET;

    return grid;
}

std::vector<std::vector<std::vector<int>>> initialisePathMemory(const std::vector<std::vector<int>> &grid){

    //Creates a second array which tells the program where the previous visited point is.

    std::vector<int> coords(2,0);
    std::vector<std::vector<int>> subArray(grid[0].size(),coords);
    std::vector<std::vector<std::vector<int>>> pathMemory(grid.size(),subArray);

    pathMemory[START_Y][START_X] = {INT_MAX,INT_MAX};

    return pathMemory;
}

std::vector<std::vector<int>> getFinalPath(const std::vector<std::vector<std::vector<int>>>& pathMemory){

    std::vector<std::vector<int>> finalPath;

    std::vector<int> currentCoord = pathMemory[TARGET_Y][TARGET_X];

    std::cout << currentCoord[0] << currentCoord[1] << std::endl;

    size_t MAX_ITER = 100;
    size_t iter = 0;

    while (currentCoord[0] != START_X && currentCoord[1] != START_Y){
        finalPath.push_back(currentCoord);
        currentCoord = pathMemory[currentCoord[1]][currentCoord[0]];
        iter ++;

        if(iter >= MAX_ITER){
            return finalPath;
        }
    }

    return finalPath;
}

std::vector<std::vector<int>> drawCoords(std::vector<std::vector<int>>& grid, const std::vector<std::vector<int>> &coordVector, const int& COLOUR){

    for(auto coord: coordVector){

        if(coord[1] == START_Y && coord[0] == START_X){
            continue;
        }

        if(coord[1] == TARGET_Y && coord[0] == TARGET_X){
            continue;
        }

        grid[coord[1]][coord[0]] = COLOUR;
    }

    return grid;
}

bool coordSeen(std::vector<int> coord,std::vector<std::vector<int>> seenVector){
    for(auto x: seenVector){
        if(coord==x){
            return true;
        }
    }
    return false;
}

std::vector<std::vector<int>> breadthFirst(std::vector<std::vector<int>> grid, int width, int height){

    std::vector<std::vector<int>> visitedCoords;
    std::vector<std::vector<int>> queue{{START_X,START_Y}};
    std::vector<std::vector<int>> finalPath;
    bool targetFound = false;

    const int NUM_ITERATIONS = INT_MAX;
    int itCounter = 0;

    auto pathMemory = initialisePathMemory(grid); //Pick a better variable name for this

    while(!targetFound && (itCounter < NUM_ITERATIONS) ){
        std::vector<std::vector<int>> tmp;
        for(int i = 0; i< queue.size();i++){
            auto coord = queue[i];

            if ( coordSeen(coord,visitedCoords) ){
                continue;
            }

            auto adjacentTiles = getAdjacentCoords(coord[1],coord[0], width, height);
            for(auto adjCoord: adjacentTiles){


                pathMemory[adjCoord[1]][adjCoord[0]] = coord; //Sets adjacent coord's parent as coord.

                //If adjacent coord is a target, end.
                if(adjCoord[1] == TARGET_Y && adjCoord[0]==TARGET_X){
                    targetFound == true;
                    std::cout << "Target Found" << std::endl;

                    // drawCoords(grid, visitedCoords); //also draw tmp?

                    auto finalPath = getFinalPath(pathMemory);

                    drawCoords(grid, finalPath,PATH);
                    return grid;
                }

                //If adjacent coord already in queue, continue
                if(coordSeen(adjCoord,queue)|| coordSeen(adjCoord,tmp)){
                    continue;
                }

                tmp.push_back(adjCoord);
            }
        }
        visitedCoords.insert(visitedCoords.end(),queue.begin(),queue.end());

        if (tmp.size() == 0){
            std::cout << "NO TARGET FOUND" << std::endl;
            return {};
        }

        queue = tmp;

        tmp = {};
        itCounter ++ ;
    }

    return {};
}


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

int main(){

    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    int WINDOW_WIDTH = 800,
        WINDOW_HEIGHT = 800;

    SDL_Window *win = SDL_CreateWindow("PathFinder", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Event event;


    int columns=25,rows=25;
    std::vector<std::vector<int>> grid = initialiseGrid(columns,rows,DEFAULT);

    int PIECE_SIZE=30;
    bool drawMode=false;
    bool eraseMode=false;
    bool exit = false;
    SDL_Point mousePos;

    grid = breadthFirst(grid,columns,rows);

    while(!exit){
        while (SDL_PollEvent(&event))
        {
            switch(event.type){

                case SDL_QUIT:
                    exit = true;
                    break;

                //Fix this to distinguish between lmb and rmb

                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_r){
                        grid = initialiseGrid(columns,rows,DEFAULT);
                    }

                case SDL_MOUSEBUTTONDOWN:

                    if(event.button.button == SDL_BUTTON_LEFT){
                        drawMode = true;
                    }

                    else if(event.button.button == SDL_BUTTON_RIGHT){
                        eraseMode = true;
                    }

                    break;

                case SDL_MOUSEBUTTONUP:

                    if(event.button.button == SDL_BUTTON_LEFT){
                        drawMode = false;
                    }

                    else if(event.button.button == SDL_BUTTON_RIGHT){
                        eraseMode = false;
                    }

                    break;

            }
            
        }

        // Fills screen with black
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 1);
        SDL_RenderClear(rend);


        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        for(int i=0;i<grid.size();i++){
            for(int j=0;j<grid[0].size();j++){
                SDL_Rect rect{PIECE_SIZE*j + 1, PIECE_SIZE*i + 1, PIECE_SIZE, PIECE_SIZE};

                if(drawMode){
                    if(SDL_PointInRect(&mousePos,&rect) && grid[i][j] != TARGET && grid[i][j] != START){
                        grid[i][j] = WALL;
                    }
                }

                else if (eraseMode){
                    if(SDL_PointInRect(&mousePos,&rect)&& grid[i][j] != TARGET && grid[i][j] != START){
                        grid[i][j] = DEFAULT;
                    }
                }
                

                std::vector<int> rgb = getCellColour(grid[i][j]);

                SDL_SetRenderDrawColor(rend, rgb[0], rgb[1], rgb[2], 1);
                SDL_RenderDrawRect(rend, &rect);
                SDL_RenderFillRect(rend, &rect);
            }
        }

        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

    }

    return 0;
}
