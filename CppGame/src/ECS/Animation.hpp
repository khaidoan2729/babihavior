#pragma once

class Animation {
public: 
     int index;
     int frames;
     int speed;


     Animation() {}
     Animation(int index_ , int frames_, int speed_) {
          index = index_;
          frames = frames_;
          speed = speed_;
     }
};
