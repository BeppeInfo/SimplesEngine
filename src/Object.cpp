#include "include/Object.h"

#include "include/Game.h"

Object::Object( Vec2 position, string icon, double scale )
{
    //ctor
    spriteSheet = spriteSheet = zoomSurface( LoadImage( icon ), scale, scale, SMOOTHING_ON );

    this->position = position;

    movable = false;

    state = 0;
}

Object::~Object()
{
    //dtor
    SDL_FreeSurface( spriteSheet );

    Game::actors.erase( find( Game::actors.begin(), Game::actors.end(), this ) );
}

void Object::Update( int timing )
{
    return;
}

void Object::Draw()
{
    int X = ( GetXPosition() - GetWidth() / 2 ) - Game::Map->MapGetXPosition();
    int Y = ( GetYPosition() - 2 * GetHeight() / 3 ) - Game::Map->MapGetYPosition();

    ApplySurface( X, Y, spriteSheet, GameScreen::screen, NULL );
}

bool Object::Collision( Vec2 target )
{
    if ( target.x > Game::Map->GetMapWidth() or target.x < 0)
        return true;
    else if ( target.y > Game::Map->GetMapHeight() or target.y < 0)
        return true;
    else
    {
        currentBlock = Game::Map->MapGetBlock( target.x / Game::Map->GetMapBlockWidth(), target.y / Game::Map->GetMapBlockHeight() );

        return ( currentBlock->bl or currentBlock->br or currentBlock->tl or currentBlock->tr );
    }

    for( vector<Object*>::iterator it = Game::actors.begin(); it != Game::actors.end(); ++it )
    {
        if( ObjCollision( target, (*it) ) )
               return true;
    }

    return false;
}

bool Object::ObjCollision( Vec2 target, Object* obj )
{
    if( obj != this )
    {
        if( abs( (int)target.x - obj->GetXPosition() ) < abs( GetWidth() + obj->GetWidth() ) / 2
        and abs( (int)target.y - obj->GetYPosition() ) < abs( GetHeight() + obj->GetHeight() ) / 3 )
            return true;
    }

    return false;
}
