#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include "player.h"

using namespace std;

const int TILE_SIZE = 32;
const int SCREENW = 1000;
const int SCREENH = 600;
const char del = ',';
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

bool isWall(SDL_Point p) {
    return (p.x == 32 && p.y == 192);
}

bool isPlayer(SDL_Point p) {
    return (p.x == 0 && p.y == 0) || ((p.x <= 7 * 32) && (p.y >= 32 && p.y <= 5 * 32));
}


void convertToFile(SDL_Point arr[MAPSIZE][MAPSIZE]) {
    std::string output = "";
    int index;
    for (int y = 0; y < MAPSIZE; y++) {
        for (int x = 0; x < MAPSIZE; x++) {
            index = (arr[x][y].x / 32) + (arr[x][y].y / 32) * 9;
            output += to_string(index) + ",";
        }
        output.pop_back();
        output += "\n";
    }
    std::ofstream outFile("output.txt");

    if (outFile.is_open()) {
        outFile << output;
        outFile.close();
        std::cout << "Text file has been created/overwritten with the content." << std::endl;
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}


void loadFile(SDL_Point arr[MAPSIZE][MAPSIZE], char deli) {
    std::ifstream file("output.txt");

    if (file.is_open()) {
        std::string line;
        int x, y;

        for (y = 0; y < MAPSIZE; y++) {
            for (x = 0; x < MAPSIZE; x++) {
                if (x < MAPSIZE - 1) {
                    std::getline(file, line, deli);
                } else {
                    std::getline(file, line);
                }
                int index = std::stoi(line);
                arr[x][y].x = (index % 9) * 32;
                arr[x][y].y = (index / 9) * 32;
            }
        }

        file.close();
    } else {
        std::cerr << "Unable to open the file." << std::endl;
    }
}

bool isFirstLast(int x, int y) {
    return (x == 0 || x == (MAPSIZE - 1) || y == 0 || y == (MAPSIZE - 1));
}

extern "C"
int main(int argc, char *args[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Tile Selector", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREENW,
                                          SCREENH,
                                          SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::string tilemapPath = "../../bd.png";
    SDL_Texture *tilemapTexture = loadTexture(tilemapPath, renderer);
    SDL_Rect tileRect = {0, 0, imgW, imgH};

    SDL_Rect selectcRect, srcRect;
    srcRect.w = srcRect.h = TILE_SIZE;
    selectcRect.w = selectcRect.h = TILE_SIZE;
    selectcRect.x = selectcRect.y = 0;

    SDL_Point mouse;

    player *p = nullptr;

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
    bool play = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p) {
                play = !play;
                const int xP = SCREENW / 2 - (MAPSIZE / 2) * 32;
                const int yP = SCREENH / 2 - (MAPSIZE / 2) * 32;
                for (int y = 0; y < MAPSIZE; y++) {
                    for (int x = 0; x < MAPSIZE; x++) {
                        mapRect[x][y].x = xP + x * 32;
                        mapRect[x][y].y = yP + y * 32;
                        if (play && isPlayer(mapPoint[x][y])) {
                            p = new player(x, y, MAPSIZE, mapRect[x][y], mapPoint);
                        }
                    }
                }
            } else if (play && p != nullptr) {
                if (e.type == SDL_KEYDOWN) {
                    p->handleMovement(e.key.keysym.sym);
                }
            } else if (!play) {
                if (e.type == SDL_MOUSEMOTION && (SDL_GetModState() & KMOD_SHIFT)) {
                    for (int y = 0; y < MAPSIZE; y++) {
                        for (int x = 0; x < MAPSIZE; x++) {
                            mapRect[x][y].x += e.motion.xrel;
                            mapRect[x][y].y += e.motion.yrel;
                        }
                    }

                } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_i) {
                    convertToFile(mapPoint);
                } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_o) {
                    loadFile(mapPoint, del);
                }
            }
        }
        if (!play) {
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
        }


        SDL_RenderClear(renderer);


        for (int y = 0; y < MAPSIZE; y++) {
            for (int x = 0; x < MAPSIZE; x++) {
                srcRect.x = mapPoint[x][y].x;
                srcRect.y = mapPoint[x][y].y;
                if (isFirstLast(x, y)) {
                    mapPoint[x][y].x = srcRect.x = 32;
                    mapPoint[x][y].y = srcRect.y = 6 * 32;
                }
                 /*SDL_Point playerCheck = {srcRect.x, srcRect.y};
                if (isPlayer(playerCheck)) {
                    cout << "y";
                } */
                SDL_RenderCopy(renderer, tilemapTexture, &srcRect, &mapRect[x][y]);
            }
        }
        if (!play)
            SDL_RenderCopy(renderer, tilemapTexture, NULL, &tileRect);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (!play)
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