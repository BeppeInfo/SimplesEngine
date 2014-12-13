#include <iostream>
#include <cstdlib>

#include "EposCmdWindows.h"

using namespace std;

/*typedef (void*)			HANDLE;
typedef int				BOOL;
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef float			FLOAT;*/

int main()
{
    HANDLE keyHandle;
    WORD nodeId = 1;
    long pPositionIs;
    DWORD pErrorCode;

    keyHandle = VCS_OpenDevice( "EPOS", "MAXON_RS232", "RS232", "COM1", &pErrorCode );
    VCS_SetProtocolStackSettings( keyHandle, 115200, 100, &pErrorCode );

    VCS_ClearFault( keyHandle, nodeId, &pErrorCode );
    VCS_SetOperationMode( keyHandle, nodeId, -1, &pErrorCode );

    bool running = true;

    while( running )
    {
        VCS_GetPositionIs( keyHandle, nodeId, &pPositionIs, &pErrorCode );
        cout << pPositionIs << endl;
        system( "cls" );
    }

    //MotionInfo_DllExport BOOL __stdcall VCS_GetPositionIs(HANDLE KeyHandle, WORD NodeId, long* pPositionIs, DWORD* pErrorCode);
    //Initialisation_DllExport HANDLE __stdcall VCS_OpenDevice(char* DeviceName, char* ProtocolStackName, char* InterfaceName, char* PortName, DWORD* pErrorCode);
    //Initialisation_DllExport BOOL __stdcall VCS_SetProtocolStackSettings(HANDLE KeyHandle, DWORD Baudrate, DWORD Timeout, DWORD* pErrorCode);

    return 0;
}
