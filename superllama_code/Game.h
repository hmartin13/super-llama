#ifndef gameh
#define gameh
#include <vector>
#include "zombie.h"
#include "boom.h"
#include "alienrocket.h"
#include "droplet.h"
#include "balloon.h"
#include <allegro.h>
using namespace std;

class Game {
    SAMPLE* pop;

    int animationCounter;
    // used to time sprite changes
    
    int pageNumber;

    // GLOBAL PHYSICAL PROPERTIES:
    float gravity;
    float friction;
    float airfriction;
    vector<int> heightMap;

    vector<zombie*> horde;        //ground zombies
    vector<zombie*> flyers;       //air zombies
    vector<droplet*> splatter;
    vector<balloon*> balloons;
    vector<alienrocket*> missile;
    vector<boom*> explosion;
    
    int bossfight_counter;
    float boss_vrotation;
    float boss_rotation;
    int boss_health;
    bool bossfight;
    float boss_vx;
    float boss_vy;
    float boss_x;
    float boss_y;
    int boss_death;

    // SUPERLLAMA's PHYSICAL PROPERTIES:
    int damage;
    int comboPointCounter;
    bool leftFacing;
    bool flying;
    bool airborne;
    int currentSprite;
    float superllama_x;
    float superllama_y;
    float superllama_vx;
    float superllama_vy;
    int superllama_sprite;
    int superllama_action;
    BITMAP* sprite_superllama[3][8];
    // 8 sprites per animation, 3 animations
    // [0] idle; [1] running; [2] jumping/flying
    
    void generateNewTerrain(int smoothness, float roughness);
    void generateTerrain(int smoothness, float roughness);

    public:
        Game();
        bool update(int s, int MS);
        void keyboardListener();
        void drawTerrain(BITMAP* buffer);
        void drawSuperllama(BITMAP* buffer);
        void loadNewZombies();
        void rainbowZombie();
        int comboMoveAvailable();
        void drawExplosions(BITMAP* b, BITMAP* f[][4]);
        void drawDroplets(BITMAP* b, BITMAP* f[][4]);
        void drawBalloons(BITMAP* b, BITMAP* f[]);
        void drawMissile(BITMAP* b, BITMAP* f[]);
        void drawRainbowZombie(BITMAP* buffer, BITMAP* zombieHead[]);
        void drawZombies(BITMAP* buffer, BITMAP* spriteSheet[][2]);
        void loadSuperllamaGraphics();
        bool stuffIsGoingDown(){ return bossfight; }
        int getPage(){ return pageNumber; }
        int llamaDamage(){ return damage; }
        bool victory(){ return boss_death>400; }
};
#endif
