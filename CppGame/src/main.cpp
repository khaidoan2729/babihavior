#include <SDL2/SDL.h>
#include <iostream>
#include "Game.hpp"


using namespace std; 

Game *game = nullptr;

int main (int argc, char *argv[]) {

  const int FPS = 60;
  const int frame_delay = 1000 / FPS;

  Uint32 frame_start;
  int frame_time;
  game = new Game();
  game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 832, 640, false);

  /* Game loop */
  while (game->running()) {
    /* How many milisec since we initialized SDL */
    frame_start = SDL_GetTicks();     
    
    game->handleEvents();
    game->update();
    game->render();

    /* How long a frame has taken */
    frame_time = SDL_GetTicks() - frame_start;   

    if (frame_delay > frame_time) {
      SDL_Delay((frame_delay - frame_time));
    }
  }
  game->clean();

  return 0;
}
