#ifndef alienrocketh
#define alienrocketh
#include <allegro.h>
using namespace std;

class alienrocket {
    int color;
    int x;        //x coordinate of the missile
    int y;        //y coordinate of the missile
    fixed angle;      //random direction for the missile to travel in
    fixed angle_stepsize;
    int targetX;  //x coordinate of the target - used to see if the missile missed
    int targetY;  //y coordinate of the target - used to see if the missile missed
    bool fired;   //keeps track of whether the missile has already been fire
    bool hit;  //keeps track of whether the missile hit it's target or not
    int speed;
    
    public:
        alienrocket();        //default constructor for alienrocket object
        void fireRocket(int dx, int dy, int a);   //dx,dy are the destination coordinates
        void recieveNewTargetCoor(int tx, int ty); //tx, ty are the target's coordinates
        void update();                //move the missile 
        void recievingNewCoordinates(int x, int y);    //x,y are rocket's new location
        bool hitTarget();             //lets us know if the missile missed(false) or not(true)
        bool hasBeenFired();          //lets us know if the missile has been fired
        void drawTo(BITMAP* b, BITMAP* f[]); //alien missile graphic handling
        void kill();
        void setFired(bool b);
        int getX(){ return fixtoi(x); }
        int getY(){ return fixtoi(y); }
};
#endif
