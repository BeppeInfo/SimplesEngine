#ifndef LOGGERCLIENT_H
#define LOGGERCLIENT_H

#include "include/utils.h"

enum{ BEGIN = 1, KEEP, END };

class LoggerClient
{
    public:
        static LoggerClient& Activate( string serverAddress = "localhost", uint16_t portNumber = 2000 )
        {
            static LoggerClient logClt( serverAddress, portNumber );
            return logClt;
        }

        //LoggerClient(string = "localhost", uint16_t = 2000);
        virtual ~LoggerClient();

        void StoreData(int);
        void SendData();

    private:
        LoggerClient( string, uint16_t );
        LoggerClient(const LoggerClient&);                 // Prevent copy-construction
        LoggerClient& operator=(const LoggerClient&);      // Prevent assignment

        stringstream buffer;
        TCPsocket clientSocket;
        IPaddress clientIP;
        bool sending;
};

#endif // LOGGERCLIENT_H
