#include "include/Hero.h"

//#include "include/GameScreen.h"
#include "include/Game.h"

Hero::Hero( Vec2 position, Vec2 size, string spriteFile, double scale )
    :Character( position, size, spriteFile, scale )
{
    //ctor
}

Hero::~Hero()
{
    //dtor
}

void Hero::Update( int timing )
{
    /*if( GameScreen::numberValues["use_arrows"] )
    {
        if( GameScreen::event.type == SDL_KEYDOWN  )
        {
            if( GameScreen::event.key.keysym.sym == SDLK_RIGHT )
                state = WALK_RIGHT;
            else if( GameScreen::event.key.keysym.sym == SDLK_LEFT )
                state = WALK_LEFT;
            else if( GameScreen::event.key.keysym.sym == SDLK_UP )
                state = WALK_UP;
            else if( GameScreen::event.key.keysym.sym == SDLK_DOWN )
                state = WALK_DOWN;
        }
        else
        {
            if( state == WALK_RIGHT ) state = STOP_RIGHT;
            else if( state == WALK_LEFT ) state = STOP_LEFT;
            else if( state == WALK_UP ) state = STOP_UP;
            else if( state == WALK_DOWN ) state = STOP_DOWN;
        }
    }*/

        /*if( GameScreen::eposMng.epos->GetPosition() > GameScreen::numberValues["epos_pos_max"] )
        {
            if( state == WALK_LEFT )  state = WALK_UP;
            else    state++;
        }
        else if( GameScreen::eposMng.epos->GetPosition() < GameScreen::numberValues["epos_pos_min"] )
        {
            if( state == WALK_UP )  state = WALK_LEFT;
            else    state--;
        }*/

    Character::Update( timing );

    return;
}
