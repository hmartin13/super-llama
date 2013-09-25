#ifndef dropleth
#define dropleth
#include <vector>
#include <allegro.h>
using namespace std;

class droplet {
    float gravity;
    double x;
    double y;
    double vx;
    double vy;
    
    int color;

    double angle;     // this is the current angle of the droplet
    bool grounded;    // keeps track of whether the droplet has hit the ground
    int dir;
    int current_sprite;
    
    public:
        droplet(int x, int y, float a, int d, int c);
        int getX(){ return (int)x; }
        int getY(){ return (int)y; }
        void update(vector<int> map);
        void drawDroplet(BITMAP* b, BITMAP* f[][4]);
        bool isGrounded();
};
#endif
