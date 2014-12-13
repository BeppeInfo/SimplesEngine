#include "include/TextManager.h"

#include "include/Game.h"

TextManager::TextManager()
{
    //ctor
    textSurface = NULL;
}

TextManager::~TextManager()
{
    //dtor
    SDL_FreeSurface(textSurface);
    fontCache.clear();
}

void TextManager::CreateTextSurface( string text, uint8_t r, uint8_t g, uint8_t b, int size, int outline )
{
    SDL_FreeSurface(textSurface);

    if( fontCache.find(size) == fontCache.end() )
    {
        fontCache[size] = TTF_OpenFont( (ROOTDIR + "data/fonts/" + GameScreen::stringValues["font_name"]).c_str(), size );
        cout << TTF_GetError() << endl;;
    }

    SDL_Color text_color = {r, g, b};

    OUTLINE_ON;//TTF_SetFontOutline( fontCache[size], outline );

    if( text.empty() ) textSurface = TTF_RenderText_Blended( fontCache[size], " ", text_color );
    else textSurface = TTF_RenderText_Blended( fontCache[size], text.c_str(), text_color );

    OUTLINE_OFF;//TTF_SetFontOutline( fontCache[size], 0 );

    if( !textSurface ) cout << TTF_GetError() << endl;

    //textBox = {0, 0, textSurface->w, textSurface->h};
    textBox.w = textSurface->w;
    textBox.h = textSurface->h;
}

void TextManager::WriteText( string text, int x, int y, uint8_t r, uint8_t g, uint8_t b, int size, int outline, SDL_Surface* destination )
{
    CreateTextSurface( text, r, g, b, size, outline );

    ApplySurface( x, y, textSurface, ( destination ? destination : GameScreen::screen ), &textBox );
}

void TextManager::WriteTextCentered( string text, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b, int size, int outline, SDL_Surface* destination )
{
    CreateTextSurface( text, r, g, b, size, outline );

    int X = x + (w - textSurface->w)/2;
    int Y = y + (h - textSurface->h)/2;

    ApplySurface( X, Y, textSurface, ( destination ? destination : GameScreen::screen ), &textBox );
}

/*void TextManager::WriteTextCentered( InputBox* box, uint8_t r, uint8_t g, uint8_t b, SDL_Surface* destination )
{
    CreateTextSurface( box->BoxText(), r, g, b, box->h/2 );

    int X = box->x + ( box->w - textSurface->w )/2;
    int Y = box->y + ( box->h - textSurface->h )/2;

    ApplySurface( X, Y, textSurface, destination, &textBox );
}*/
