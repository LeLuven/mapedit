//
// Created by Jonas Luven on 23.04.23.
//
#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>

const int MAPSIZE = 10;

class player {
public:

    player(int x, int y,SDL_Rect &rect, SDL_Rect (&rects)[MAPSIZE][MAPSIZE], SDL_Point (&map)[MAPSIZE][MAPSIZE]);

    void handleMovement(SDL_Keycode key);

private:
    int posX, posY;
    int mapSize;
    SDL_Rect * playerRect;
    SDL_Point (*mapPoints)[MAPSIZE];
    SDL_Rect (*mapRects)[MAPSIZE];
};

#endif
