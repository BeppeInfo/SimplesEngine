#include "include/GameScreen.h"

map<string,string>& GameScreen::stringValues = GameScreen::SetStringValues();
map<string,int>& GameScreen::numberValues = GameScreen::SetNumberValues();

SDL_Event GameScreen::event;
bool GameScreen::mousePress = false;
bool GameScreen::mouseClick = false;
bool GameScreen::keyPress = false;
bool GameScreen::keyClick = false;
uint16_t GameScreen::mouseX = 0;
uint16_t GameScreen::mouseY = 0;

SDL_Surface* GameScreen::screen = NULL;

SoundManager& GameScreen::soundMng = SoundManager::Activate();
TextManager& GameScreen::textMng = TextManager::Activate();
EposManager& GameScreen::eposMng = EposManager::Activate();

string GameScreen::nextScreen;

Uint32 GameScreen::absoluteTime = SDL_GetTicks();
Uint32 GameScreen::elapsedTime;

GameScreen::GameScreen( string imageName )
{
    //ctor
    nextScreen.clear();

    background = LoadImage( imageName );

    elapsedTime = 0;
}

GameScreen::~GameScreen()
{
    //dtor
    SDL_FreeSurface( background );
}

bool GameScreen::ProcessEvents()
{
    SDL_ShowCursor( numberValues["use_mouse"] );

    elapsedTime = ( SDL_GetTicks() - absoluteTime ) * numberValues["game_speed"];
    absoluteTime = SDL_GetTicks();

    mouseClick = false;
    keyClick = false;

    while( SDL_PollEvent( &event ) );
    {
        if( event.type == SDL_QUIT )
            return false;

        if( event.type == SDL_MOUSEMOTION )
        {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }

        if( SDL_ShowCursor( SDL_QUERY ) == SDL_ENABLE and event.type == SDL_MOUSEBUTTONDOWN )
            mouseClick = true;
        else if( mousePress and event.type == SDL_MOUSEBUTTONUP )
        {
            mouseClick = true;
            mousePress = false;
        }

        if( event.type == SDL_KEYDOWN )
            keyPress = true;
        else if( keyPress and event.type == SDL_KEYUP )
        {
            keyClick = true;
            keyPress = false;
        }
    }

    return true;
}

bool GameScreen::Update()
{
    if( keyClick and event.key.keysym.sym == SDLK_ESCAPE )
    {
        nextScreen = returnScreen;
        return true;
    }

    return false;
}

void GameScreen::Draw()
{
    if( background )
        ApplySurface((screen->w - background->w)/2, (screen->h - background->h)/2, background, screen);

    for( vector<InputBox*>::iterator it = inputBoxes.begin(); it != inputBoxes.end(); ++it )
        (*it)->Draw();

    SDL_Flip( screen );

    numberValues["fps"] = (int)(1000.0 / elapsedTime);

    return;
}

void GameScreen::CreateButton( int x, int y, int w, int h, int bw, string name, string link )
{
    inputBoxes.push_back( new Button(x, y, w, h, bw, name, link) );
}

void GameScreen::CreateTextBox( int x, int y, int w, int h, int bw, string value_key )
{
    inputBoxes.push_back( new TextTypeBox(x, y, w, h, bw, GameScreen::stringValues[value_key]) );
}

void GameScreen::CreateFileBox( int x, int y, int w, int h, int bw, string dir_name, string value_key )
{
    inputBoxes.push_back( new FileSelectionBox(x, y, w, h, bw, dir_name, GameScreen::stringValues[value_key]) );
}

void GameScreen::CreateNumberBox( int x, int y, int w, int h, int bw, string value_key, int min_value, int max_value, string unit )
{
    inputBoxes.push_back( new NumberSelectionBox(x, y, w, h, bw, GameScreen::numberValues[value_key], min_value, max_value, unit) );
}

void GameScreen::CreateEposBox( int x, int y, int w, int h, int bw, string value_key, int id, int mode, int selected )
{
    inputBoxes.push_back( new EposValueBox(x, y, w, h, bw, GameScreen::numberValues[value_key], id, mode) );

    inputBoxes.back()->selected = (bool)selected;
}
