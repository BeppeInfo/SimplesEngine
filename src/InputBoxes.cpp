#include "include/InputBoxes.h"

#include "include/GameScreen.h"

InputBox::InputBox(int x, int y, int w, int h, int bw, string iconFile)
{
    //ctor
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->bw = bw;

    box.x = x - bw;
    box.y = y - bw;
    box.w = w + 2*bw;
    box.h = h + 2*bw;

    icon = LoadImage( iconFile );
    icon = zoomSurface( icon, (double)w/icon->w, (double)h/icon->h, SMOOTHING_ON );

    selected = false;
}

Button::Button( int x, int y, int w, int h, int bw, string text, string link )
    :InputBox( x, y, w, h, bw, ROOTDIR + "data/icons/"  + GameScreen::stringValues["button_icon"] )
{
    //ctor
    this->link = link;

    GameScreen::textMng.WriteTextCentered( text, 0, 0, icon->w, icon->h, 255, 180, 0, icon->h/2, 0, icon );
}

TextTypeBox::TextTypeBox(int x, int y, int w, int h, int bw, string& ref_text)
    :InputBox( x, y, w, h, bw, ROOTDIR + "data/icons/"  + GameScreen::stringValues["box_icon"] ), ref_text(ref_text)
{
    //ctor
    if( ref_text.empty() ) ref_text = "";
}

FileSelectionBox::FileSelectionBox(int x, int y, int w, int h, int bw, string dir_name, string& ref_file)
    :InputBox(x, y, w, h, bw, ROOTDIR + "data/icons/"  + GameScreen::stringValues["box_icon"] ), ref_file(ref_file)
{
    file_list = ListFiles( ROOTDIR + dir_name );

    file_it = find( file_list.begin(), file_list.end(), ref_file );

    if( file_it == file_list.end() ) file_it = file_list.begin();
}

NumberSelectionBox::NumberSelectionBox(int x, int y, int w, int h, int bw, int& ref_value, int min_value, int max_value, string unit)
    :InputBox( x, y, w, h, bw, ROOTDIR + "data/icons/"  + GameScreen::stringValues["box_icon"] ), ref_value(ref_value)
{
    //ctor
    this->min_value = min_value;
    this->max_value = max_value;
    this->unit = unit;

    if( max_value - min_value )
    {
        if( ref_value > max_value ) ref_value = max_value;
        else if( ref_value < min_value ) ref_value = min_value;
    }
}

EposValueBox::EposValueBox(int x, int y, int w, int h, int bw, int& ref_value, int eposId, int mode)
    :InputBox( x, y, w, h, bw, ROOTDIR + "data/icons/"  + GameScreen::stringValues["box_icon"] ), ref_value(ref_value)
{
    //ctor
    this->eposId = eposId;
    this->mode = mode;
}

InputBox::~InputBox()
{
    //dtor
    SDL_FreeSurface(icon);
}

Button::~Button()
{
    //dtor
}

TextTypeBox::~TextTypeBox()
{
    //dtor
}

FileSelectionBox::~FileSelectionBox()
{
    //dtor
    file_list.clear();
}

NumberSelectionBox::~NumberSelectionBox()
{
    //dtor
}

EposValueBox::~EposValueBox()
{
    //dtor
}

void InputBox::Draw()
{
    if( CursorInside() )
        SDL_FillRect( GameScreen::screen, &box, SDL_MapRGB(GameScreen::screen->format, 255, 255, 255) );
    else if( selected )
        SDL_FillRect( GameScreen::screen, &box, SDL_MapRGB(GameScreen::screen->format, 255, 255, 0) );

    ApplySurface( x, y, icon, GameScreen::screen );

    GameScreen::textMng.WriteTextCentered( BoxText(), x, y, icon->w, icon->h, 0, 255, 0, 2 * icon->h / 3, 0, GameScreen::screen );

    return;
}

void Button::Draw()
{
    if ( CursorInside() or selected )
        SDL_FillRect( GameScreen::screen, &box, SDL_MapRGB(GameScreen::screen->format, 255, 255, 0) );

    ApplySurface(x, y, icon, GameScreen::screen);

    return;
}

void InputBox::SnapCursor()
{
    SDL_WarpMouse( box.x + box.w / 2, box.y + box.h / 2 );
}

bool InputBox::CursorInside()
{
    if( GameScreen::mouseX > x && GameScreen::mouseX < x + w
    && GameScreen::mouseY > y && GameScreen::mouseY < y + h )
        return true;
    else
        return false;
}

bool Button::Update()
{
    return true;
}

bool TextTypeBox::Update()
{
    if( GameScreen::keyPress )
    {
        SDL_Delay(200);

        if( GameScreen::event.key.keysym.sym == SDLK_BACKSPACE )
        {
            if( ref_text.size() > 0 ) ref_text.erase( ref_text.length() - 1 );
        }
        else
        {
            if( ref_text.size() > 0 ) ref_text += (char)GameScreen::event.key.keysym.unicode;
            else ref_text += ( (char)GameScreen::event.key.keysym.unicode - 32 );
        }
    }

    return false;
}

bool FileSelectionBox::Update()
{
    if( GameScreen::keyClick )
    {
        if( GameScreen::event.key.keysym.sym == SDLK_RIGHT )
        {
            if( file_it != file_list.end() - 1 ) file_it++;
            else file_it = file_list.begin();
        }
        else if( GameScreen::event.key.keysym.sym == SDLK_LEFT )
        {
            if( file_it != file_list.begin() ) file_it--;
            else file_it = file_list.end() - 1;
        }
    }

    ref_file = (*file_it);

    return false;
}

bool NumberSelectionBox::Update()
{
    if( GameScreen::keyClick )
    {
        if( GameScreen::event.key.keysym.sym == SDLK_RIGHT and ref_value < max_value )
            ref_value++;
        else if( GameScreen::event.key.keysym.sym == SDLK_LEFT and ref_value > min_value )
            ref_value--;
    }

    return false;
}

bool EposValueBox::Update()
{
    switch( mode )
    {
        case POS:   ref_value = GameScreen::eposMng.eposList[eposId].GetPosition();
            break;
        case VEL:   ref_value = GameScreen::eposMng.eposList[eposId].GetVelocity();
            break;
        case CURR:  ref_value = GameScreen::eposMng.eposList[eposId].GetCurrent();
            break;
    }

    return false;
}

