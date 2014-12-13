#ifndef ANIMATION_H
#define ANIMATION_H

#include "include/utils.h"

class Animation
{
    public:
        Animation( SDL_Surface*, int, int, Vec2, int, Vec2 );
        virtual ~Animation();
        bool Play(int);
        void Draw(Vec2, SDL_Surface*, SDL_Surface*);

        int interval;
        Vec2 displacement;

        vector<int> keyTriggers;
        vector<int> stateTriggers;

    private:
        vector<SDL_Rect> clip;
        int framesCount;
        int loopTime;
        int frame;
};

#endif // ANIMATION_H
