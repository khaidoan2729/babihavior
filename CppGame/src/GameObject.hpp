#ifndef GameObject_hpp
#define GameObject_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "Game.hpp"


class GameObject {

public: 
     GameObject(const char* texturesheet, int x, int y);   //Constructor
     ~GameObject();  //Destructor

     void Update();
     void Render();

private: 
     // x,y position to draw on screen
     int xpos;
     int ypos;
      
     SDL_Texture* objTexture;
     SDL_Rect srcRect, destRect;
 
};

#endif 