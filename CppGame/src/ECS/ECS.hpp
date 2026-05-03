//#ifndef ECS_hpp
//#define ECS_hpp

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>


class Component; 
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;


/*
 * Every time we call this function we will get the next ID. 
 * Works for multiple types of components.
 */
inline ComponentID getNewComponentTypeID() {
     static ComponentID lastID = 0u;  /* unsigned*/ 
     return lastID++; 
}


/*
 * Get the new type ID for the component. 
 * Works for multiple types of components.
 */
template <typename T> inline ComponentID getComponentTypeID() noexcept {
     static ComponentID typeID = getNewComponentTypeID();
     return typeID;
}



constexpr std::size_t maxComponents = 32;               /* Maximum number of components that an entity can hold */
constexpr std::size_t maxGroups = 32;                   /* Maximum number of groups */


using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;


/*
 * Component class.
 */
class Component {
     public: 
          Entity *entity;
          virtual void init() {}
          virtual void update() {}
          virtual void draw() {}

          virtual ~Component() {}

};



/*
 * Entity class.
 */
class Entity {
     private: 
          Manager& manager;
          bool active = true;
          std::vector<std::unique_ptr<Component>> components;
          ComponentArray componentArray;
          ComponentBitSet componentBitSet;
          GroupBitSet groupBitSet;
     
     public:
          bool isOnGround = true;
          int jumpCount = 0;
          bool isJumping = false;

          /* Constructor */
          Entity(Manager& mManager) : manager(mManager) {
          }
          
          /* Update all components in this entity */
          void update() {
               for (auto& c : components) c->update();     
          }
          
          /* Draw all components in this entity */
          void draw() {
               for (auto& c : components) c->draw();       
          }

          /* If this entity is active */
          bool isActive() {  return active;  }

          /* Destroy this entity by making it inactive */
          void destroy() { active = false; }

          /* Return bit at position mGroup in bitset */
          bool hasGroup(Group mGroup) {
               return groupBitSet[mGroup];
          }

          void addGroup(Group mGroup);

          /* Remove group from bitset */
          void delGroup(Group mGroup) {
               groupBitSet[mGroup] = false;
          }

          /* Check if entity has a certain component using the component bit set */
          template <typename T> bool hasComponent() const {
               return componentBitSet[getComponentTypeID<T>()];
          }

          /* Add a component */
          template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
               T* c(new T(std::forward<TArgs>(mArgs)...));
               c->entity = this;
               std::unique_ptr<Component> uPtr{ c };
               components.emplace_back(std::move(uPtr));
               componentArray[getComponentTypeID<T>()] = c;
               componentBitSet[getComponentTypeID<T>()] = true;

               c->init();
               return *c;
          }
          
          /* Get a component */
          template <typename T> T& getComponent() const {
               auto ptr(componentArray[getComponentTypeID<T>()]);
               return *static_cast<T*>(ptr);
          }
};



/*
 * Manager class.
 */
class Manager {
     private: 
          /* Entities of of this manager */
          std::vector<std::unique_ptr<Entity>> entities;
          /* Grouped entities, array of vectors */
          std::array<std::vector<Entity*>, maxGroups> groupedEntities;

     public:

          /* Update all entities */
          void update() {
               for (auto& e : entities) e->update();
          }

          /* Draw all entities */
          void draw() {
               for (auto& e : entities) e->draw();
          }

          /* 
           * Go through each vector in every group, erase entities that are not active or does not 
           * belong to the same group. Do the same for the entities vector attribute.  
           */
          void refresh() {
               for (unsigned int i = 0; i < maxGroups; i++) {
                    auto& v(groupedEntities[i]);
                    v.erase(
                         std::remove_if(
                              std::begin(v), std::end(v), 
                              [i](Entity* mEntity) {
                                   return !mEntity->isActive() || !mEntity->hasGroup(i);
                              }
                         ),
                         std::end(v)
                    );
               }

               entities.erase(
                    std::remove_if(
                         std::begin(entities), 
                         std::end(entities), 
                         [](const std::unique_ptr<Entity> &mEntity) { 
                              return !mEntity->isActive();
                         }
                    ), 
                    std::end(entities)
               );
          }

          /* Add entity to the a group */
          void AddToGroup(Entity* mEntity, Group mGroup) {
               groupedEntities[mGroup].emplace_back(mEntity);
          }

          /* Get a group from the group array */
          std::vector<Entity*>& getGroup(Group mGroup) {
               return groupedEntities[mGroup];
          }

          /* Add a new entity to the entities vector */
          Entity& addEntity() {
               Entity* e = new Entity(*this);
               std::unique_ptr<Entity> uPtr{ e };
               entities.emplace_back(std::move(uPtr));
               return *e;
          }

};



//#endif 
