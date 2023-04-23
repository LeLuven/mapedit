#include "player.h"

player::player(int x, int y, int mapSize, SDL_Rect &rect, SDL_Point (&map)[MAPSIZE][MAPSIZE]) :
        posX(x), posY(y), mapSize(mapSize), playerRect(&rect), mapPoints(map) {}

void player::handleMovement(SDL_Keycode key) {
    int newX = posX;
    int newY = posY;

    if (key == SDLK_w) {
        newY -= 1;
    } else if (key == SDLK_a) {
        newX -= 1;
    } else if (key == SDLK_s) {
        newY += 1;
    } else if (key == SDLK_d) {
        newX += 1;
    }

    if (newX >= 0 && newX < mapSize && newY >= 0 && newY < mapSize) {
        posX = newX;
        posY = newY;
        playerRect->x = mapPoints[posX][posY].x;
        playerRect->y = mapPoints[posX][posY].y;
    }
}
