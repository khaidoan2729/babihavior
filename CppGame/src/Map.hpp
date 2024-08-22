//#ifndef Map_hpp
//#define Map_hpp
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>
#include <string>

class Map {

public: 
     Map();   //Constructor
     ~Map();  //Destructor


     static void LoadMap(std::string path, int sizeX, int sizeY);


private: 
};

//#endif 
