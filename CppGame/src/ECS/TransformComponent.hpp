//#ifndef TransformComponent_hpp
//#define TransformComponent_hpp
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "ECS.hpp"
#include "Components.hpp"
#include "../Vector2D.hpp"


class TransformComponent : public Component {

public:
     Vector2D position;
     Vector2D velocity;

     int height = 64;
     int width = 64;
     int scale = 1;

     int speed = 4;

     TransformComponent () {     //default
          position.Zero();
     }

     TransformComponent (float x, float y) {
          position.x = x;
          position.y = y;
     }

     TransformComponent (int s) {
          position.Zero();
          scale = s;
     }

     TransformComponent (float x, float y, int w, int h, int s) {
          position.x = x;
          position.y = y;
          width =  w;
          height = h;
          scale = s;
     }


     void init() override {
          velocity.Zero();
     }

     void update() override {
          position.x += velocity.x * speed;
          position.y += velocity.y * speed;
     }


};

//#endif 
