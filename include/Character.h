#ifndef CHARACTER_H
#define CHARACTER_H

#include "include/Object.h"
#include "include/Animation.h"


class Character : public Object
{
    public:
        Character( Vec2, Vec2, string, double = 1.0 );
        virtual ~Character();

        void Configure( ifstream& );

        virtual int GetWidth() { return (int)size.x; }
        virtual int GetHeight() { return (int)size.y; }

        virtual void Update( int );
        virtual void Draw();

        vector<Animation> animations;

    private:
        Vec2 size;
        int oldState;
};

#endif // CHAR_H
