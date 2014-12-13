#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    #define rmask  0xff000000
    #define gmask  0x00ff0000
    #define bmask  0x0000ff00
    #define amask  0x000000ff
#else
    #define rmask  0x000000ff
    #define gmask  0x0000ff00
    #define bmask  0x00ff0000
    #define amask  0xff000000
#endif

#include "include/utils.h"

#include "include/pngfuncs.h"

enum{ BEGIN = 1, KEEP, END };

void CreateGraph( stringstream&, SDL_Surface*, string& );

int main( int argc, char *argv[] )
{
    IPaddress serverIP;
    TCPsocket serverSocket;
    vector<TCPsocket> clientSockets;
    map<TCPsocket,string> clientLogs;
    map<TCPsocket,string> logFiles;
    map<TCPsocket,SDL_Surface*> logGraphs;
    SDLNet_SocketSet socketSet = NULL;

    //string fileName;
    ofstream log;
    stringstream dataBuffer;
    int data;

    bool listening = false;

    //if (SDLNet_Init() < 0)		cerr << "SDLNet_Init: " << SDLNet_GetError() << endl;
    if( Init("Server") == false )
    {
        cout << "SDL não pode ser inicializada" << endl;
        return 0;
    }

	if( SDLNet_ResolveHost(&serverIP, NULL, 2000) < 0 )		cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << endl;

	if( (serverSocket = SDLNet_TCP_Open(&serverIP)) )		listening = true;
	else    cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;

	socketSet = SDLNet_AllocSocketSet( clientSockets.capacity() );

	while( listening )
	{
        TCPsocket clientSocket;
		while( (/*TCPsocket*/ clientSocket = SDLNet_TCP_Accept(serverSocket)) )
        {
            IPaddress* remoteIP;
            if( (remoteIP = SDLNet_TCP_GetPeerAddress(clientSocket)) )
            {
                cout << "Host connected: " << ios::hex << SDLNet_Read32(&remoteIP->host)
                << " " << ios::dec << SDLNet_Read16(&remoteIP->port) << endl;
            }
            else
            {
                cerr << "SDLNet_TCP_GetPeerAddress: " << SDLNet_GetError() << endl;
            }

            clientSockets.push_back( clientSocket );

            if( SDLNet_TCP_AddSocket(socketSet, clientSocket) < 0 )
            {
                SDLNet_FreeSocketSet( socketSet );

                socketSet = SDLNet_AllocSocketSet( clientSockets.capacity() );

                for( vector<TCPsocket>::size_type i = 0 ; i < clientSockets.size() ; ++i )
                {
                    if( clientSockets[i] )  SDLNet_TCP_AddSocket( socketSet, clientSockets[i] );
                }
            }
        }   //while socket accept

        if( SDLNet_CheckSockets(socketSet, 0) > 0 )
        {
            for( vector<TCPsocket>::size_type i = 0 ; i < clientSockets.size() ; ++i )
            {
                TCPsocket clientSocket;
                if( (clientSocket = clientSockets[i]) )
                {
                    if( SDLNet_SocketReady(clientSocket) )
                    {
                        char buffer[512] = "";

                        int bytes = SDLNet_TCP_Recv( clientSocket, buffer, 512 );

                        buffer[511] = '\0';
                        //dataBuffer.str( std::string() );
                        //dataBuffer << buffer;
                        dataBuffer.str( buffer );

                        cout << "Socket " << i << "(buffer):  " << buffer << endl;
                        cout << "Socket " << i << "(sstream):  " << dataBuffer.str() << endl;

                        dataBuffer >> data;

                        if( bytes > 0 and data != END )
                        {
                            if( data == BEGIN )
                            {
                                cout << "BEGIN" << endl;

                                //dataBuffer >> fileName;
                                dataBuffer >> logFiles[clientSocket];

                                int width, height;
                                dataBuffer >> width >> height;

                                logFiles[clientSocket] += ".txt";
                                //clientLogs[clientSocket] << fileName << ".txt" << endl;
                                logGraphs[clientSocket] = SDL_CreateRGBSurface( SDL_HWSURFACE, width, height, 32,
                                                                               rmask, gmask, bmask, amask );
                                //SDL_SaveBMP( logGraphs[clientSocket], "testeimagem" );
                            }
                            else if( data == KEEP )
                            {
                                //log.open( clientLogs[clientSocket].c_str(), ios::app );

                                /*while( dataBuffer >> data )
                                    clientLogs[clientSocket] << data << '\t';

                                clientLogs[clientSocket] << endl;*/
                                cout << logFiles[clientSocket] << endl;

                                clientLogs[clientSocket] += dataBuffer.str();

                                //log << buffer;

                                //log.close();
                            }
                        }
                        else
                        {
                            cout << "END" << endl;

                            //dataBuffer.str( clientLogs[clientSocket] );
                            //CreateGraph( dataBuffer, logGraphs[clientSocket], logFiles[clientSocket] );

                            //fileName = logFiles[clientSocket];
                            //fileName += ".txt";

                            log.open( /*fileName*/logFiles[clientSocket].c_str() );
                            log << clientLogs[clientSocket];
                            log.close();

                            SDLNet_TCP_DelSocket(socketSet, clientSocket);
                            SDLNet_TCP_Close(clientSocket);
                            clientSockets[i] = NULL;
                        }

                    }   //if socket ready
                }
            }
        }   //if check socket

	}   //while listening

	//CLEAN ALL !!
	SDLNet_FreeSocketSet( socketSet );
	clientLogs.clear();
	clientSockets.clear();

	SDLNet_TCP_Close(serverSocket);
	SDLNet_Quit();

	return 0;
}

void CreateGraph( stringstream& dataStream, SDL_Surface* graphSurface, string& imageName )
{
    int n, x, y;

    cout << "aqui" << endl;

    while( dataStream >> n )
    {
        if( n == KEEP )
        {
            dataStream >> n;

            for( int i = 0; i < n; i++ )
            {
                dataStream >> x >> y;

                SDL_Color color = { 0, 0, 255 };
                SDLPoint( graphSurface, x, y, color );
            }
        }
    }

    cout << "aqui" << endl;

    char* fileName = (char*) calloc( imageName.size() + 1, sizeof(char) );
    strcpy( fileName, imageName.c_str() );
    png_save_surface( fileName, graphSurface );

    cout << "aqui" << endl;
}
