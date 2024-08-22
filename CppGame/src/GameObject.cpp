#include "GameObject.hpp"
#include "TextureManager.hpp" 


GameObject::GameObject(const char* texturesheet_, int x_, int y_) {
     objTexture = TextureManager::LoadTexture(texturesheet_);
     xpos = x_;
     ypos = y_;
}

void GameObject::Update() {
     xpos++;
     ypos++;

     srcRect.h = 64;
     srcRect.w = 64;
     srcRect.x = 0;
     srcRect.y = 0;

     destRect.w = srcRect.w * 2;
     destRect.h = srcRect.h * 2;
     destRect.x = xpos;
     destRect.y = ypos;

}

void GameObject::Render() {
     SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

