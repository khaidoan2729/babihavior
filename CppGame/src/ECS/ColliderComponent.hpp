//#ifndef ColliderComponent_hpp
//#define ColliderComponent_hpp
#pragma once


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "ECS.hpp"
#include "../Game.hpp"
#include "Components.hpp"

/* 
 * Collidable components class 
 */
class ColliderComponent : public Component {

     public: 
          SDL_Rect collider;
          std::string tag;
          TransformComponent* transform;

          /* Constructor */
          ColliderComponent (std::string t) {
               tag = t;
          }

          /* Init function. */
          void init() override {
               /* Add transform component if not there already. */
               if (!entity->hasComponent<TransformComponent>()) {
                    entity->addComponent<TransformComponent>();
               }
               transform = &entity->getComponent<TransformComponent>();
               Game::colliders.push_back(this);
          }

          /* Update component positione and size. */
          void update() override {

               collider.x = static_cast<int>(transform->position.x);
               collider.y = static_cast<int>(transform->position.y);
               collider.w = transform->width * transform->scale;
               collider.h = transform->height * transform->scale;
          }

     private: 

};

//#endif 
