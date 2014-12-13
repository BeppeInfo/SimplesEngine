#ifndef TEXTMANAGER_H
#define TEXTMANAGER_H

#include "include/utils.h"

#if TTF_PATCHLEVEL >= 10
    #define OUTLINE_ON ( TTF_SetFontOutline( fontCache[size], outline ) )
    #define OUTLINE_OFF ( TTF_SetFontOutline( fontCache[size], 0 ) )
#else
    #define OUTLINE_ON
    #define OUTLINE_OFF
#endif

class TextManager
{
    public:

        static TextManager& Activate()
        {
            static TextManager txtmng;
            return txtmng;
        }

        virtual ~TextManager();

        void WriteText( string, int, int, uint8_t = 255, uint8_t = 255, uint8_t = 255, int = 20, int = 0, SDL_Surface* = NULL );
        void WriteTextCentered( string, int, int, int, int, uint8_t = 255, uint8_t = 255, uint8_t = 255, int = 20, int = 0, SDL_Surface* = NULL);
        //void WriteTextCentered( InputBox*, uint8_t, uint8_t, uint8_t, SDL_Surface* );

    private:
        TextManager();
        TextManager(const TextManager&);                 // Prevent copy-construction
        TextManager& operator=(const TextManager&);      // Prevent assignment

        void CreateTextSurface( string, uint8_t, uint8_t, uint8_t, int, int = 0 );

        map<int,TTF_Font*> fontCache;
        SDL_Surface* textSurface;
        SDL_Rect textBox;
};

#endif // TEXTMANAGER_H
