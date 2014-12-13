#include "include/Character.h"

#include "include/Game.h"

Character::Character( Vec2 position, Vec2 size, string spriteFile, double scale )
:Object( position, spriteFile, scale )
{
    this->size = (float)scale * size;

    movable = true;

    oldState = state;
}

Character::~Character()
{
    //dtor
    SDL_FreeSurface( spriteSheet );

    animations.clear();

    Game::actors.erase( find( Game::actors.begin(), Game::actors.end(), this ) );
}

void Character::Configure( ifstream& configFile )
{
    string data;

    int firstFrame = 0, lastFrame = 0, interval = 100;
    int dx = 0.0f, dy = 0.0f;
    float vx = 0.0f, vy = 0.0f;

    while( configFile >> data )
    {
        if( data == "ANIMATION:" )
        {
            vector<int> states;

            while( configFile >> data and data != ":ANIMATION" )
            {
                if( data == "first_last_frames=" )
                    configFile >> firstFrame >> lastFrame;
                else if( data == "speed=" )
                {
                    configFile >> vx >> vy;
                    dx = vx * interval / 1000;
                    dy = vy * interval / 1000;
                }
                else if( data == "update_interval=" )
                    configFile >> interval;
                else if( data == "update_move=" )
                    configFile >> dx >> dy;
                else if( data == "state_conditions=" )
                    states = ReadValues( configFile );
                else if( data == "#" )
                    SkipLine( configFile );
            }

            animations.push_back( Animation( spriteSheet, firstFrame, lastFrame, size, interval, Vec2(dx,dy) ) );
            animations.back().stateTriggers = states;
        }
    }
}

void Character::Update( int timing )
{
    Uint8* keyStates = SDL_GetKeyState( NULL );

    if( !animations.empty() )
    {
        if( state == oldState )
        {
            for( int id = (int)animations.size() - 1; id >= 0 ; id-- )
            {
                int checked = 0;
                for( size_t trigger = 0; trigger < animations[id].keyTriggers.size(); trigger++ )
                {
                    if( keyStates[ animations[id].keyTriggers[trigger] ] )
                        checked++;
                }

                bool ready = true;
                for( size_t trigger = 0; trigger < animations[id].stateTriggers.size(); trigger++ )
                {
                    if( state == abs(animations[id].stateTriggers[trigger]) )
                    {
                        ready = ( animations[id].stateTriggers[trigger] >= 0 ) ? true: false;
                        break;
                    }
                    else
                        ready = ( animations[id].stateTriggers[trigger] >= 0 ) ? false: true;
                }

                if( checked == (int)animations[id].keyTriggers.size() and ready )
                {
                    state = id;
                    break;
                }
            }

            oldState = state;
        }

        state = state % animations.size();

        if( animations[state].Play( timing ) and !Collision( position + animations[state].displacement ) )
            position += animations[state].displacement;
    }

    return;
}

void Character::Draw()
{
    int X = (int)position.x - Game::Map->MapGetXPosition();
    int Y = (int)position.y - Game::Map->MapGetYPosition();

    if( !animations.empty() ) animations[state].Draw( Vec2(X,Y), spriteSheet, GameScreen::screen );
}
