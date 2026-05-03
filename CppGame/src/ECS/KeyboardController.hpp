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

          /* Init function, getting transform and entity components*/
          void init() override {
               transform = &entity->getComponent<TransformComponent>();
               sprite = &entity->getComponent<SpriteComponent>();  
          }

          /* Update function, catching key interrupt and doing corresponding action. */
          void update() override {
               /* Key press event */
               if (Game::event.type == SDL_KEYDOWN) {
                    switch (Game::event.key.keysym.sym) {
                         case SDLK_UP: 
                              jumpUp();
                              sprite->Play("Walk");
                              break;
                         case SDLK_LEFT: 
                              transform->velocity.x = -1;
                              sprite->spriteFlip = SDL_FLIP_HORIZONTAL;    /* Flip when moving horizontaly */
                              sprite->Play("Walk");
                              break;
                         case SDLK_RIGHT: 
                              transform->velocity.x = 1;
                              sprite->spriteFlip = SDL_FLIP_NONE;
                              sprite->Play("Walk");
                              break;
                         case SDLK_ESCAPE: 
                              Game::isRunning = false;
                         default: 
                              break;
                    }
               }

               /* Key release event */
               if (Game::event.type == SDL_KEYUP) {
                    switch (Game::event.key.keysym.sym) {
                         case SDLK_UP: 
                              sprite->Play("Idle");
                              break;
                         case SDLK_LEFT:
                              entity->isOnGround = false;
                              transform->velocity.x = 0;
                              sprite ->Play("Idle");
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

          /* Execute "jump up" action, only when the component is on the ground and not jumping. */
          void jumpUp() {
               if (entity->isOnGround && !entity->isJumping) {
                    transform->position.y --;
                    transform->velocity.y = -1.75;
                    entity->isOnGround = false;
                    entity->isJumping = true; 
               }
          }

          /* Execute "jump down" action. */
          void jumpDown() {
               if (!entity->isOnGround && entity->jumpCount <= 10) {
                    transform->velocity.y = -0.25;                    
               }
          }

};

//#endif 
