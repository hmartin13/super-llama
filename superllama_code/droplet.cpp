#ifndef dropletcpp
#define dropletcpp
#include <math.h>
#include "droplet.h"

droplet::droplet(int ix, int iy, float a, int d, int c){
    color = c;
    x = ix; 
    y = iy; 
    vx = 0;
    vy = -3;
    angle = a;
    dir = d;
    current_sprite = 0;
    gravity = .4;
    grounded = false;
}

void droplet::update(vector<int> map){
    if((int)y>= map[(int)x+20]){
        grounded = true;
    }
    if(dir == 0){ vx += 0.05; }
    else{ vx -= 0.05; }
    vy += gravity;
    x += vx;
    y += vy;
    angle += 10;
}

void droplet::drawDroplet(BITMAP* b, BITMAP* f[][4]){
    double speed = sqrt(vx*vx+vy*vy);
    if(speed>6) current_sprite = 0;
    else if(speed>3) current_sprite = 1;
    else if(speed>1) current_sprite = 2;
    else current_sprite = 3;
    pivot_sprite(b, f[color][current_sprite], (int)x, (int)y, 20, 20, ftofix(angle));
}

bool droplet::isGrounded(){
     return grounded;
}

#endif
