#ifndef musich
#define musich
#include "fmod.h"
#include <string>
#include <allegro.h>
using namespace std;

class Music {
    FSOUND_STREAM *music;
    FSOUND_SAMPLE *sound;

    public:
        Music(){ FSOUND_Init(32000, 64, 0); }
        
        void start(const char* str){
            music = FSOUND_Stream_Open(str, FSOUND_2D, 0, 0);
        }

        void playSound(const char* str){
            sound = FSOUND_Sample_Load(0, str, 0, 0, 10);
            FSOUND_PlaySound (1, sound);
        }
        
        void update(){
            FSOUND_Stream_Play(0, music);
        }
};
#endif
