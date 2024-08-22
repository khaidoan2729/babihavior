#include <SDL2/SDL.h>
#include <iostream>
#include "Game.hpp"


using namespace std; 

Game *game = nullptr;


int main (int argc, char *argv[]) {
     int a = -1;
     a *= -1;
     cout << "a = " << a << endl;  

  const int FPS = 60;
  const int frame_delay = 1000 / FPS;

  Uint32 frame_start;
  int frame_time;
  
  game = new Game();
  

  game->init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

  //Game loop
  while (game->running()) {
    frame_start = SDL_GetTicks();    // How many milisec since we initialized SDL  
    
    game->handleEvents();
    game->update();
    game->render();

    frame_time = SDL_GetTicks() - frame_start;   // how long a frame has taken

    if (frame_delay > frame_time) {
      SDL_Delay((frame_delay - frame_time));
    }
  }
  game->clean();

  return 0;
}




/*
int main (int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    cout << "fail";
  } else {
    cout << "success";
  }


  SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    cout << "Cant create window";
    SDL_Quit();
    return -1;
  }

  // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

  // SDL_SetRenderDrawColor(renderer, 0,255,0,255);

  // SDL_RenderClear(renderer);

  // SDL_RenderPresent(renderer);

  // SDL_Delay(8000);
  //

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 0, 255, 1));
  SDL_UpdateWindowSurface(window);

  SDL_bool quit = SDL_FALSE;
  while(!quit) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = SDL_TRUE;
      }
    }
  }
  
  SDL_DestroyWindow(window);
  SDL_Quit();


}
*/


