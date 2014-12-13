#include "include/Animation.h"

Animation::Animation( SDL_Surface* spriteSheet, int startFrame, int endFrame, Vec2 frameSize, int interval, Vec2 displacement )
{
    //ctor
    framesCount = endFrame - startFrame + 1;
    this->interval = interval;
    this->displacement = displacement;
    loopTime = 0;

    clip.resize( framesCount );

    for( frame = 0; frame < framesCount; frame++ )
    {
        clip[frame].x = ( (startFrame + frame) * (int)frameSize.x ) % spriteSheet->w;
        clip[frame].y = ( ( (startFrame + frame) * (int)frameSize.y ) / spriteSheet->w ) * (int)frameSize.x;
        clip[frame].w = (int)frameSize.x;
        clip[frame].h = (int)frameSize.y;
    }

    frame = 0;
}

Animation::~Animation()
{
    //dtor
}

bool Animation::Play( int elapsedTime )
{

    if (loopTime + elapsedTime >= interval)
    {
        frame = ( frame + 1 ) % framesCount;
        loopTime = 0;

        return true;
    }
    else
    {
        loopTime += elapsedTime;

        return false;
    }
}

void Animation::Draw( Vec2 place, SDL_Surface *spriteSheet, SDL_Surface *screen )
{
    int drawX = place.x - clip[frame].w / 2;
    int drawY = place.y - 2 * clip[frame].h / 3;

    ApplySurface( drawX, drawY, spriteSheet, screen, &clip[frame]);
}
