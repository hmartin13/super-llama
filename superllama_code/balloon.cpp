#ifndef ballooncpp
#define ballooncpp
#include <math.h>
#include "balloon.h"

balloon::balloon(int ix, int iy, int d, int c){
    if(d == 1) x = ix + 64;
    else x = ix; 
    color = c;
    y = iy; 
    vx = 0;
    vy = -3;
    dir = d;
    antigravity = -.1;
    gone = false;
}

void balloon::update(){
    if((x <0) || (x > 800) || (y < 0)){ gone = true; }
    if(dir == 1){ vx += 0.l; }
    else{ vx -= 0.1; }
    vy += antigravity;
    x += vx;
    y += vy;
}

void balloon::drawBalloon(BITMAP* b, BITMAP* f[]){
    pivot_sprite(b, f[color], (int)x, (int)y, 20, 20, ftofix(0));
}

bool balloon::isGone(){
     return gone;
}

#endif
