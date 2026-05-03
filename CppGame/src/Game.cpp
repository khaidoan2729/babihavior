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

/* Entities */
auto& sky(manager.addEntity());
auto& mountain(manager.addEntity());
auto& player(manager.addEntity());
auto& enemy1(manager.addEntity());
auto& enemy2(manager.addEntity());


bool Game::isRunning = true;

int Game::gameWidth = 0;
int Game::gameHeight = 0;
int Game::tileWidth = 0;
int Game::tileHeight = 0;

enum groupLabels:std::size_t {
     groupMap, 
     groupPlayers,
     groupEnemies,
     groupColliders,
     groupGround,
     groupCoins,
     groupBoundaries, 
};

vector<ColliderComponent*> Game::colliders;

/* Constructor */
Game::Game() {
}

/* Destructor */
Game::~Game() {
}

/* Init function */
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
     cnt = 0;
     int flags = 0;
     Game::gameWidth = width;
     Game::gameHeight = height;
     Game::tileWidth = width/64;
     Game::tileHeight = height/64;

     if (fullscreen) {
          flags = SDL_WINDOW_FULLSCREEN;
     }
     
     if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {   /* Successfully init SDL */
          cout << "Subsystems Initialized!" << endl;
    
          /* Init window */
          window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
          if (window) cout << "Window created!" << endl;

          /* Init renderer */
          renderer = SDL_CreateRenderer(window, -1, 0);
          if (renderer) {
               SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
               cout << "Renderer created!" << endl;
          }
    
          /* Game running */
          Game::isRunning = true;

     } else {
          /* Game not running */
          Game::isRunning = false;
     }

     
     Game::AddGround("assets/world/ground.png", 600);

     sky.addComponent<TransformComponent>(0.0f, 0.0f, 960, 768, 1);
     sky.addComponent<SpriteComponent>("assets/world/sky.png");

     mountain.addComponent<TransformComponent>(0.0f, -40.0f, 120, 96, 7);
     mountain.addComponent<SpriteComponent>("assets/world/mountain.png");     

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
     
     enemy1.addComponent<TransformComponent>(550.0f, 550.0f, 34, 45, 2);
     enemy1.addComponent<SpriteComponent>("assets/enemies/carrot/carrot.png", true, 2);
     enemy1.addComponent<ColliderComponent>("enemy-1");
     enemy1.addGroup(groupEnemies);
     enemy1.getComponent<TransformComponent>().velocity.y = 1;


     enemy2.addComponent<TransformComponent>(400.0f, 550.0f, 34, 45, 1);
     enemy2.addComponent<SpriteComponent>("assets/enemies/banana/banana.png", true, 1);
     enemy2.addComponent<ColliderComponent>("enemy-2");
     enemy2.addGroup(groupEnemies);
     enemy2.getComponent<TransformComponent>().velocity.y = 1;
     
     this->setBoundaries();

     this->addTiles();

}

void Game::setBoundaries() {
     auto& left(manager.addEntity());
     auto& right(manager.addEntity());
     left.addComponent<TransformComponent>(-1,0,1,640,1);
     left.addComponent<ColliderComponent>("left-boundary");
     right.addComponent<TransformComponent>(800,0,1,640,1);
     right.addComponent<ColliderComponent>("right-boundary");
     left.addGroup(groupBoundaries);
     right.addGroup(groupBoundaries);
}


void Game::isPlayer(Entity* e) {
     cout << "Setting player..." << endl;

     e->addComponent<TransformComponent>(10,200,40,45,1);
     e->addComponent<SpriteComponent>("assets/players/sky/idle_walk.png", true, 0);
     e->addComponent<KeyboardController>();
     e->addComponent<ColliderComponent>("player");
     e->addGroup(groupPlayers);
     e->getComponent<TransformComponent>().velocity.y = 1;
     e->isJumping = false;
     e->isOnGround = false;
}


void Game::handleEvents() {
     SDL_PollEvent(&event);
     if (event.type == SDL_QUIT) {
          Game::isRunning = false;
     }
}


/* Groups */
auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& boundaries(manager.getGroup(groupBoundaries));
auto& coins(manager.getGroup(groupCoins));
auto& collides(manager.getGroup(groupColliders));

/* Update game */
void Game::update() {
     cnt++;
     manager.refresh();
     manager.update();
     
     /* Handle collisions between player and collidable components */
     handleCollideColliders(&player, collides);

     /* Handle collisions between enemies and collidable components */
     for (auto& e : enemies){
          handleCollideColliders(e, collides);
     }

     /* Handle collisions between player and boundaries */
     handleCollideBoundaries(&player, boundaries);

     /* Handle coin collection */
     for (auto& c : coins) {
          GetCoin(&player, c);
     }

     /* Handle interactions with enemies */
     handleEnemies(&player, enemies); 
}

/* Handle coin collection */
void Game::GetCoin(Entity* player, Entity* coin) {
     if (Collision::AABB(
          player->getComponent<ColliderComponent>(), 
          coin->getComponent<ColliderComponent>())
     ) {
          coin->delGroup(groupCoins);
     }
}

/* Handle collision with boundaries */
void Game::handleCollideBoundaries(Entity* player, std::vector<Entity*>& bounds) {
     int idx = 0;

     for (auto& b : bounds) {
          int res = Collision::AABBSides(
               player->getComponent<ColliderComponent>(),
               b->getComponent<ColliderComponent>()
          );

          if (res != 0 && idx == 0) {
               /* Snap position to right */
               player->getComponent<TransformComponent>().position.x = b->getComponent<TransformComponent>().position.x
                    + static_cast<int>(b->getComponent<ColliderComponent>().collider.w);
          } else if (res != 0) {
               /* Snap position to left */
               player->getComponent<TransformComponent>().position.x = b->getComponent<TransformComponent>().position.x
                    - static_cast<int>(player->getComponent<ColliderComponent>().collider.w);
          }
          idx++;
     }
}

/* Handle collision with colliders */
void Game::handleCollideColliders(Entity* player, std::vector<Entity*>& collides) {

     bool noCollideFlag = true;

     for (auto& col: collides) {
          int res = Collision::AABBSides(
               player->getComponent<ColliderComponent>(),
               col->getComponent<ColliderComponent>()
          );

          if (res == 1) {     
               /* Snap left */
               player->getComponent<TransformComponent>().position.x = col->getComponent<TransformComponent>().position.x
                    - static_cast<int>(player->getComponent<ColliderComponent>().collider.w);
               noCollideFlag = false;
          } else if (res == 2) {
               /* Snap right */
               player->getComponent<TransformComponent>().position.x = col->getComponent<TransformComponent>().position.x
                    + static_cast<int>(col->getComponent<ColliderComponent>().collider.w);
               noCollideFlag = false;
          } else if (res == 3){
               /* Snap top */
               player->getComponent<TransformComponent>().position.y = col->getComponent<TransformComponent>().position.y
                    - static_cast<int>(player->getComponent<ColliderComponent>().collider.h);
               player->getComponent<TransformComponent>().velocity.y = 0;
               player->isOnGround = true;
               player->jumpCount = 0;
               player->isJumping = false;
               noCollideFlag = false;
               break;
          } else if (res == 4) {
               /* Snap bottom */
               player->getComponent<TransformComponent>().position.y = col->getComponent<TransformComponent>().position.y
                    + col->getComponent<ColliderComponent>().collider.h;
               player->isJumping = false;
               break;
          }  
     }
     
     if (player->isJumping) {
          player->getComponent<TransformComponent>().velocity.y += 0.05;
          player->isOnGround = false;
     }

     if (noCollideFlag && !player->isJumping) {
          if (player->getComponent<TransformComponent>().velocity.y < 1.2)
               player->getComponent<TransformComponent>().velocity.y += 0.2;
          player->isOnGround = false;
     }

}

/* Handle interactions with enemies */
void Game::handleEnemies(Entity* p, std::vector<Entity*>& enemies) {
     for (auto& e : enemies) {
          if (e->getComponent<SpriteComponent>().animations.size() >= 2) {
               int dist = 
                    (e->getComponent<TransformComponent>().position.x - p->getComponent<TransformComponent>().position.x)*
                    (e->getComponent<TransformComponent>().position.x - p->getComponent<TransformComponent>().position.x)+ 
                    (e->getComponent<TransformComponent>().position.y - p->getComponent<TransformComponent>().position.y)*
                    (e->getComponent<TransformComponent>().position.y - p->getComponent<TransformComponent>().position.y);
               if (10000 > dist && dist > -10000) {
                    e->getComponent<SpriteComponent>().Play("Attack");
               } else {
                    e->getComponent<SpriteComponent>().Play("Idle");
               }
          }
     }
}

/* Render game components */
void Game::render() {
     SDL_RenderClear(renderer);   
     sky.draw();
     mountain.draw();

     for (auto& t : tiles)        { t->draw();}
     for (auto& c : collides)     { c->draw();}
     for (auto& c : coins)        { c->draw();}
     for (auto& p : players)      { p->draw();}
     for (auto& e : enemies)      { e->draw();}
     for (auto& b : boundaries)   { b->draw();}
     SDL_RenderPresent(renderer);
}

/* Clean game */
void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  cout << "Cleaned Game" << endl;
}

/* Get the game state whether it is running */
bool Game::running() {
     return Game::isRunning;
}

/* Add collidable tiles */
void Game::addTiles() {
 
     auto& wall2(manager.addEntity());
     wall2.addComponent<TransformComponent>(285.0f, 485.0f, 64, 64, 1);
     wall2.addComponent<SpriteComponent>("assets/world/bg_dirt_64x64.png");
     wall2.addComponent<ColliderComponent>("big-dirt");
     wall2.addGroup(groupColliders);

     auto& wall3(manager.addEntity());
     wall3.addComponent<TransformComponent>(485.0f, 285.0f, 64, 64, 1);
     wall3.addComponent<SpriteComponent>("assets/world/bg_dirt_64x64.png");
     wall3.addComponent<ColliderComponent>("big-dirt");
     wall3.addGroup(groupColliders);
}

/* 
 * Add ground to main game. 
 * TODO: might need rework
 */
void Game::AddGround(const char* path, int y) {
     for (int i = 0; i <= Game::gameWidth; i+=64) {
          auto& ground(manager.addEntity());
          ground.addComponent<TransformComponent>((float)i,(float)y, 64, 64, 1);
          ground.addComponent<SpriteComponent>(path);
          ground.addComponent<ColliderComponent>("ground");
          ground.addGroup(groupColliders);
     }
}
