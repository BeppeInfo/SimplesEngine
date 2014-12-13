#ifndef GAME_H
#define GAME_H

#include "include/GameScreen.h"
#include "include/Hero.h"

#if	defined _WIN32
    #include "Python.h"
#else
	#include </usr/include/python2.7/Python.h>
#endif

class Game : public GameScreen
{
    public:
        Game(string = "", string = "init_game.py", string = "game_logic.py");
        virtual ~Game();

        static Character* SetChar( Vec2, string, double = 1.0, string = "" );

        virtual bool Update();
        virtual void Draw();

        static SDLMappy *Map;

        static LoggerClient& logClient;

        static vector<Object*> actors;

        static bool gameEnded;

    private:
        void CreateMap();
        void MoveMap();
        void UpdateMap();
};

#endif // GAME_H
