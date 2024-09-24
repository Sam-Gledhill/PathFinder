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
        rgbVector = {0,255,0};
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

    std::vector<std::vector<int>> grid 
    {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
    };

    int PIECE_SIZE=50;
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
                    if(SDL_PointInRect(&mousePos,&rect)){
                        grid[i][j] = 1;
                    }
                }

                else if (eraseMode){
                    SDL_GetMouseState(&mousePos.x, &mousePos.y);
                    if(SDL_PointInRect(&mousePos,&rect)){
                        grid[i][j] = 0;
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