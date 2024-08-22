#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
#include "ECS/Components.hpp"
#include "Collision.hpp"
#include "Vector2D.hpp"
#include "ECS/ECS.cpp"


using namespace std;


Map* map;

SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::event;

Manager manager;

auto& sky(manager.addEntity());
auto& mountain(manager.addEntity());
auto& player(manager.addEntity());
auto& wall(manager.addEntity());


bool Game::isRunning = true;

int Game::gameWidth = 0;
int Game::gameHeight = 0;


enum groupLabels : std::size_t {
     groupMap, 
     groupPlayers,
     groupEnemies,
     groupColliders,
     groupGround,
     groupCoins,
};

vector<ColliderComponent*> Game::colliders;

Game::Game() {
}

Game::~Game() {
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
     cnt = 0;
     int flags = 0;
     Game::gameWidth = width;
     Game::gameHeight = height;
     if (fullscreen) {
          flags = SDL_WINDOW_FULLSCREEN;
     }
     if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {   // Successfully init SDL
          cout << "Subsystems Initialized!" << endl;
    
          // Init window
          window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    
          if (window) {
               cout << "Window created!" << endl;
          }

          //Init renderer
          renderer = SDL_CreateRenderer(window, -1, 0);
          if (renderer) {
               SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
               cout << "Renderer created!" << endl;
          }
    
          //Game running
          Game::isRunning = true;

     } else {
          //Game not running
          Game::isRunning = false;
     }

     //map = new Map();
     
     Game::AddGround("assets/world/ground.png", 600);

     sky.addComponent<TransformComponent>(0.0f, 0.0f, 960, 768, 1);
     sky.addComponent<SpriteComponent>("assets/world/sky.png");
     //sky.addGroup(groupBackground);

     mountain.addComponent<TransformComponent>(0.0f, -40.0f, 120, 96, 7);
     mountain.addComponent<SpriteComponent>("assets/world/mountain.png");
     //mountain.addGroup(groupBackground);
     
     wall.addComponent<TransformComponent>(485.0f, 485.0f, 64, 64, 1);
     wall.addComponent<SpriteComponent>("assets/world/bg_dirt_64x64.png");
     wall.addComponent<ColliderComponent>("big-dirt");
     wall.addGroup(groupColliders);

     auto& coin1(manager.addEntity());

     coin1.addComponent<TransformComponent>(450.0f, 475.0f, 20, 24, 1);
     coin1.addComponent<SpriteComponent>("assets/receivables/coin_anim.png", true, 1);
     coin1.addComponent<ColliderComponent>("coin");
     coin1.addGroup(groupCoins);
     
     auto& coin2(manager.addEntity());

     coin2.addComponent<TransformComponent>(500.0f, 475.0f, 20, 24, 1);
     coin2.addComponent<SpriteComponent>("assets/receivables/coin_anim.png", true, 1);
     coin2.addComponent<ColliderComponent>("coin");
     coin2.addGroup(groupCoins);

     this->isPlayer(&player);
}


void Game::isPlayer(Entity* e) {
     cout << "Setting player..." << endl;

     e->addComponent<TransformComponent>(0,400,40,45,1);
     e->addComponent<SpriteComponent>("assets/players/sky/idle_walk.png", true, 0);
     e->addComponent<KeyboardController>();
     e->addComponent<ColliderComponent>("player");
     e->addGroup(groupPlayers);
     e->getComponent<TransformComponent>().velocity.y = 1;
}


void Game::handleEvents() {
  
     SDL_PollEvent(&event);
  
     if (event.type == SDL_QUIT) {
          Game::isRunning = false;
     }
}

void Game::handleJump(Entity* player) {
     if (!(player->isOnGround)) {
          player->getComponent<TransformComponent>().velocity.y += 0.05;
     }
}


auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
//auto& grounds(manager.getGroup(groupGround));
auto& coins(manager.getGroup(groupCoins));
auto& collides(manager.getGroup(groupColliders));

void Game::update() {
     cnt++;
     manager.refresh();
     manager.update();

     //handleJump(&player);

     handleCollide(&player, collides);
     
     //Land(&player, grounds);


     for (auto& c : coins) {
          GetCoin(&player, c);
     }

}

void Game::GetCoin(Entity* player, Entity* coin) {
     if (Collision::AABB(player->getComponent<ColliderComponent>(), coin->getComponent<ColliderComponent>()) ) {  
          coin->delGroup(groupCoins);
     }
}

void Game::Land(Entity* player, std::vector<Entity*>& grounds) {
     for (auto& ground: grounds) {
          if (Collision::AABB(player->getComponent<ColliderComponent>(), ground->getComponent<ColliderComponent>()) ) {  
               //player->getComponent<TransformComponent>().velocity.x = 0;
               player->getComponent<TransformComponent>().velocity.y = 0;
               player->getComponent<TransformComponent>().position.y = ground->getComponent<TransformComponent>().position.y  
                                                            - static_cast<int>(player->getComponent<ColliderComponent>().collider.h) - 1;
               player->isOnGround = true;

               player->jumpCount = 0;

               if (player->isJumping) { 
                    player->isJumping = false;
               }
          }
     }
     if (!player->isOnGround)
          player->getComponent<TransformComponent>().velocity.y += 0.05;
}


void Game::handleCollide(Entity* player, std::vector<Entity*>& collides) {

     bool noCollideFlag = true;

     for (auto& col: collides) {
          int res = Collision::AABBSides(player->getComponent<ColliderComponent>(), col->getComponent<ColliderComponent>());

               
          if (res == 1) {
               player->getComponent<TransformComponent>().position.x = col->getComponent<TransformComponent>().position.x
                    - static_cast<int>(player->getComponent<ColliderComponent>().collider.w);
               noCollideFlag = false;
               cout << "Left" << endl;
               //player->isJumping = false;
               
          } else if (res == 2) {
               player->getComponent<TransformComponent>().position.x = col->getComponent<TransformComponent>().position.x
                    + static_cast<int>(col->getComponent<ColliderComponent>().collider.w);
               noCollideFlag = false;
               cout << "Right" << endl;
               //player->isJumping = false;
               
          } else if (res == 3){
               player->getComponent<TransformComponent>().position.y = col->getComponent<TransformComponent>().position.y
                    - static_cast<int>(player->getComponent<ColliderComponent>().collider.h);
               player->getComponent<TransformComponent>().velocity.y = 0;
               player->isOnGround = true;
               player->jumpCount = 0;
               player->isJumping = false;
               cout << "Top" << endl;
               noCollideFlag = false;
               break;
          } else if (res == 4) {
               player->getComponent<TransformComponent>().position.y = col->getComponent<TransformComponent>().position.y
                    + col->getComponent<ColliderComponent>().collider.h;
               //player->getComponent<TransformComponent>().velocity.y = 0.05;
               //noCollideFlag = false;
               player->isJumping = false;

               cout << "Bottom" << endl;
               cout << "block: x=" << col->getComponent<TransformComponent>().position.x << ", y=" << col->getComponent<TransformComponent>().position.y << " / w="
                    << col->getComponent<ColliderComponent>().collider.w << ", h=" << col->getComponent<ColliderComponent>().collider.h
                    << "player: x=" << player->getComponent<TransformComponent>().position.x << ", y=" << player->getComponent<TransformComponent>().position.y << " / w="
                    << player->getComponent<ColliderComponent>().collider.w << ", h=" << player->getComponent<ColliderComponent>().collider.h << endl;

               break;
          }  
     }
     
     //if (cnt % 4 == 0) {
     //     cout << "Velocity: "  << player.getComponent<TransformComponent>().velocity.x << ","
     //                           << player.getComponent<TransformComponent>().velocity.y << "  onground: "
     //                           << player.isOnGround << endl;
     //}


     if (player->isJumping) {
          player->getComponent<TransformComponent>().velocity.y += 0.05;
          cout << "Jumping "  << cnt << endl;
     }

     if (noCollideFlag && !player->isJumping) {
          player->getComponent<TransformComponent>().velocity.y += 0.3;
          cout << "No Collide " << cnt << endl;
     }

}

void Game::render() {
     SDL_RenderClear(renderer);   
     //for (auto& b : backgrounds)  { b->draw();}
     sky.draw();
     mountain.draw();
     for (auto& t : tiles)        { t->draw();}
     //for (auto& g : grounds)      { g->draw();}
     for (auto& c : collides)     { c->draw();}
     for (auto& c : coins)        { c->draw();}
     for (auto& p : players)      { p->draw();}
     for (auto& e : enemies)      { e->draw();}
     SDL_RenderPresent(renderer);

}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  cout << "Cleaned Game" << endl;
}

bool Game::running() {
  return Game::isRunning;
}

void Game::AddTile(int id, int x, int y) {
     auto& tile(manager.addEntity());
     tile.addComponent<TileComponent>(x,y,64,64,id);
     tile.addGroup(groupMap);
}

void Game::AddGround(const char* path, int y) {
     
      for (int i = 0; i <= Game::gameWidth; i+=64) {
           auto& ground(manager.addEntity());
           ground.addComponent<TransformComponent>((float)i,(float)y, 64, 64, 1);
           ground.addComponent<SpriteComponent>(path);
           ground.addComponent<ColliderComponent>("ground");
           ground.addGroup(groupColliders);
      }
}

