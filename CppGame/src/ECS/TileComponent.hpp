//#ifndef TileComponent_hpp
//#define TileComponent_hpp
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "ECS.hpp"
#include "Components.hpp"


class TileComponent : public Component {

public: 
     TransformComponent* transform;
     SpriteComponent* sprite; 
     int tileID;
     SDL_Rect tileRect;

     const char* path;

     TileComponent() = default;

     TileComponent(int x, int y, int w, int h, int id) {
          tileRect.x = x;
          tileRect.y = y;
          tileRect.w = w;
          tileRect.h = h;
          tileID = id;

          if (id == 0) {
               path = "assets/world/bg_water_64x64.png";
          } else if (id == 1) {
               path = "assets/world/bg_dirt_64x64.png";
          } else if (id == 2) {
               path = "assets/world/bg_grass_64x64.png";
          }

     }

     void init() override {
          entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1);
          transform = &entity->getComponent<TransformComponent>();
          entity->addComponent<SpriteComponent>(path);
          sprite = &entity->getComponent<SpriteComponent>();

     }

private: 

};

//#endif 
