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
     Game();   //Constructor
     ~Game();  //Destructor

     void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
     void handleEvents();
     void update();
     void render();
     void clean();
     bool running();   // Let program know if the game is still running => continue loop

     static SDL_Renderer* renderer;
     static SDL_Event event;
     static std::vector<ColliderComponent*> colliders;
     static void AddTile(int id, int x, int y);
     static bool isRunning;
     static int gameWidth;
     static int gameHeight;
     static void AddGround(const char* path, int y);

private: 
     //bool isRunning;
     SDL_Window *window;
     int cnt;

     void isPlayer(Entity* e);
     void Land(Entity* player, std::vector<Entity*>& grounds);
     void GetCoin(Entity* player, Entity* coin);
     void handleJump(Entity *player);
     void handleCollide(Entity* player, std::vector<Entity*>& collides);

};

//#endif // DEBUG
