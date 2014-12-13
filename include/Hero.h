#ifndef HERO_H
#define HERO_H

#include "include/Character.h"

class Hero : public Character
{
    public:
        Hero( Vec2, Vec2, string, double = 1.0 );
        virtual ~Hero();

        virtual void Update( int );
};

#endif // HERO_H
