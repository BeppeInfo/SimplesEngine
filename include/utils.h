
// ----------------------------------------------------------------
// File: utils.h
// Author:
// Date Created:
// Description:
// Copyright:
// ----------------------------------------------------------------

//The headers
#ifndef UTILS_H
#define UTILS_H

#if	defined _WIN32
	#include "SDL\SDL.h"
	#include "SDL\SDL_image.h"
	#include "SDL\SDL_ttf.h"
	#include "SDL\SDL_mixer.h"
	#include "SDL\SDL_rotozoom.h"
	#include "SDL\SDL_net.h"
	#include <windows.h>
    #define IS_NORMAL_FILE (GetFileAttributes(entry->d_name) != FILE_ATTRIBUTE_DIRECTORY)
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_mixer.h>
	#include <SDL/SDL_rotozoom.h>
	#include <SDL/SDL_net.h>
	#define IS_NORMAL_FILE (entry->d_type != DT_DIR )
#endif

#define DEBUG cout << "funfa !" << endl;
#define ROOTDIR string("../")

#define SCREEN_WIDTH	800     // Screen Width
#define SCREEN_HEIGHT	600     // Screen Height

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <bitset>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <dirent.h>

#include <sys/stat.h>   // For stat().

using namespace std;

#include "include/SDLMappy.h"

#include "include/MathUtils.h"

string GetString( ifstream& );

string ReadFile( ifstream& );

void SkipLine( ifstream& );

vector<int> ReadValues( ifstream& );

vector<string> ListFiles( string );

SDL_Surface* Init( const char* = "SDL Application" );

void Quit();

void ApplySurface( int, int, SDL_Surface*, SDL_Surface*, SDL_Rect* = NULL );

SDL_Surface *LoadImage( string filename );


/*void draw_text(TTF_Font* font, int x, int y, SDL_Color color, const char *format,...){
    SDL_Surface *aux;
    SDL_Rect ret={x,y,0,0};

    // Imprimindo para vetor
    char buf[256];
    va_list args;
    va_start (args, format);
    vsprintf (buf, format, args);
    va_end(args);

    aux = TTF_RenderText_Blended(font, buf, color);
    SDL_BlitSurface(aux, 0, screen, &ret);
    SDL_FreeSurface(aux);
}*/

// ----------------------------------------------------------------
//  Name:           SDLPoint
//  Description:    draws a point on a surface
//  Arguments:      p_surface: surface to draw on
//                  x, y: coordinates
//                  p_color: the color of the point
//  Return Value:   None
// ----------------------------------------------------------------
void SDLPoint( SDL_Surface* p_surface, int x, int y, SDL_Color p_color );


// ----------------------------------------------------------------
//  Name:           SDLLine
//  Description:    draws a line on a surface
//  Arguments:      p_surface: surface to draw on
//                  x1, y1: first point
//                  x2, y2: second point
//                  p_color: color of the line
//  Return Value:   None
// ----------------------------------------------------------------
void SDLLine( SDL_Surface* p_surface, int x1, int y1, int x2, int y2, SDL_Color p_color );


#endif //UTILS_H
