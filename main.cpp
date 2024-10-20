#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <vector>
#include <climits>

#include "colours.h"
#include "vector-operations.h"

std::vector<int> startCoords = {2,2};
std::vector<int> targetCoords = {22,22};


std::vector<std::vector<int>> initialiseGrid(int columns, int rows, int defaultVal, const std::vector<int> &startCoords, const std::vector<int> &targetCoords){

    std::vector<int> subArray(columns,defaultVal);
    std::vector<std::vector<int>> grid(rows,subArray);

    grid[startCoords[1]][startCoords[0]] = START;
    grid[targetCoords[1]][targetCoords[0]] = TARGET;

    return grid;
}

std::vector<std::vector<std::vector<int>>> initialisePathMemory(const std::vector<std::vector<int>> &grid){

    //Creates a second array which tells the program where the previous visited point is.

    std::vector<int> coords(2,-1);
    std::vector<std::vector<int>> subArray(grid[0].size(),coords);
    std::vector<std::vector<std::vector<int>>> pathMemory(grid.size(),subArray);

    return pathMemory;
}

std::vector<std::vector<int>> getFinalPath(const std::vector<std::vector<std::vector<int>>>& pathMemory, const std::vector<int> &startCoords, const std::vector<int> &targetCoords){

    std::vector<std::vector<int>> finalPath;

    std::vector<int> currentCoord = pathMemory[targetCoords[1]][targetCoords[0]];

    size_t MAX_ITER = INT_MAX;
    size_t iter = 0;

    while (!(currentCoord[0] == startCoords[0] && currentCoord[1] == startCoords[1])){
        finalPath.push_back(currentCoord);
        currentCoord = pathMemory[currentCoord[1]][currentCoord[0]];
        iter ++;

        if(iter >= MAX_ITER){
            std::cout << "PATH NOT FOUND" << std::endl;
            return finalPath;
        }
    }

    return finalPath;
}

std::vector<std::vector<int>> breadthFirst(std::vector<std::vector<int>> grid, const std::vector<int> &startCoords, const std::vector<int> &targetCoords,int width, int height){

    std::vector<std::vector<int>> visitedCoords;
    std::vector<std::vector<int>> queue{{startCoords[0],startCoords[1]}};
    std::vector<std::vector<int>> finalPath;
    bool targetFound = false;

    const int NUM_ITERATIONS = INT_MAX;
    int itCounter = 0;

    auto pathMemory = initialisePathMemory(grid); //Pick a better variable name for this

    while(!targetFound && (itCounter < NUM_ITERATIONS) ){
        std::vector<std::vector<int>> _toAddToQueue;
        for(int i = 0; i< queue.size();i++){
            auto coord = queue[i];

            if ( coordIn(coord,visitedCoords) ){
                continue;
            }

            auto adjacentTiles = getAdjacentCoords(coord[0],coord[1], width, height);
            for(auto adjCoord: adjacentTiles){

                //abstract this logic
                if(coordIn(adjCoord,_toAddToQueue) || pathMemory[adjCoord[1]][adjCoord[0]][0] != -1 || grid[adjCoord[1]][adjCoord[0]]==WALL){ //This probably needs to be here
                    continue;
                }

                pathMemory[adjCoord[1]][adjCoord[0]] = coord; //Sets adjacent coord's parent as coord.

                //If adjacent coord is a target, end.
                if(adjCoord == targetCoords){
                    targetFound == true;
                    std::cout << "Target Found" << std::endl;

                    auto finalPath = getFinalPath(pathMemory,startCoords,targetCoords);

                    drawCoords(grid, finalPath,PATH, startCoords, targetCoords);
                    return grid;
                }

                _toAddToQueue.push_back(adjCoord);
            }
        }
        visitedCoords.insert(visitedCoords.end(),queue.begin(),queue.end());

        if (_toAddToQueue.size() == 0){
            std::cout << "NO TARGET FOUND" << std::endl;
            return grid;
        }

        queue = _toAddToQueue;

        _toAddToQueue = {};
        itCounter ++ ;
    }

    std::cout << "Should not reach here" << std::endl;
    return {};
}

std::vector<std::vector<int>> recalculatePath(std::vector<std::vector<int>> grid, int rows, int columns){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(grid[i][j]== PATH){
                grid[i][j] = DEFAULT;
            }
        }
    }
    return breadthFirst(grid,startCoords,targetCoords,columns,rows);
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
    std::vector<std::vector<int>> grid = initialiseGrid(columns,rows,DEFAULT,startCoords,targetCoords);

    int PIECE_SIZE=30;

    bool ctrlModifier=false;
    bool movingStart=false;
    bool movingTarget=false;
    bool drawMode=false;
    bool eraseMode=false;
    bool exit = false;
    SDL_Point mousePos;

    grid = breadthFirst(grid,startCoords,targetCoords,columns,rows);

    Uint32 initialTime = SDL_GetTicks();
    Uint32 currentTime;
    Uint32 deltaTime;
    float FPS = 30.0;

    while(!exit){

        currentTime = SDL_GetTicks();
        deltaTime = currentTime - initialTime;

        if (deltaTime < 1000.0/FPS){
            continue;
        }   
        initialTime = currentTime;


        while (SDL_PollEvent(&event))
        {
            switch(event.type){

                case SDL_QUIT:
                    exit = true;
                    break;

                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_r){
                        grid = initialiseGrid(columns,rows,DEFAULT,startCoords,targetCoords);
                    }

                    else if (event.key.keysym.sym == SDLK_LCTRL){
                        ctrlModifier = true;
                    }

                    else if(event.key.keysym.sym == SDLK_f){
                        grid = recalculatePath(grid,rows,columns);

                    }

                    else if(event.key.keysym.sym == SDLK_q){
                        exit = true;
                    }

                    break;

                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_LCTRL){
                        ctrlModifier = false;
                    }
                    break;


                case SDL_MOUSEBUTTONDOWN:

                    if(event.button.button == SDL_BUTTON_LEFT){
                        if(ctrlModifier){
                            movingStart = true;
                        }
                        else{
                        drawMode = true;
                        }
                    }

                    else if(event.button.button == SDL_BUTTON_RIGHT){
                        if(ctrlModifier){
                            movingTarget = true;
                        }
                        else{
                            eraseMode = true;
                        }
                    }

                    break;

                case SDL_MOUSEBUTTONUP:


                    grid = recalculatePath(grid,rows,columns);


                    if(event.button.button == SDL_BUTTON_LEFT){
                        movingStart = false;
                        drawMode = false;
                    }

                    else if(event.button.button == SDL_BUTTON_RIGHT){
                        movingTarget = false;
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

                else if (movingStart){
                    if(SDL_PointInRect(&mousePos,&rect)&& grid[i][j] != TARGET){

                        grid[startCoords[1]][startCoords[0]] = DEFAULT;
                        startCoords = {j,i};
                        grid[i][j] = START;
                    }

                }

                else if (movingTarget){
                    if(SDL_PointInRect(&mousePos,&rect)&& grid[i][j] != START){
                        grid[targetCoords[1]][targetCoords[0]] = DEFAULT;
                        targetCoords = {j,i};
                        grid[i][j] = TARGET;
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
