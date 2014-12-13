#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include "include/utils.h"

#include "include/InputBoxes.h"
#include "include/SoundManager.h"
#include "include/TextManager.h"
#include "include/LoggerClient.h"
#include "include/EposManager.h"

class GameScreen
{
    public:

        GameScreen(string = "");
        virtual ~GameScreen();

        static bool ProcessEvents();
        virtual bool Update();
        virtual void Draw();

        static SDL_Surface* screen;

        static SDL_Event event;
        static bool mousePress, mouseClick;
        static bool keyPress, keyClick;
        static uint16_t mouseX, mouseY;

        static SoundManager& soundMng;
        static TextManager& textMng;
        static EposManager& eposMng;

        void CreateButton( int, int, int, int, int, string, string );

        void CreateTextBox( int, int, int, int, int, string );
        void CreateFileBox( int, int, int, int, int, string, string );
        void CreateNumberBox( int, int, int, int, int, string, int, int, string );
        void CreateEposBox( int, int, int, int, int, string, int, int, int );

        SDL_Surface* background;

        string name;
        string returnScreen;
        static string nextScreen;

        string track;

        static Uint32 absoluteTime;
        static Uint32 elapsedTime;

        static map<string,string> &stringValues;
        static map<string,int> &numberValues;

    protected:

        vector<InputBox*> inputBoxes;

    private:

        static map<string,string> &SetStringValues()
        {
            static map<string,string> svs;

            svs["map_name"] = ListFiles( ROOTDIR + "data/maps" )[0];
            if( svs["map_name"].empty() ) cout << "No map file found" << endl;

            svs["font_name"] = ListFiles( ROOTDIR + "data/fonts" )[0];
            if( svs["font_name"].empty() ) cout << "No font file found" << endl;

            svs["character"] = ListFiles( ROOTDIR +  "data/chars" )[0];
            if( svs["character"].empty() ) cout << "No char folder found" << endl;

            svs["item_icon"] = ListFiles( ROOTDIR + "data/items" )[0];
            if( svs["item_icon"].empty() ) cout << "No item image found" << endl;

            svs["button_icon"] = svs["box_icon"] = ListFiles( ROOTDIR + "data/icons" )[0];
            if( svs["button_icon"].empty() or svs["box_icon"].empty() ) cout << "No icon image found" << endl;

            svs["player_name"] = "Player";
            svs["server_address"] = "localhost";

            return svs;
        }

        static map<string,int> &SetNumberValues()
        {
            static map<string,int> nvs;
            nvs["num_enemies"] = 1;
            nvs["num_items"] = 10;
            nvs["game_speed"] = 1;
            nvs["port_number"] = 2000;
            nvs["use_mouse"] = 1;
            nvs["use_arrows"] = 1;
            return nvs;
        }
};

#endif // GAME_SCREEN_H
