#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <vector>
#include <exception>
#include <algorithm>

const int DEFAULT = 0;
const int WALL = 1;
const int TARGET = 2;
const int START = 3;
const int SEEN = 4;
const int PATH = 5;

const int start_x=2,start_y=2;
const int target_x=22,target_y=20;

float euclidianDistance(float x1, float y1, float x2, float y2){
    float dx = x2-x1;
    float dy = y2-y1;

    return sqrt(dx*dx + dy*dy);
}

std::vector<int> minEuclidianCoord(const std::vector<std::vector<int>> &queue){

    std::vector<float> euclidianVector;

    for(auto coord: queue){
        euclidianVector.push_back(euclidianDistance(coord[0],coord[1],target_x,target_y));
    }

    auto it = std::min_element(euclidianVector.begin(),euclidianVector.end());

    size_t minIndex = it-euclidianVector.begin();

    return queue[minIndex];
}

std::vector<std::vector<int>> getAdjacentCoords(int i, int j, int width, int height){

    std::vector<std::vector<int>> adjacentCoords;

    if(i-1 >= 0){
        adjacentCoords.push_back({i-1,j});
    }

    if(i+1 < width){
        adjacentCoords.push_back({i+1,j});
    }

    if(j-1 >=0){
        adjacentCoords.push_back({i,j-1});
    }

    if(j+1 < height){
        adjacentCoords.push_back({i,j+1});
    }

    return adjacentCoords;

}


std::vector<std::vector<int>> initialiseGrid(int columns, int rows, int defaultVal){

    std::vector<int> subArray(columns,defaultVal);
    std::vector<std::vector<int>> grid(rows,subArray);

    grid[start_y][start_x] = START;
    grid[target_y][target_x] = TARGET;

    return grid;

}

std::vector<std::vector<std::vector<int>>> initialisePathMemory(const std::vector<std::vector<int>> &grid){

    //Creates a second array which tells the program where the previous visited point is.

    std::vector<int> coords(2,0);
    std::vector<std::vector<int>> subArray(grid[0].size(),coords);
    std::vector<std::vector<std::vector<int>>> pathMemory(grid.size(),subArray);

    return pathMemory;
}

//Do breadth first search first
//Follow through with A* - same but nodes are weighted with euclidian distance

//Start at start_x, start_y
//Explore all neighbouring nodes -> check for target
//Explore all nodes that neighbour those nodes
//Repeat until target found

//Have a list which stores the previous node traversed to
//When new coord added to queue, add a value to pathMemory that contains the value the queue was generated from.

std::vector<std::vector<int>> drawCoords(std::vector<std::vector<int>>& grid, std::vector<std::vector<int>> coordVector){

    for(auto coord: coordVector){

        if(coord[1] == start_y && coord[0] == start_x){
            continue;
        }

        if(coord[1] == target_y && coord[0] == target_x){
            continue;
        }

        grid[coord[1]][coord[0]] = SEEN;
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
    std::vector<std::vector<int>> queue{{start_x,start_y}};
    std::vector<std::vector<int>> finalPath;
    bool targetFound = false;

    const int NUM_ITERATIONS = 100;
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

                //If adjacent coord is a target, end.
                if(adjCoord[1] == target_x && adjCoord[0]==target_y){
                    targetFound == true;
                    std::cout << targetFound << std::endl;
                    std::cout << adjCoord[0] << " " << adjCoord[1] << std::endl;

                    // drawCoords(grid, visitedCoords); //also draw tmp?
                    drawCoords(grid, finalPath);
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

        finalPath.push_back( minEuclidianCoord(queue) );

        if (tmp.size() == 0){
            std::cout << "NO TARGET FOUND" << std::endl;
            return {};
        }

        queue = tmp;

        for (auto x: queue){
            float r = euclidianDistance(start_x,start_y,x[0],x[1]);
            if(tmp.size() == 0){
                throw -1;
            }
            // std::cout << r << std::endl;
        }
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
