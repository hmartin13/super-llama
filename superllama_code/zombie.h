#ifndef zombieh
#define zombieh
#include <vector>
#include <allegro.h>
using namespace std;

class zombie {
      // GLOBAL PHYSICAL PROPERTIES:
      float gravity;
      float friction;
      float airfriction;
      
      int color;

      float x;                  //current x position of the zombie
      float y;                  //current y position of the zombie
      float vx;
      float vy;
      float dest_x;             //the x coordinate of where the zombie is moving to
      float dest_y;             //the y coordinate of where the zombie is moving to

      int sprite;               //the zombie's current sprite animation
      int attack;              //if player's velocity is not great enough the zombie will attack
      bool dead;                //if player's velocity IS great enough the zombie will die
      bool hit;                 //detects if an air zombie has been hit
      int leftFacing;
      int type;                 //0 is ground type ... 1 is air type

      public:
             zombie(int t);  //t is the type
             //px,py is the player's current position and pvx,pvy is the player's current velocity
             void update(int px, int px, float pvx, float pvy, vector<int> map);
             int getX(){ return (int)x; }
             int getY(){ return (int)y; }
             int isAttacking();
             int getDir();
             bool isDead();
             bool isHit();
             int getType();
             void kill();       //muder zombie...with superllama-ness...
             void drawZombie(BITMAP* b, BITMAP* s[][2]);
             int getColor(){ return color; }
};
#endif

