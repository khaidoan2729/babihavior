#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {

     if (
          recA.x + recA.w >= recB.x &&
          recB.x + recB.w >= recA.x &&
          recA.y + recA.h >= recB.y &&
          recB.y + recB.h >= recA.y 
     ) {
          return true;
     }
     return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB) {
     if (AABB(colA.collider, colB.collider)) {
          //std::cout << colA.tag << " hit " << colB.tag << std::endl;
          return true;
     }
     
     return false;
}

int Collision::AABBSides(const ColliderComponent& colA, const ColliderComponent& colB) {
     if (AABB(colA.collider, colB.collider)) {
          int xA = colA.collider.x;
          int yA = colA.collider.y;
          int wA = colA.collider.w;
          int hA = colA.collider.h;
          int xB = colB.collider.x;
          int yB = colB.collider.y;
          int wB = colB.collider.w;
          int hB = colB.collider.h;
          bool left = true;
          bool top = true;
          int w_ = 0;
          int h_ = 0;

          if (xA + wA - xB > xB + wB - xA) {
               w_ = xB + wB - xA;
               left = false;
          } else if (xA + wA - xB < xB + wB - xA) {
               w_ = xA + wA - xB;
          } else {
               if (yA <= yB) {
                    return 3;   // top
               } else {
                    return 4;   // bottom
               }
          }

          if (yA + hA - yB > yB + hB - yA) {
               h_ = yB + hB - yA;
               top = false;
          } else if (yA + hA - yB < yB + hB - yA) {
               h_ = yA + hA - yB;
          } else {
               if (left) return 1;    // left
               else return 2;         // right
          }

          if (w_ > h_) {
               if (top) return 3;
               else return 4;
          } else {
               if (left) return 1;
               else return 2;
          }

     }


     return 0;

}
