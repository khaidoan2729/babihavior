//#ifndef SpriteComponent_hpp
//#define SpriteComponent_hpp
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

     SpriteComponent(const char* path, bool isAnimated, int id) {
          animated = isAnimated;
          int idleFrame = 2;

          if (id == 1) idleFrame = 6; //

          Animation idle = Animation(0,idleFrame,100);
          animations.emplace("Idle", idle);
          if (id  == 0) {
               Animation walk = Animation(1,4,100);
               animations.emplace("Walk", walk); 
          }
          Play("Idle");
          setTexture(path);
     }

     ~SpriteComponent() {
          SDL_DestroyTexture(texture);

     }

     void setTexture(const char* path) {
          texture = TextureManager::LoadTexture(path);
     } 



     void init() override {

          transform = &entity->getComponent<TransformComponent>();

          srcRect.x = srcRect.y = 0;
          srcRect.w = transform->width;
          srcRect.h = transform->height;
     }

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

     void draw() override {
          TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
     }


     void Play(const char* animName) {
          frames = animations[animName].frames;
          animIndex = animations[animName].index;
          speed = animations[animName].speed;
     }

};

//#endif 
