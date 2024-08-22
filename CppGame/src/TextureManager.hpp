//#ifndef TextureManager_hpp
//#define TextureManager_hpp
#pragma once

#include "Game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>


class TextureManager {

public: 

     static SDL_Texture* LoadTexture(const char* fileName);

     static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);


private: 

};

//#endif 
