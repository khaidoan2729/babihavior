//#ifndef KeyboardController_hpp
//#define KeyboardController_hpp
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "../Game.hpp"
#include "ECS.hpp"
#include "Components.hpp"




class KeyboardController : public Component {

public: 
     TransformComponent* transform;
     SpriteComponent* sprite;
     static int jumpCount; 
     static bool isLanded;


     void init() override {
          transform = &entity->getComponent<TransformComponent>();
          sprite = &entity->getComponent<SpriteComponent>();
          
     }

     void update() override {
          if (Game::event.type == SDL_KEYDOWN) {
               switch (Game::event.key.keysym.sym) {
               case SDLK_SPACE: 
                    jumpUp();
                    sprite->Play("Walk");
                    break;
               case SDLK_LEFT: 
                    //entity->isOnGround = false;
                    transform->velocity.x = -1;
                    //transform->velocity.y = 1;
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    sprite->Play("Walk");
                    break;
               case SDLK_DOWN: 
                    transform->velocity.y = 1;
                    sprite->Play("Walk");
                    break;
               case SDLK_RIGHT: 
                    transform->velocity.x = 1;
                    //entity->isOnGround = false;
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    sprite->Play("Walk");
                    break;
               case SDLK_ESCAPE: 
                    Game::isRunning = false;
               default: 
                    break;
               }
          }

          if (Game::event.type == SDL_KEYUP) {
               switch (Game::event.key.keysym.sym) {
               case SDLK_SPACE: 
                    //transform->velocity.y = 0;
                    //jumpDown();
                    sprite->Play("Idle");
                    break;
               case SDLK_LEFT:
                    entity->isOnGround = false;
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                    break;
               case SDLK_DOWN: 
                    transform->velocity.y = 0;
                    sprite->Play("Idle");
                    break;
               case SDLK_RIGHT: 
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                    break;
               default: 
                    break;
               }

          }
     } 

private:
     void jumpUp() {
          if (entity->isOnGround) {
               transform->position.y --;
               transform->velocity.y = -1.75;
               entity->isOnGround = false;
               entity->isJumping = true; 
          }
     }

     void jumpDown() {
          if (!entity->isOnGround && entity->jumpCount <= 10) {
               transform->velocity.y = -0.25;
               //entity->isOnGround = true;
               
          }
     }

};

//#endif 
