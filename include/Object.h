#ifndef OBJECT_H
#define OBJECT_H

#include "include/utils.h"

class Object
{
    public:
        Object( Vec2, string, double = 1.0 );
        virtual ~Object();

        virtual void Update( int );
        virtual void Draw();

        int GetXPosition() { return (int)position.x; }
        int GetYPosition() { return (int)position.y; }
        Vec2 GetPosition() { return position; }
        void SetPosition( Vec2 position ) { this->position = position; }
        void SetPosition( int x, int y ) { position.Set(x, y); }

        virtual int GetWidth() { return spriteSheet->w; }
        virtual int GetHeight() { return spriteSheet->h; }

        bool ObjCollision( Vec2, Object* );
        bool Collision( Vec2 );

        int state;
        bool movable;

    protected:
        SDL_Surface* spriteSheet;
        Vec2 position;

    private:
        BLKSTR *currentBlock;
};

#endif // OBJECT_H
