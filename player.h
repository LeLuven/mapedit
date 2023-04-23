//
// Created by Jonas Luven on 23.04.23.
//
#include <SDL.h>
#ifndef MAPEDITOR_PLAYER_H
#define MAPEDITOR_PLAYER_H

const int MAPSIZE = 16;

class player {
public:
    player(int x, int y, int mapSize, SDL_Rect &rect, SDL_Point (&map)[MAPSIZE][MAPSIZE]);

    void handleMovement(SDL_Keycode key);

private:
    int posX, posY;
    int mapSize;
    SDL_Rect *playerRect;
    SDL_Point (&mapPoints)[MAPSIZE][MAPSIZE];
};

#endif //MAPEDITOR_PLAYER_H
