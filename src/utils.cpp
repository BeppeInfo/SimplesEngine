#include "include/utils.h"

string GetString( ifstream& ifs )
{
    string s_value;
    char c;
    bool enter = false;

    while( ifs.good() )
    {
        c = ifs.get();
        if( ifs.good() )
        {
            if( c == '\"' ) { if(enter) break; else enter = true; }
            else if(enter and (int)c != 10 ) s_value += c;
        }
    }

    return s_value;
}

string ReadFile( ifstream& ifs )
{
    string s_value;
    char c;

    while (ifs.good())     // loop while extraction from file is possible
    {
        c = ifs.get();       // get character from file
        if( ifs.good() )
          s_value += c;
    }

    return s_value;
}

void SkipLine( ifstream& ifs )
{
    char c;

    while (ifs.good())     // loop while extraction from file is possible
    {
        c = ifs.get();       // get character from file
        if( ifs.good() )
        {
            if( c == '\n' )
                break;
        }
    }

    return;
}

vector<int> ReadValues( ifstream& ifs )
{
    char c;
    string s;

    while (ifs.good())     // loop while extraction from file is possible
    {
        c = ifs.get();       // get character from file
        if( ifs.good() )
        {
            if( c == '\n' )
                break;
            s += c;
        }
    }

    stringstream ss(s);
    vector<int> values;
    int value;

    while( ss >> value )
        values.push_back( value );

    return values;
}

SDL_Surface* Init( const char *title )
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return NULL;

	// SDL_Ttf
    if( TTF_Init() < 0 )
        return NULL;
    //atexit( TTF_Quit );

    //Set the window caption
    SDL_WM_SetCaption( title, NULL );

    if( SDLNet_Init() < 0 )  cerr << "SDLNet_Init: " << SDLNet_GetError() << endl;

    SDL_EnableUNICODE( SDL_ENABLE );

    //Set up the screen
    SDL_Surface* screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );

    //If there was an error in setting up the screen
    /*if( screen == NULL )
    {
        cerr << "Screen:" << SDL_GetError() << endl;
        return false;
    }*/

    //If everything initialized fine
    //return true;
    return screen;
}

void Quit()
{
    SDL_EnableUNICODE( SDL_DISABLE );

    TTF_Quit();
    Mix_CloseAudio();
    SDLNet_Quit();
    SDL_Quit();
}

void ApplySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    if( SDL_BlitSurface( source, clip, destination, &offset ) < 0 )
        cout << SDL_GetError() << endl;
}

vector<string> ListFiles( string dir_name )
{
    vector<string> valid_files;

    DIR* directory = opendir( dir_name.c_str() );
    struct dirent *entry;

    while( (entry = readdir(directory)) )
    {
        //struct stat status;
        //if( stat( entry->d_name, &status ) == 0 ){

        //if( status.st_mode & !S_IFDIR )
        //if( S_ISREG (status.st_mode) )
        //if( IS_NORMAL_FILE )
        if( string(entry->d_name) != "." and string(entry->d_name) != ".." )
            valid_files.push_back(entry->d_name);
        //}
    }

    closedir(directory);

    return valid_files;
}

SDL_Surface *LoadImage( string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF ) );
        }
    }
    else
    {
        cout << "Couldn't open " << filename << " - ";
        cout << "SDL Error: " << SDL_GetError() << endl;
    }

    //Return the optimized surface
    return optimizedImage;
}

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
void SDLPoint( SDL_Surface* p_surface, int x, int y, SDL_Color p_color )
{
    Uint32 col = SDL_MapRGB( p_surface->format, p_color.r, p_color.g, p_color.b );
    static SDL_Rect rec;
    rec.w = 1;
    rec.h = 1;
    rec.x = x;
    rec.y = y;
    SDL_FillRect( p_surface, &rec, col );
}

// ----------------------------------------------------------------
//  Name:           SDLLine
//  Description:    draws a line on a surface
//  Arguments:      p_surface: surface to draw on
//                  x1, y1: first point
//                  x2, y2: second point
//                  p_color: color of the line
//  Return Value:   None
// ----------------------------------------------------------------
void SDLLine( SDL_Surface* p_surface, int x1, int y1, int x2, int y2, SDL_Color p_color )
{
    // lock the surface
    SDL_LockSurface( p_surface );

    // find distances of the line
    int x;
    float y;
    int dx = x2 - x1;
    int dy = y2 - y1;
    float m;

    // if the line is more wide than tall...
    if( abs(dx) > abs(dy) )
    {
        // check to see if dx is negative. If so,
        // then swap the coordinates.
        if( dx < 0 )
        {
            dx = -dx;
            dy = -dy;
            x = x1;
            x1 = x2;
            x2 = x;
            x = y1;
            y1 = y2;
            y2 = x;
        }

        // find the slope and the beginning y coordinate
        m = (float)dy / (float)dx;
        y = (float)y1;

        // draw a point for each x
        for( x = x1; x <= x2; x++ )
        {
            SDLPoint( p_surface, x, (int)y, p_color );
            y += m;
        }
    }
    else
    {
        // check to see if dy is negative. If so,
        // then swap the coordinates.
        if( dy < 0 )
        {
            dx = -dx;
            dy = -dy;
            x = x1;
            x1 = x2;
            x2 = x;
            x = y1;
            y1 = y2;
            y2 = x;
        }


        // find the slope and the beginning x coordinate
        m = (float)dx / (float)dy;
        y = (float)x1;


        // draw a point for each y
        for( x = y1; x <= y2; x++ )
        {
            SDLPoint( p_surface, (int)y, x, p_color );
            y += m;
        }
    }

    // unlock the surface
    SDL_UnlockSurface( p_surface );
}
