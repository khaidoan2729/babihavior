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

inline ComponentID getNewComponentTypeID() {
     static ComponentID lastID = 0u; 
     return lastID++; 
     //Every time we call this function we will get the next ID
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {

     static ComponentID typeID = getNewComponentTypeID();
     return typeID;
}

constexpr std::size_t maxComponents = 32;               // Maximum number of components that an entity can hold
constexpr std::size_t maxGroups = 32;                   // Maximum number of groups 


using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;

class Component {
public: 
     Entity *entity;
     virtual void init() {}
     virtual void update() {}
     virtual void draw() {}

     virtual ~Component() {}

};


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

     Entity(Manager& mManager) : manager(mManager) {
     }

     void update() {
          for (auto& c : components) c->update();     // Update all components in the entity
     }
     
     void draw() {
          for (auto& c : components) c->draw();       // Then draw
     }

     bool isActive() {  return active;  }
     void destroy() { active = false; }

     bool hasGroup(Group mGroup) {
          return groupBitSet[mGroup];
     }

     void addGroup(Group mGroup);

     void delGroup(Group mGroup) {
          groupBitSet[mGroup] = false;
     }

     template <typename T> bool hasComponent() const {
          return componentBitSet[getComponentTypeID<T>()];
     }

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

     template <typename T> T& getComponent() const {
          auto ptr(componentArray[getComponentTypeID<T>()]);
          return *static_cast<T*>(ptr);
     }
};


class Manager {
private: 
     std::vector<std::unique_ptr<Entity>> entities;
     std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
     void update() {
          for (auto& e : entities) e->update();
     }

     void draw() {
          for (auto& e : entities) e->draw();
     }

     void refresh() {
          for (unsigned int i = 0; i < maxGroups; i++) {
               auto& v(groupedEntities[i]);
               v.erase(
                    std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity) {
                         return !mEntity->isActive() || !mEntity->hasGroup(i);
                    }),
                    std::end(v));
          }

          entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity) { 
                    return !mEntity->isActive();
               }), 
               std::end(entities));
     }

     void AddToGroup(Entity* mEntity, Group mGroup) {
          groupedEntities[mGroup].emplace_back(mEntity);
     }

     std::vector<Entity*>& getGroup(Group mGroup) {
          return groupedEntities[mGroup];
     }

     Entity& addEntity() {
          Entity* e = new Entity(*this);
          std::unique_ptr<Entity> uPtr{ e };
          entities.emplace_back(std::move(uPtr));
          return *e;
     }
};


//#endif 
