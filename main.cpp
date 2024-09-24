#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <vector>

const int DEFAULT = 0;
const int WALL = 1;
const int TARGET = 2;
const int START = 3;

const int start_x=3,start_y=2;
const int target_x=15,target_y=15;

std::vector<std::vector<int>> initialiseGrid(int columns, int rows, int defaultVal){


    std::vector<int> subArray(columns,defaultVal);
    std::vector<std::vector<int>> grid(rows,subArray);

    grid[start_y][start_x] = START;
    grid[target_y][target_x] = TARGET;

    return grid;

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

        for(int i=0;i<grid.size();i++){
            for(int j=0;j<grid[0].size();j++){
                SDL_Rect rect{PIECE_SIZE*j + 1, PIECE_SIZE*i + 1, PIECE_SIZE, PIECE_SIZE};

                if(drawMode){
                    SDL_GetMouseState(&mousePos.x, &mousePos.y);
                    if(SDL_PointInRect(&mousePos,&rect) && grid[i][j] != TARGET && grid[i][j] != START){
                        grid[i][j] = WALL;
                    }
                }

                else if (eraseMode){
                    SDL_GetMouseState(&mousePos.x, &mousePos.y);
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