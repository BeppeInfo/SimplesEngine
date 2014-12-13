#ifndef EPOSMANAGER_H
#define EPOSMANAGER_H

#include "include/utils.h"

#if	defined _WIN32
	#include "include/EposCmdWindows.h"
#else
	#include "include/EposCmdUnix.h"
	#include "include/WinTypes.h"
	#include "include/ftd2xx.h"
#endif


enum EposOperationMode { STEP_DIRECTION_MODE = -6,   //Motor de passo
                        MASTER_ENCODER_MODE = -5,    //Usa posição de referência
                        CURRENT_MODE = -3,           //Modo de corrente
                        VELOCITY_MODE = -2,          //Modo de velocidade
                        POSITION_MODE = -1,          //Modo de posição
                        PROFILE_POSITION_MODE = 1,   //Modo de posicionamento com perfil definido
                        PROFILE_VELOCITY_MODE = 3,   //Modo de velocidade com perfil definido
                        HOMING_MODE = 6 };           //Modo de partida inicial

class Epos
{
    public:
        Epos( HANDLE, WORD, EposOperationMode, long, long, long, bool );
        Epos( HANDLE, WORD, EposOperationMode );
        virtual ~Epos();

        EposOperationMode operationMode;

        void SetState( EposOperationMode, long = 0, long = 0, long = 0, bool = false );

        long GetPosition();
        void SetPosition( long );

        long GetVelocity();
        void SetVelocity( long );

        short GetCurrent();
        void SetCurrent( short );

        WORD GetAnalog( WORD );
        void GetAllDigitalInputs();
        WORD GetDigitalInput( int );
        void GetAllDigitalOutputs();
        void SetDigitalOutputs( int, bool );

        void Disable();

        WORD nodeId;

        bool absolute;
        bool immediately;

        WORD digitalInputs;
        WORD digitalOutputs;
        string sDigitalInputs;
        string sDigitalOutputs;

        long maxErrorPosition;
        long maxErrorVelocity;
        short maxErrorCurrent;

    private:

        //EposOperationMode operationMode;
        long position;
        long velocity;
        short current;
        DWORD baudRate;
        DWORD timeOut;
        HANDLE keyHandle;
        DWORD errorCode;
        bool enableState;
        long acceleration;
        long deceleration;
        static const int MAX_ANALOG;
        WORD* analog;
        static const int MAX_OUTPUTS;
        WORD* outputs;
};

class EposManager
{
    public:
        static EposManager& Activate()
        {
            static EposManager epmng;
            return epmng;
        }

        virtual ~EposManager();

        inline HANDLE GetKeyHandle() { return this->keyHandle; }

        vector<Epos> eposList;

    private:

        EposManager();
        EposManager( const EposManager& );                 // Prevent copy-construction
        EposManager& operator=( const EposManager& );      // Prevent assignment

        bool OpenDevice();

        string deviceName;
        string protocolStackName;
        string interfaceName;
        string portName;
        WORD nodeID;
        HANDLE keyHandle;
        DWORD errorCode;
        DWORD baudRate;
        DWORD timeOut;

        int8_t mode;

        static const int MAX_NODES;
};

#endif // EPOSMANAGER_H
