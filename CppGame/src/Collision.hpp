//#ifndef Collision_hpp
//#define Collision_hpp

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>

class ColliderComponent;

class Collision {

public: 
     static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
     static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
     static int AABBSides(const ColliderComponent& colA, const ColliderComponent& colB);
 
private: 

};

//#endif 
