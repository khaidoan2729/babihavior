#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "Components.hpp"
#include "../TextureManager.hpp"
#include "Animation.hpp"
#include <map>


class SpriteComponent : public Component {

     private: 
          TransformComponent* transform; 
          SDL_Texture* texture;
          SDL_Rect srcRect, destRect;

          bool animated = false;
          int frames = 0;
          int speed = 100;

     public: 
          int animIndex = 0;

          std::map<const char*, Animation> animations;

          SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

          SpriteComponent() = default;

          SpriteComponent(const char* path) {
               setTexture(path);
          }

          /* Constructor for sprite component. 
           * TODO: might need rework for multiple components
           */
          SpriteComponent(const char* path, bool isAnimated, int id) {
               animated = isAnimated;
               int idleFrame = 2;

               if (id == 1) idleFrame = 6;
               else if (id == 2) idleFrame = 4;

               Animation idle = Animation(0, idleFrame, 100);
               animations.emplace("Idle", idle);
               if (id == 0) {
                    Animation walk = Animation(1, 4, 100);
                    animations.emplace("Walk", walk); 
               }
               if (id == 2) {
                    Animation attack = Animation(1, 5, 100);
                    animations.emplace("Attack", attack);
               }
               Play("Idle");
               setTexture(path);
          }

          /* Destructor */
          ~SpriteComponent() {
               SDL_DestroyTexture(texture);

          }

          /* Set texture to load sprite images using SDL. */
          void setTexture(const char* path) {
               texture = TextureManager::LoadTexture(path);
          }

          /* Init function. */
          void init() override {
               transform = &entity->getComponent<TransformComponent>();
               srcRect.x = srcRect.y = 0;
               srcRect.w = transform->width;
               srcRect.h = transform->height;
          }

          /* Update function which calculate the destination rectangle to move the image toward. */
          void update() override {
               if (animated) {
                    srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
               }
               srcRect.y = animIndex * transform->height;
               destRect.x = static_cast<int>(transform->position.x);
               destRect.y = static_cast<int>(transform->position.y);
               destRect.w = transform->width * transform->scale;
               destRect.h = transform->height * transform->scale;
          }

          /* Draw function using SDL. */
          void draw() override {
               TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
          }

          /* Play different states of a component, based on key of animations map. */
          void Play(const char* animName) {
               frames = animations[animName].frames;
               animIndex = animations[animName].index;
               speed = animations[animName].speed;
          }
};

//#endif 
