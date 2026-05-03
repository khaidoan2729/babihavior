//#ifndef Game_hpp
//#define Game_hpp
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "ECS/ECS.hpp"
#include "Collision.hpp"

class ColliderComponent;

class Game {

public: 
     Game();
     ~Game();

     void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
     void handleEvents();
     void update();
     void render();
     void clean();

     /* Let program know if the game is still running, if yes continue loop */
     bool running();  

     static SDL_Renderer* renderer;
     static SDL_Event event;
     static std::vector<ColliderComponent*> colliders;
     static bool isRunning;
     static int gameWidth;
     static int gameHeight;
     static int tileWidth;
     static int tileHeight;
     static void AddGround(const char* path, int y);

private: 
     SDL_Window *window;
     int cnt;

     void isPlayer(Entity* e);
     void GetCoin(Entity* player, Entity* coin);
     void handleCollideColliders(Entity* player, std::vector<Entity*>& collides);

     void setBoundaries();
     void handleCollideBoundaries(Entity*player, std::vector<Entity*>& bounds);
     void handleEnemies(Entity*player, std::vector<Entity*>& enemies);
     void addTiles();
};

//#endif // DEBUG
