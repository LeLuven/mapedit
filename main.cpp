#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <array>
#include <vector>

using namespace std;

const int TILE_SIZE = 32;
const int MAPSIZE = 15;
int imgW, imgH;

SDL_Texture *loadTexture(const std::string &path, SDL_Renderer *renderer) {
    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = IMG_Load(path.c_str());

    if (surface == nullptr) {
        std::cout << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_QueryTexture(texture, NULL, NULL, &imgW, &imgH);
        SDL_FreeSurface(surface);
    }

    return texture;
}

bool isFirstLast(int x, int y){
    return (x == 0 || x == (MAPSIZE-1) || y == 0 || y == (MAPSIZE-1));
}

extern "C"
int main(int argc, char *args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Tile Selector", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 600,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::string tilemapPath = "../../bd.png";
    SDL_Texture *tilemapTexture = loadTexture(tilemapPath, renderer);
    SDL_Rect tileRect = {0, 0, imgW, imgH};

    SDL_Rect selectcRect, srcRect;
    srcRect.w = srcRect.h = TILE_SIZE;
    selectcRect.w = selectcRect.h = TILE_SIZE;

    SDL_Point mouse;

    SDL_Point mapPoint[MAPSIZE][MAPSIZE];
    SDL_Rect mapRect[MAPSIZE][MAPSIZE];

    for (int y = 0; y < MAPSIZE; y++) {
        for (int x = 0; x < MAPSIZE; x++) {
            mapRect[x][y] = {x * TILE_SIZE + imgW, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            mapPoint[x][y].x = 1 * TILE_SIZE;
            mapPoint[x][y].y = 7 * TILE_SIZE;
        }
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEMOTION && (SDL_GetModState() & KMOD_SHIFT)){
                for (int y = 0; y < MAPSIZE; y++) {
                    for (int x = 0; x < MAPSIZE; x++) {
                       mapRect[x][y].x += e.motion.xrel;
                        mapRect[x][y].y +=e.motion.yrel;
                    }
                }

            }


        }

        Uint32 mouseState = SDL_GetMouseState(NULL, NULL);
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            mouse.x = x;
            mouse.y = y;

            if (x >= 0 && x <= imgW && y >= 0 && y <= imgH) {
                cout << x / TILE_SIZE << ":" << y / TILE_SIZE << endl;
                selectcRect.x = (x / TILE_SIZE) * TILE_SIZE;
                selectcRect.y = (y / TILE_SIZE) * TILE_SIZE;
            }

            for (int y = 0; y < MAPSIZE; y++) {
                for (int x = 0; x < MAPSIZE; x++) {
                    if (SDL_PointInRect(&mouse, &mapRect[x][y])) {
                        mapPoint[x][y].x = selectcRect.x;
                        mapPoint[x][y].y = selectcRect.y;
                    }
                }
            }
        }

        SDL_RenderClear(renderer);


        for (int y = 0; y < MAPSIZE; y++) {
            for (int x = 0; x < MAPSIZE; x++) {
                srcRect.x = mapPoint[x][y].x;
                srcRect.y = mapPoint[x][y].y;
                if(isFirstLast(x,y)){
                    srcRect.x = 32;
                    srcRect.y = 6 * 32;
                }
                SDL_RenderCopy(renderer, tilemapTexture, &srcRect, &mapRect[x][y]);
            }
        }
        SDL_RenderCopy(renderer, tilemapTexture, NULL, &tileRect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &selectcRect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(tilemapTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}