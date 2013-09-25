#ifndef boomh
#define boomh
#include <vector>
#include <allegro.h>
using namespace std;

class boom {
    int x;
    int y;
    int color;
    int current_sprite;
    bool dispersed;
    
    public:
        boom(int x, int y);
        int getX(){ return x; }
        int getY(){ return y; }
        void drawExplosion(BITMAP* b, BITMAP* f[][4]);
        bool isDispersed();
};
#endif
