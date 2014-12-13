#include "include/utils.h"

#include "include/Game.h"
#include "include/Menu.h"

bool processEvents();

GameScreen* loadScreen( string );

void quitGame( vector<GameScreen*>& );

bool compScreens( GameScreen* ptrScreen ) { return ptrScreen->name == GameScreen::nextScreen; }

int main( int argc, char *argv[] )
{
    if( ( GameScreen::screen = Init( "Simples Engine" ) ) == NULL )
        cout << "Couldn't initialize screen" << endl;

    char pySearchPath[] = "Python27";
    Py_SetPythonHome( pySearchPath );
    Py_SetProgramName( argv[0] );

    bool game_running = true;

    vector<GameScreen*> game_screens;
    game_screens.push_back( loadScreen("main") );
    vector<GameScreen*>::iterator it_screen = game_screens.begin();

    GameScreen::soundMng.PlayMusic( game_screens.back()->track );

    while( game_running )
    {
        game_running = GameScreen::ProcessEvents();

        if( game_screens.back()->Update() )
        {
            if( GameScreen::nextScreen.empty() )
                game_running = false;
            else
            {
                it_screen = find_if( game_screens.begin(), game_screens.end(), compScreens );

                if( it_screen == game_screens.end() )
                    game_screens.push_back( loadScreen( GameScreen::nextScreen ) );
                else
                    game_screens.erase( it_screen + 1, game_screens.end() );

                GameScreen::soundMng.PlayMusic( game_screens.back()->track );
            }
        }

        game_screens.back()->Draw();
    }

    quitGame( game_screens );

    return 0;
}

GameScreen* loadScreen( string archive )
{
    GameScreen* ptrScreen = NULL;
    string element;
    int x, y, w, h, b;

    ifstream data( (ROOTDIR + "data/screens/" + archive + ".txt").c_str() );

    if( !data )
    {
        cout << "File " << archive << " couldn't be opened" << endl;
        return NULL;
    }

    while( data >> element )
    {
        if( element == "screen=" )
        {
            element = GetString( data );
            ptrScreen = new GameScreen( ROOTDIR + "data/backgrounds/" + element );
        }
        else if( element == "game=" )
        {
            element = GetString( data );
            string initFile = GetString( data );
            string loopFile = GetString( data );
            ptrScreen = new Game( ROOTDIR + "data/backgrounds/" + element, ROOTDIR + "data/screens/" + initFile, ROOTDIR + "data/screens/" + loopFile );
        }
        else if( element == "menu=" )
        {
            element = GetString( data );
            ptrScreen = new Menu( ROOTDIR + "data/backgrounds/" + element );
        }
        else if( element == "title=" )
        {
            int s, r, g, b, bw; element = GetString( data );
            data >> y >> r >> g >> b >> s >> bw;

            GameScreen::textMng.WriteTextCentered( element, 0, y-bw, ptrScreen->background->w, s+bw, 0, 0, 0, s, bw, ptrScreen->background );
            GameScreen::textMng.WriteTextCentered( element, 0, y, ptrScreen->background->w, s, r, g, b, s, 0, ptrScreen->background );
        }
        else if( element == "text=" )
        {
            int s, r, g, b, bw; element = GetString( data );
            data >> x >> y >> r >> g >> b >> s >> bw;

            GameScreen::textMng.WriteText( element, x-bw, y-bw, 0, 0, 0, s, bw, ptrScreen->background );
            GameScreen::textMng.WriteText( element, x, y, r, g, b, s, 0, ptrScreen->background );
        }
        else if( element == "music=" )
        {
            element = GetString( data );
            ptrScreen->track = ROOTDIR + "data/audio/" + element;
        }
        else if( element == "button=" )
        {
            string link;
            data >> x >> y >> w >> h >> b >> link;  element = GetString( data );
            ptrScreen->CreateButton( x, y, w, h, b, element, link );
        }
        else if( element == "text_box=" )
        {
            data >> x >> y >> w >> h >> b >> element;
            ptrScreen->CreateTextBox( x, y, w, h, b, element );
        }
        else if( element == "file_box=" )
        {
            data >> x >> y >> w >> h >> b;  string dir = GetString( data ); data >> element;
            ptrScreen->CreateFileBox( x, y, w, h, b, dir, element );
        }
        else if( element == "number_box=" )
        {
            int min, max;
            data >> x >> y >> w >> h >> b >> element >> min >> max;
            string unit = GetString( data );
            ptrScreen->CreateNumberBox( x, y, w, h, b, element, min, max, unit );
        }
        else if( element == "epos_box=" )
        {
            int id, mode, selected;
            data >> x >> y >> w >> h >> b >> element >> id >> mode >> selected;
            ptrScreen->CreateEposBox( x, y, w, h, b, element, id, mode, selected );
        }
        else if( element == "number_value=" )
        {
            data >> element >> x;
            GameScreen::numberValues[element] = x;
        }
        else if( element == "string_value=" )
        {
            data >> element;    string s_value = GetString( data );
            GameScreen::stringValues[element] = s_value;
        }
        else if( element == "return=" )
            data >> ptrScreen->returnScreen;
        else if( element == "#" )
            SkipLine( data );
    }

    data.close();

    ptrScreen->name = archive;

    return ptrScreen;
}

void quitGame( vector<GameScreen*>& screens )
{
    for( vector<GameScreen*>::iterator it = screens.begin(); it != screens.end(); ++it )
        delete (*it);
    screens.clear();

    Quit();
}
