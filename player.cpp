#include "player.h"

player::player(int x, int y, SDL_Rect &rect,SDL_Rect (&rects)[MAPSIZE][MAPSIZE], SDL_Point (&map)[MAPSIZE][MAPSIZE]) :
        posX(x), posY(y),playerRect(&rect),mapRects(rects), mapPoints(map) {}

void player::handleMovement(SDL_Keycode key) {
    int newX = posX;
    int newY = posY;

    if (key == SDLK_w) {
        newY -= 1;
        if(newY < 1){
            newY = 1;
        }
    } else if (key == SDLK_a) {
        newX -= 1;
        if(newX < 1){
            newX = 1;
        }
    } else if (key == SDLK_s) {
        newY += 1;
        if(newY > MAPSIZE-2){
            newY = MAPSIZE-2;
        }
    } else if (key == SDLK_d) {
        newX += 1;
        if(newX > MAPSIZE-2){
            newX = MAPSIZE-2;
        }
    }
    playerRect = &mapRects[newX][newY];
    mapPoints[posX][posY] = {32*4,0};
    mapPoints[newX][newY] = {0,0};
    posX = newX;
    posY = newY;

}
