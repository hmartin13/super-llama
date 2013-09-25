#ifndef alienrocketcpp
#define alienrocketcpp
#include <math.h>
#include "alienrocket.h"

alienrocket::alienrocket(){
    color = 0;
    x = 0;
    y = 0; 
    fired = false;
    hit = false;
    speed = 5;
    angle_stepsize = ftofix(3);
    angle = 0;
}

bool alienrocket::hasBeenFired(){
     return fired;
}

bool alienrocket::hitTarget(){
     return hit;
}

void alienrocket::kill(){
     fired = false;
     hit = false;
}

void alienrocket::drawTo(BITMAP* b, BITMAP* f[]){
     if(color > 14){ color = 0; }
     if(fired){ rotate_sprite(b, f[color], fixtoi(x), fixtoi(y), angle); }
     color++;
}

void alienrocket::recievingNewCoordinates(int xi, int yi){
     if(fired == false){
          x = itofix(xi);
          y = itofix(yi);    
     }
}

void alienrocket::recieveNewTargetCoor(int tx, int ty){
     targetX = itofix(tx);
     targetY = itofix(ty);
}

void alienrocket::fireRocket(int dx, int dy, int a){
     angle = rand()% 360;
     targetX = itofix(dx);
     targetY = itofix(dy);
     fired = true;
     int r1;   //randomly picks an x coorinate in the general direction of the target
     int r2;   //randomly picks a y coordinate in the general direction of the target
     r1 = (dx - 100) + int(200.0 * rand()/(RAND_MAX+1.0));
     r2 = (dy - 100)+ int(200.0 * rand()/(RAND_MAX+1.0));
     angle = a;
     angle = fatan2(itofix(r2) - y, itofix(r1)- x);
}

void alienrocket::setFired(bool b){
     fired = b;
}

void alienrocket::update(){
     if(fired){
        // rocket moves towards destination:
        if((fixtoi(x) > 0) && (fixtoi(x) < 800) && (fixtoi(y) < 600) && (fixtoi(y) > 0)){
              x += speed * fcos (angle);
              y += speed * fsin (angle); 
        }else{ fired = false; }
        //angle = (angle + angle_stepsize) & 0xFFFFFF; 
        //if we hit the player
        if (abs (x - targetX) + abs (y - targetY) < itofix(50)){ hit = true; fired = false; } 
    } 
}

#endif
