#ifndef balloonh
#define balloonth
#include <vector>
#include <allegro.h>
using namespace std;

class balloon {
    float antigravity;
    double x;
    double y;
    double vx;
    double vy;
    
    int color;

    double angle;     // this is the current angle of the droplet
    bool gone;    // keeps track of whether the balloon is off the screen
    int dir;
    
    public:
        balloon(int x, int y, int d, int c);
        int getX(){ return (int)x; }
        int getY(){ return (int)y; }
        void update();
        void drawBalloon(BITMAP* b, BITMAP* f[]);
        bool isGone();
};
#endif
