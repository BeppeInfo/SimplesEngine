#include "include/Menu.h"

InputBox* Menu::handle;
uint8_t Menu::currentBox;

Menu::Menu( string imageName )
    :GameScreen( imageName )
{
    //ctor
    currentBox = 0;

    handle = NULL;
}

Menu::~Menu()
{
    //dtor
    for( vector<InputBox*>::iterator it = inputBoxes.begin(); it != inputBoxes.end(); ++it )
        delete (*it);
    inputBoxes.clear();
}

bool Menu::Update()
{
    if( !inputBoxes.empty() and handle == NULL )
    {
        handle = inputBoxes.front();
        handle->SnapCursor();
    }

    if( GameScreen::Update() )
    {
        if(handle) handle->selected = false;
        handle = NULL;
        currentBox = 0;
        return true;
    }

    if( keyClick and event.key.keysym.sym == SDLK_UP )
    {
        currentBox = ( currentBox > 0 ) ? currentBox - 1 : inputBoxes.size() - 1;
        inputBoxes[currentBox]->SnapCursor();
    }
    else if( keyClick and event.key.keysym.sym == SDLK_DOWN )
    {
        currentBox = ( currentBox < inputBoxes.size() - 1 ) ? currentBox + 1 : 0;
        inputBoxes[currentBox]->SnapCursor();
    }

    for( vector<InputBox*>::iterator it = inputBoxes.begin(); it != inputBoxes.end(); ++it )
    {
        if( (*it)->CursorInside() and (
        ( mouseClick and SDL_ShowCursor( SDL_QUERY ) == SDL_ENABLE )
        or ( keyClick and event.key.keysym.sym == SDLK_RETURN ) ) )
        {
            soundMng.PlaySound( ROOTDIR + "data/audio/hotclick.wav" );
            if( handle ) handle->selected = false;
            handle = (*it);
            handle->selected = true;
            break;
        }
    }

    if( handle )
    {
        if( handle->selected )
        {
            if( handle->Update() )
            {
                nextScreen = handle->BoxText();
                handle->selected = false;
                handle = NULL;
                currentBox = 0;
                return true;
            }
        }
    }

    return false;
}
