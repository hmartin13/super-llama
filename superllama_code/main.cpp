#include <allegro.h>
#include <stdio.h>
#include "Music.h"
#include <vector>
#include <string>
#include "Game.h"

void init();
void deinit();
void loadAudio();
void loadGraphics();
void drawColorometer();
void drawHealthBar();

int health = 255; // out of 255

int MAX_SCORE;
int score;
int colorcounter;
bool thePartyIsStarted;
bool groundShake = false;

BITMAP* title;      // the title page image
BITMAP* fail;
BITMAP* failtext;
BITMAP* victory;
BITMAP* missiles[15];
BITMAP* explosions[15][4];

float zoom = 1.0;

BITMAP* buffer;     // the canvass for buffering
BITMAP* rocks[6];   // 0=grey ... 5=grassy
BITMAP* sky[6];     // 0=grey ... 5=awesome
BITMAP* zombies[13][2];
BITMAP* balloons[13];
BITMAP* splatter[13][4];
BITMAP* rainbowZombie[5];
BITMAP* foreground;
int color = 0;

vector<int> colors;

int frameRateCounter = 0;   // used to limit framerate to 60 FPS
int currentPage = 0;        // 0: title screen
                            // 1: gameplay
                            // 2: failure
                            // 3: victory!

Music* gameMusic;   // the object used to loop background music
Game* currentGame;  // the object used to store gameplay logic

int main() { init();
	while (!key[KEY_ESC]) {        // ESCAPE key to exit from any page
        while(frameRateCounter>0){
            gameMusic->update();

            if(currentPage == 0){                                       // TITLE PAGE
                blit(title, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);    //
                if(key[KEY_ENTER]){                                     //
                    currentPage = 1;                                    //
                    currentGame = new Game();                           //
                    gameMusic->start("superllama2.wav");                //
                }
            } else if(currentPage == 1){                                         // GAMEPLAY
                score += currentGame->comboMoveAvailable();
                if(score>MAX_SCORE) score=MAX_SCORE;
                color = (int)((float)score/MAX_SCORE*5.0);
                
                health -= currentGame->llamaDamage();
                
                currentGame->keyboardListener();                                // USER INPUT
                if(currentGame->update(score, MAX_SCORE)){
                    groundShake = true;
                    gameMusic->start("roar.wav");
                }
                else if(!thePartyIsStarted && currentGame->stuffIsGoingDown()){
                    gameMusic->start("superllama3.wav");
                    thePartyIsStarted = true;
                    groundShake = false;
                    health = 255;
                }
                                                                                //
                blit(sky[color], buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);  // BACKGROUND (sky)
                currentGame->drawRainbowZombie(buffer, rainbowZombie);
                blit(rocks[color], foreground, 0, 0, 0, 0, SCREEN_W, SCREEN_H);     // FOREGROUND (rocks)
                currentGame->drawTerrain(foreground);                           //
                if(groundShake) masked_blit(foreground, buffer, 0, 0, 0, rand()%5, SCREEN_W, SCREEN_H);
                else masked_blit(foreground, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                currentGame->drawZombies(buffer, zombies);
                currentGame->drawDroplets(buffer, splatter);
                currentGame->drawBalloons(buffer, balloons);
                
                if(currentGame->getPage()==0) textprintf_centre_ex(buffer, font, 400, 270, 0, -1, "Use the ARROW KEYS (or A and D) to MOVE!");
                if(currentGame->getPage()==1) textprintf_centre_ex(buffer, font, 400, 270, 0, -1, "Hold down the UP arrow (or W) to FLY");
                if(currentGame->getPage()==2) textprintf_centre_ex(buffer, font, 400, 270, 0, -1, "Squish COLOR ZOMBIES to earn POINTS!");
                if(currentGame->getPage()==2) textprintf_centre_ex(buffer, font, 400, 290, 0, -1, "Squish them faster for COMBO POINTS!");
                if(currentGame->getPage()==3) textprintf_centre_ex(buffer, font, 400, 270, 0, -1, "Good luck, SUPERLLAMA!");
                if(currentGame->getPage()==3) score = 9999;
                                                                                //
                if(!thePartyIsStarted) drawColorometer();
                drawHealthBar();
                currentGame->drawMissile(buffer, missiles);
                currentGame->drawSuperllama(buffer);       
                currentGame->drawExplosions(buffer, explosions);                     //
                
                if(health < 1) currentPage = 2;
                if(currentGame->victory()) {
                    gameMusic->start("superllama1.wav");
                    currentPage = 3;
                }
            } else if(currentPage == 2){
                zoom += 0.0001;
                pivot_scaled_sprite(buffer, fail, 400, 300, 400, 300, ftofix(0), ftofix(zoom));
                draw_sprite(buffer, failtext, 400-failtext->w/2, 200);
            } else if(currentPage == 3){
                draw_sprite(buffer, victory, 0, 0);
            }

            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);               // DRAW GRAPHICS
            frameRateCounter--;
        }
        clear_keybuf();
	}

	deinit();
	return 0;
}
END_OF_MAIN()

void drawColorometer(){
    colorcounter++;
    // ADD NEW COLORS IF NECESSARY
    if(colors.size()<(float)score/MAX_SCORE*760){
        // COLORS IN COLOROMETER ARE BASED ON CURRENT SCORE
        colors.push_back(makecol(rand()%(score*155/MAX_SCORE)+100,
            rand()%(score*155/MAX_SCORE)+100, rand()%(score*155/MAX_SCORE)+100));
    }
    
    if(colors.size()>0){
        rect(buffer, 19, 19, colors.size()+20, 41, colors[0]);
        for(int i=0; i<colors.size(); i++){
            // SLIDE COLORS SIDEWAYS
            if(colorcounter>5){ colorcounter = 0;
                colors.push_back(makecol(rand()%(score*155/MAX_SCORE)+100,
                rand()%(score*155/MAX_SCORE)+100, rand()%(score*155/MAX_SCORE)+100));
                colors.erase(colors.begin());
            }

            // DRAW COLORS!
            vline(buffer, i+20, 20, 40, colors[i]);
        }
    }
    
    // supercombotaculometer
    if(currentGame->comboMoveAvailable()>0)
        rectfill(buffer, 20, 50, 20+currentGame->comboMoveAvailable(),
            60, makecol(255, 255, 255));
}

void drawHealthBar(){
    rectfill(buffer, 19, 559, 276, 581, 0);
    rectfill(buffer, 20, 560, 20+health, 580, makecol(255-health, health, 0));
}

void checkFrameRate(){ frameRateCounter++; }
END_OF_FUNCTION(checkFrameRate);

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	srand(time(0));

    thePartyIsStarted = false;
	MAX_SCORE = 999999;
	colorcounter = 0;
	score = 0;

    // Integrated Input Interface Initializations

	install_timer();
	install_keyboard();
	install_mouse();
	
	loadAudio();
	loadGraphics();
	
	LOCK_VARIABLE(frameRateCounter);
	LOCK_FUNCTION(checkFrameRate);
	install_int_ex(checkFrameRate, BPS_TO_TIMER(60));
}

void loadAudio(){
    gameMusic = new Music();
    gameMusic->start("superllama1.wav");
    
    //install a digital sound driver
    if(install_sound(DIGI_AUTODETECT, MIDI_NONE, "") != 0) {
        allegro_message("Error initializing sound system");
        return;
    }
}

void loadGraphics(){
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    title = load_bitmap("title.bmp", NULL);
    fail = load_bitmap("fail.bmp", NULL);
    failtext = load_bitmap("failtext.bmp", NULL);
    victory = load_bitmap("victory.bmp", NULL);
    
    foreground = create_bitmap(SCREEN_W, SCREEN_H);
    
    sky[0] = load_bitmap("sky0.bmp", NULL);
    sky[1] = load_bitmap("sky1.bmp", NULL);
    sky[2] = load_bitmap("sky2.bmp", NULL);
    sky[3] = load_bitmap("sky3.bmp", NULL);
    sky[4] = load_bitmap("sky4.bmp", NULL);
    sky[5] = load_bitmap("sky5.bmp", NULL);
    
    rocks[0] = load_bitmap("rocks0.bmp", NULL);
    rocks[1] = load_bitmap("rocks1.bmp", NULL);
    rocks[2] = load_bitmap("rocks2.bmp", NULL);
    rocks[3] = load_bitmap("rocks3.bmp", NULL);
    rocks[4] = load_bitmap("rocks4.bmp", NULL);
    rocks[5] = load_bitmap("rocks5.bmp", NULL);
    
    rainbowZombie[0] = load_bitmap("rainbowzombie_neck.bmp", NULL);
    rainbowZombie[1] = load_bitmap("rainbowzombie_head1.bmp", NULL);
    rainbowZombie[2] = load_bitmap("rainbowzombie_head2.bmp", NULL);
    rainbowZombie[3] = load_bitmap("rainbowzombie_head3.bmp", NULL);
    rainbowZombie[4] = load_bitmap("rainbowzombie_head4.bmp", NULL);
    
    BITMAP* zombieSheet = load_bitmap("zombies.bmp", NULL);
    
    for(int i=0; i<13; i++){
        balloons[i] = create_bitmap(48, 51);
        zombies[i][0] = create_bitmap(48, 64);
        zombies[i][1] = create_bitmap(48, 115);
        blit(zombieSheet, balloons[i], 48*i, 0, 0, 0, 48, 51);
        blit(zombieSheet, zombies[i][0], 48*i, 51, 0, 0, 48, 64);
        blit(zombieSheet, zombies[i][1], 48*i, 0, 0, 0, 48, 115);
        for(int j=0; j<4; j++){
            splatter[i][j] = create_bitmap(48, 40);
            blit(zombieSheet, splatter[i][j], 48*i, 115+40*j, 0, 0, 48, 40);
        }
    }
    
    BITMAP* missileSheet = load_bitmap("missiles.bmp",NULL);
    for(int i = 0; i<15; i++){
            missiles[i] = create_bitmap(32,17);
            blit(missileSheet, missiles[i], 0, 17*i,0,0,32,17);
    }
    
    BITMAP* explosionSheet = load_bitmap("boom.bmp",NULL);
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 4; j++){
            explosions[i][j] = create_bitmap(64,64);
            blit(explosionSheet, explosions[i][j], i*64, j*64,0,0,64,64);
        }
    }        
}

void deinit() {
	clear_keybuf();
	destroy_bitmap(title);
	destroy_bitmap(fail);
	destroy_bitmap(failtext);
	destroy_bitmap(victory);
	destroy_bitmap(buffer);
	for(int i=0; i<6; i++){
        destroy_bitmap(sky[i]);
        destroy_bitmap(rocks[i]);
        if(i<5) destroy_bitmap(rainbowZombie[i]);
    }
    for(int i=0; i<13; i++){
        destroy_bitmap(balloons[i]);
        destroy_bitmap(zombies[i][0]);
        destroy_bitmap(zombies[i][1]);
        for(int j=0; j<4; j++){
            destroy_bitmap(splatter[i][j]);
        }
    }
    for(int i = 0; i < 15; i++){
        destroy_bitmap(missiles[i]);
        for(int j = 0; j < 4; j++){
                destroy_bitmap(explosions[i][j]);
        }
    }
    destroy_bitmap(foreground);
    remove_sound();
}
