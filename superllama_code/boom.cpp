#ifndef boomcpp
#define boomcpp
#include <math.h>
#include "boom.h"

boom::boom(int ix, int iy){
    color = rand()%4;
    x = ix; 
    y = iy; 
    current_sprite = 0;
    dispersed = false;
}

void boom::drawExplosion(BITMAP* b, BITMAP* f[][4]){
    if(current_sprite > 14){ dispersed = true; }
    if(!dispersed)draw_sprite(b, f[current_sprite][color], x, y);
    current_sprite++;
}

bool boom::isDispersed(){
     return dispersed;
}

#endif
