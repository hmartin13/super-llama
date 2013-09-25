#ifndef gamecpp
#define gamecpp
#include "Game.h"
#include <allegro.h>
using namespace std;

Game::Game(){
    for(int i=0; i<5; i++){
        missile.push_back(new alienrocket());
    }             
    pop = load_sample("pop.wav");
    generateTerrain(1, 1.0);
    loadSuperllamaGraphics();
    superllama_action = 0;
    superllama_sprite = 0;
    superllama_x = 300;
    superllama_y = 300;
    superllama_vx = 0;
    superllama_vy = 0;
    
    damage = 0;
    pageNumber = 0;

    bossfight_counter = 0;
    boss_vrotation = 0.0;
    boss_rotation = 0.0;
    bossfight = false;
    boss_health = 100;
    boss_x = 600;
    boss_y = 700;
    boss_vx = 0;
    boss_vy = 0;
    boss_death = 0;
    
    airfriction = 1.02;
    friction = 1.05;
    gravity = .4;
    airborne = true;
    leftFacing = false;
    comboPointCounter = 0;
}

bool Game::update(int score, int MAX_SCORE){
    damage = 0;
    
    if(comboPointCounter>0) comboPointCounter--;
    if(comboPointCounter>760) comboPointCounter=760;
    
    animationCounter++;
    // CYCLE THROUGH ANIMATION SPRITES
    if(animationCounter>4){
        animationCounter = 0;
        superllama_sprite ++;
        if(superllama_sprite>7)
            superllama_sprite = 0;
    }

    // CHECK FOR PAGE TURNS
    if(bossfight_counter==0 && horde.size()==0 && flyers.size()==0){
        if(superllama_x<-superllama_vx){
            superllama_vy = 0;
            superllama_x = 730;
            generateNewTerrain((int)((float)score/2/MAX_SCORE*8)+1, 1.0);
            if(score>0 && score<MAX_SCORE) loadNewZombies();
            else if(score==MAX_SCORE) bossfight_counter = 1;
        }else if(superllama_x>=736-superllama_vx){
            superllama_vy = 0;
            superllama_x = 6;
            generateNewTerrain((int)((float)score/2/MAX_SCORE*8)+1, 1.0);
            if(score>0 && score<MAX_SCORE) loadNewZombies();
            else if(score==MAX_SCORE) bossfight_counter = 1;
        }
    } else if(superllama_x<-superllama_vx || superllama_x>=736-superllama_vx){
        superllama_vx /= -1.5;
    }
    
    // APPLY PHYSICS to SUPERLLAMA
    if(airborne) superllama_vx /= airfriction;
    else superllama_vx /= friction;
    if(!flying) superllama_vy += gravity;
    superllama_x += superllama_vx;
    superllama_y += superllama_vy;
    
    // CAUSE SUPERLLAMA to COLLIDE with the GROUND
    if(superllama_y+64 > heightMap[(int)superllama_x+32]){
        superllama_y = heightMap[(int)superllama_x+32]-64;
        superllama_vy /= -1.8;
        airborne = false;
    }

    // UPDATE DROPLETS
    for(int i=0; i<splatter.size(); i++){
        splatter[i]->update(heightMap);
        if(splatter[i]->isGrounded())
            splatter.erase(splatter.begin()+i);
    }

    //UPDATE BALLOONS
     for(int i=0; i<balloons.size(); i++){
        balloons[i]->update();
        if(balloons[i]->isGone())
            balloons.erase(balloons.begin()+i);
    }
    
    if(bossfight_counter>0){
        // UPDATE BOSS ZOMBIE!
        rainbowZombie();
        
        if(bossfight){
            if(superllama_x > boss_x-220){
                superllama_vx = -superllama_vx;
                superllama_x = boss_x-220;
                int ow = abs((int)(superllama_vx));
                if(ow<0) ow = 0;
                boss_health -= ow;
                if(ow>0){
                    splatter.push_back(new droplet((int)superllama_x+20, (int)superllama_y, rand()%360, 0, rand()%13));
                }
                
                if(boss_health<0 && boss_death == 0){
                    boss_death = 1;
                }
            }
        }
        
        else {
            flying = false;
            if(superllama_x > 300){ superllama_vx -= 0.4; superllama_action = 1; }
            else if(superllama_x < 100){ superllama_vx += 0.4; superllama_action = 1; }
            else { superllama_action = 0; leftFacing = false; }
        }
        
        if(bossfight_counter==100 && !bossfight) return true;
    } else {

        //UPDATE ZOMBIES
        for(int i = 0; i < horde.size(); i++){
            horde[i]->update((int)superllama_x, (int)superllama_y, superllama_vx, superllama_vy, heightMap);
            if(horde[i]->isAttacking() == 3){ superllama_vy = -5; }
            else if(horde[i]->isAttacking() == 4){ superllama_vy = 5;}
            else if(horde[i]->isAttacking() == 1){ superllama_vx = -3; damage = 5; }
            else if(horde[i]->isAttacking() == 2){ superllama_vx = 3; damage = 5; }
            if(horde[i]->isDead()){
                 play_sample(pop, 1000, 128, rand()%1000+500, FALSE);
                 comboPointCounter+=80;
                 superllama_vx /= 2;
                 superllama_vy /= 2;
                 splatter.push_back(new droplet(horde[i]->getX(), horde[i]->getY(), 30, 0, horde[i]->getColor()));
                 splatter.push_back(new droplet(horde[i]->getX(), horde[i]->getY(), 60, 1, horde[i]->getColor()));
                 horde.erase(horde.begin()+i);
            }
        }
        for(int i = 0; i < flyers.size(); i++){
            flyers[i]->update((int)superllama_x, (int)superllama_y, superllama_vx, superllama_vy, heightMap);
            if(flyers[i]->isAttacking() == 3){ superllama_vy = -5; }
            else if(flyers[i]->isAttacking() == 4){ superllama_vy = 5; }
            else if(flyers[i]->isAttacking() == 1){ superllama_vx = -3; damage = 5; }
            else if(flyers[i]->isAttacking() == 2){ superllama_vx = 3; damage = 5; }
            if(((flyers[i]->isAttacking() == 4) || (flyers[i]->isAttacking() == 5)
                 ||(flyers[i]->isAttacking() == 3))&& flyers[i]->getType() == 1){
                comboPointCounter+=80;
                play_sample(pop, 1000, 128, rand()%1000+500, FALSE);
                balloons.push_back(new balloon(flyers[i]->getX(), flyers[i]->getY(), flyers[i]->getDir(), flyers[i]->getColor()));
            }
            //if(flyers
            if(flyers[i]->isDead()){
                play_sample(pop, 1000, 128, rand()%1000+500, FALSE);
                comboPointCounter+=80;
                superllama_vx /= 2;
                superllama_vy /= 2;
                splatter.push_back(new droplet(flyers[i]->getX(), flyers[i]->getY(), 30, 0, flyers[i]->getColor()));
                splatter.push_back(new droplet(flyers[i]->getX(), flyers[i]->getY(), 60, 1, flyers[i]->getColor()));
                flyers.erase(flyers.begin()+i);
            }
        }
    }
    
    if(explosion.size()>0){
        for(int i = 0; i < explosion.size(); i++){
                if(explosion[i]->isDispersed()){ explosion.erase(explosion.begin()+i); }        
        }
    }
    if(boss_death==1) return true;
    return false;
}

void Game::keyboardListener(){
    if(bossfight_counter<=0 || bossfight){
        superllama_action = 0;

        // WASD or ARROW KEYS to move

        if(key[KEY_LEFT]||key[KEY_A]){
            superllama_action = 1;
            superllama_vx -= 0.4;
            leftFacing = true;
        }
        if(key[KEY_RIGHT]||key[KEY_D]){
            superllama_action = 1;
            superllama_vx += 0.4;
            leftFacing = false;
        }
        if(airborne) superllama_action = 2;

        flying = false;

        if(key[KEY_UP]||key[KEY_W]){
            if(airborne){
                flying = true;
                if(superllama_y < 100) superllama_vy /= 1.1;

            }else if(superllama_y+74 > heightMap[(int)superllama_x+32]){
                // You can only jump if you're touching (ish) the ground
                superllama_vy = -10;
                airborne = true;
            }
        }
    }
}

void Game::rainbowZombie(){
    if(bossfight_counter<500) bossfight_counter++;
    else bossfight = true;
    
    if(boss_death>0){
        for(int i = 0; i < missile.size(); i++){
                missile[i]->setFired(false);
        }
        boss_death++;
        if(boss_vy < 1) boss_vy += .1;
        if(boss_rotation < 90) boss_rotation += .1;
        splatter.push_back(new droplet(rand()%((int)boss_x-200)+300,
            rand()%((int)boss_y-200)+200, 1.0, rand()%360, rand()%13));
        explosion.push_back(new boom(rand()%((int)boss_x-200)+300,
            rand()%((int)boss_y-200)+200));
        
    } else if(bossfight){
        // MAKE RAINBOW ZOMBIE BREATHE
        boss_vrotation /= 1.1;
        //boss_vy /= 1.1;
        
        if(boss_y > 300){
            boss_vy -= .01;
            boss_vrotation += .002;
        } else {
            boss_vy += .01;
            boss_vrotation -= .002;
        }
        
        // MISSILES!
        
        for(int i = 0; i < missile.size(); i++){
            int randomFire = rand()%40;
            if(randomFire == 0){
            //doing the boss missile logic here
                if(!missile[i]->hasBeenFired()){ //missile has not been fired
                    missile[i]->recievingNewCoordinates(rand()%((int)boss_x-200)+300,
                    rand()%((int)boss_y-200)+200);  //update the starting coordinates
                    missile[i]->fireRocket((int)superllama_x+32,(int) superllama_y+32, (int)boss_rotation); //send it the target's current coordinates;
                }
            }
            if(missile[i]->hasBeenFired()){
                missile[i]->recieveNewTargetCoor((int)superllama_x+32, (int)superllama_y+32);
                missile[i]->update();
                if(missile[i]->hitTarget()){
                    damage = 10;
                    missile[i]->kill();
                    play_sample(pop, 1000, 128, rand()%1000+500, FALSE);
                    explosion.push_back(new boom((int)superllama_x, (int)superllama_y));
                }
                else if(missile[i]->getY() > heightMap[missile[i]->getX()]-10){
                    explosion.push_back(new boom(missile[i]->getX()-32, missile[i]->getY()-32));
                    missile[i]->kill();
                    play_sample(pop, 1000, 128, rand()%1000+500, FALSE);
                }
            }
        }
    } else {
        // MAKE RAINBOW ZOMBIE EMERGE AND ROAR
        if(boss_y<300) boss_vy /= 1.05;
        else {
            boss_vy -= .02;
        }
        
        if(bossfight_counter>300){
            boss_vrotation -= .006;
            boss_vy += .01;
        }

        if(boss_rotation<30){
            boss_vrotation += 0.003;
        } else boss_vrotation /= 1.1;
        boss_x += rand()%5 -2;
    }

    boss_rotation += boss_vrotation;
    boss_x += boss_vx;
    boss_y += boss_vy;
}

void Game::generateNewTerrain(int smoothness, float roughness){
    pageNumber ++;
    for(int i=0; i<800; i++){
        heightMap.erase(heightMap.begin(), heightMap.end());
    }
    generateTerrain(smoothness, roughness);
}

void Game::generateTerrain(int smoothness, float roughness){
    float slope; float h;
    // SLOPE is the slope at the current x-value during creation
    // H is the current height at any given x-value
    slope = 0; h = 475;
    // At any given time, the width of the map will be 1000 pixels
    for(int i=0; i<800; i++){
        // the slope changes approximately every SMOOTHNESS iterations
        if(rand()%smoothness==0){
            // slope can increment any amount from 0 to +- ROUGHNESS/2
            slope+=((float)rand()/RAND_MAX*roughness)-(roughness/2);
            // makes sure that the terrain remains within boundaries
            if(h>550) slope = -1;  if(h<400) slope = 1;
            // prevents extreme cliffs of doom and ugliness
            if(slope<-roughness) slope += roughness/2;
            if(slope> roughness) slope -= roughness/2;
            h+=slope;
        }
        heightMap.push_back((int)h);
    }
}

void Game::loadNewZombies(){
    //ERASE BALLOONS
    balloons.erase(balloons.begin(), balloons.end());

    //ERASE DROPLETS
    splatter.erase(splatter.begin(), splatter.end());

    //LOADING GROUND ZOMBIES
    horde.erase(horde.begin(), horde.end());
    for(int i=0; i<5; i++){
        horde.push_back(new zombie(0));
    }

    //LOADING FLYING ZOMBIES
    flyers.erase(flyers.begin(), flyers.end());
    for(int i=0; i<5; i++){
        flyers.push_back(new zombie(1));
    }
}

int Game::comboMoveAvailable(){
     return comboPointCounter;
}

void Game::drawExplosions(BITMAP* b, BITMAP* f[][4]){
    if(explosion.size() > 0){
        for(int i=0; i<explosion.size(); i++){
            explosion[i]->drawExplosion(b, f);
        }
    }
}

void Game::drawDroplets(BITMAP* b, BITMAP* f[][4]){
    for(int i=0; i<splatter.size(); i++){
        splatter[i]->drawDroplet(b, f);
    }
}

void Game::drawBalloons(BITMAP* b, BITMAP* f[]){
    for(int i=0; i<balloons.size(); i++){
        balloons[i]->drawBalloon(b, f);
    }
}

void Game::drawSuperllama(BITMAP* buffer){
    if(leftFacing) draw_sprite_h_flip(buffer, sprite_superllama[superllama_action][superllama_sprite], (int)superllama_x, (int)superllama_y);
    else draw_sprite(buffer, sprite_superllama[superllama_action][superllama_sprite], (int)superllama_x, (int)superllama_y);
}

void Game::drawZombies(BITMAP* buffer, BITMAP* spriteSheet[][2]){
    for(int i = 0; i < horde.size(); i++){
        horde[i]->drawZombie(buffer, spriteSheet);
    }
    for(int i = 0; i < flyers.size(); i++){
        if(flyers[i]->isHit())flyers[i]->drawZombie(buffer, spriteSheet);
        else flyers[i]->drawZombie(buffer, spriteSheet);
    }
}

void Game::drawMissile(BITMAP* b, BITMAP* f[]){
     for(int i = 0; i < missile.size();i++){
         missile[i]->drawTo(b,f);
     }
}

void Game::drawRainbowZombie(BITMAP* buffer, BITMAP* zombieHead[]){
    int eyes = 1;
    if(superllama_y > 100) eyes++;
    if(superllama_y > 200) eyes++;
    if(superllama_y > 300) eyes++;
    if(bossfight_counter>0){
        pivot_sprite(buffer, zombieHead[0], (int)boss_x, (int)boss_y, 200, 50, ftofix(boss_rotation/10.0 -5));
        pivot_sprite(buffer, zombieHead[eyes], (int)boss_x, (int)boss_y, 320, 320, ftofix(boss_rotation));
    }
}

void Game::drawTerrain(BITMAP* buffer){
    for(int i=0; i<heightMap.size(); i++){
        vline(buffer, i, 0, heightMap[i], makecol(255, 0, 255));
        putpixel(buffer, i, heightMap[i], makecol(100, 100, 100));
    }
}

void Game::loadSuperllamaGraphics(){
    BITMAP* sheet_superllama = load_bitmap("superllama.bmp", NULL);
    for(int j=0; j<3; j++){
        for(int i=0; i<8; i++){
            sprite_superllama[j][i] = create_bitmap(64, 64);
            blit(sheet_superllama, sprite_superllama[j][i], i*64, j*64, 0, 0, 64, 64);
        }
    }
}

#endif
