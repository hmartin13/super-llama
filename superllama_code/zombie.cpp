#ifndef zombiecpp
#define zombiecpp
#include <math.h>
#include "zombie.h"

zombie::zombie(int t){
      color = rand()%13;
      
      //PHYSICS VARIABLES
      airfriction = 1.02;
      friction = 1.05;
      gravity = .4;

      //ZOMBIE MOTION VARIABLES
      x =  rand()%700 +50;    //spawns with a random x,y value at the moment - MIGHT NEED TO CHAINGE
      y =  rand()%200 +50;
      dest_x = x;              //we don't want it wandering unless wander yet
      dest_y = y;              //so we set it's wander destination to it's current location
      vx = 0;                  //no velocity at start
      vy = 0;
      
      if(rand()%3==0) vx = 0.3;
      if(rand()%3==0) vx = -0.3;
      if(rand()%3==0) vy = -0.3;
      if(rand()%3==0) vy = 0.3;

      //ZOMBIE STATE VARIABLES
      hit = false;
      dead = false;            //spawing dead is stoopid
      attack = 0;          //attacking on spawn wouldn't be very nice either
      type = t;

      //ZOMBIE ANIMATION VARIABLES
      sprite = 0;
      leftFacing = rand()%2;   //the direction it faces is also random
}

void zombie::update(int px, int py, float pvx, float pvy, vector<int> map){
     //UPDATING MOVEMENT OF ZOMBIE BASED ON TYPE (AIR / GROUND)
     float velocity = sqrt(pvx*pvx + pvy*pvy);
     if(type == 0){
          if(dest_x<x) leftFacing = 1;
          else if(dest_x>x) leftFacing = 0;
          if(dest_x<60) dest_x = 60;
          if(dest_x>740) dest_x = 740;
          if(rand()%50==0){
              dest_x = rand()%200 - 100 + x;
          }
          if(dest_x>x) x++;
          if(dest_x<x) x--;
          y = map[(int)x+32]-64;
     }else{
          if(hit){
              vy += gravity;
              y += vy;
          }
          
          if(vx>0) leftFacing = 0;
          if(vx<0) leftFacing = 1;
          
          if(y<0) vy = -vy;
          if(x<0||x>752) vx = -vx;
          
          if(rand()%10==0) vx+=.01;
          if(rand()%10==0) vx-=.01;
          if(rand()%10==0) vy+=.01;
          if(rand()%10==0) vy-=.01;
          
          x += vx;
          y += vy;
          
          if((int)y>= map[(int)x+32]-115){
               y = map[(int)x+32]-64;
               type = 0;
               hit = false;
          }
     }

    //COLLISION DETECTION
    
    if(!hit){
        if (abs ((int)x - px) + abs ((int)y - py) < 60){
            if(y-32 > py){
                if(type == 0) dead = true;
                else hit = true;
                attack = 3;
            } else if((y < py) && (type == 1)){
                hit = true;
                attack = 4;
            } else if(velocity > 6){
                if(type == 0) dead = true;
                else{ hit = true; attack = 5; }
            } else {
                if(x > px) attack = 1;
                else attack = 2;
            }
        } else attack = 0;
    } else attack = 0;
}

void zombie::kill(){
     dead = false;
}

bool zombie::isDead(){
     return dead;
}

int zombie::isAttacking(){
     return attack;
}

bool zombie::isHit(){
     return hit;
}

int zombie::getType(){
    return type;
}

int zombie::getDir(){
    return leftFacing;
}

void zombie::drawZombie(BITMAP* b, BITMAP* s[][2]){
    
    if(hit){
        if(leftFacing == 0){ draw_sprite_h_flip(b, s[color][0], (int)x, (int)y); }
        else{ draw_sprite(b, s[color][0], (int)x, (int)y); }
    } else {
        if(leftFacing == 0){ draw_sprite_h_flip(b, s[color][type], (int)x, (int)y); }
        else{ draw_sprite(b, s[color][type], (int)x,(int) y); }
    }
}
#endif
