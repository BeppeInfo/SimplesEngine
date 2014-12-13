#include "include/LoggerClient.h"

#include "include/Game.h"

LoggerClient::LoggerClient(string host, uint16_t port)
{
    //ctor
    buffer.str( std::string() );
    sending = false;

	if( SDLNet_ResolveHost(&clientIP, host.c_str(), port) < 0 )   fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());

	if( ( clientSocket = SDLNet_TCP_Open(&clientIP) ) )   sending = true;
    else    fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());

    if( sending )
    {
        buffer << BEGIN << " " << GameScreen::stringValues["player_name"] << "_" << GameScreen::numberValues["total_game_time"] << "_"
            << GameScreen::numberValues["num_enemies"] << "_" << GameScreen::numberValues["num_items"] << "_" << GameScreen::numberValues["game_speed"]
            << '\t' << Game::Map->GetMapWidth() << '\t' << Game::Map->GetMapHeight();

        SendData();
    }

    cout << host << " " << clientIP.host << endl;
}

LoggerClient::~LoggerClient()
{
    //dtor
    //StoreData( END );
    //SendData();

    SDLNet_TCP_Close( clientSocket );
	//SDLNet_Quit();
}

void LoggerClient::StoreData(int data)
{
    if( sending ) { buffer << data << '\t'; }
}

void LoggerClient::SendData()
{
    if( !buffer.str().empty() and sending )
    {
        buffer << endl;

        const char* message = buffer.str().c_str();
        int size = buffer.str().size();

        if( SDLNet_TCP_Send( clientSocket, (void*)message, size ) < size )
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());

        buffer.str( std::string() );
    }
}
