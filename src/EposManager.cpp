#include "include/EposManager.h"

const int Epos::MAX_ANALOG = 2;
const int Epos::MAX_OUTPUTS = 4;

Epos::Epos( HANDLE keyHandle, WORD nodeId, EposOperationMode operationMode, long velocity, long acceleration, long deceleration, bool enableState )
{
    analog = new WORD[Epos::MAX_ANALOG];
    outputs = new WORD[Epos::MAX_OUTPUTS];
    outputs[0] = outputs[1] = outputs[2] = outputs[3] = 0;

    this->maxErrorCurrent = 3000;            // mA
    this->maxErrorPosition = 2147483640;     // qc
    this->maxErrorVelocity = 3000;           // rpm
    this->baudRate = 115200;
    this->timeOut = 100;

    this->keyHandle = keyHandle;
    this->nodeId = nodeId;
    this->operationMode = operationMode;
    this->enableState = enableState;
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->deceleration = deceleration;

    absolute = true;
    immediately = true;

    VCS_ClearFault( keyHandle, nodeId, &errorCode);
    VCS_SetOperationMode( keyHandle, nodeId, (int8_t)operationMode, &errorCode);

    VCS_GetPositionIs( keyHandle, nodeId, &position, &errorCode );
    VCS_GetVelocityIs( keyHandle, nodeId, &velocity, &errorCode );
    VCS_GetCurrentIs( keyHandle, nodeId, &current, &errorCode );

    if( this->enableState )
        VCS_SetEnableState( keyHandle, nodeId, &errorCode );
    else
        VCS_SetDisableState( keyHandle, nodeId, &errorCode );


    SetState( operationMode, velocity, acceleration, deceleration, enableState );

    GetAllDigitalInputs();
    GetAllDigitalOutputs();
}

Epos::Epos( HANDLE keyHandle, WORD nodeId, EposOperationMode operationMode )
{
    analog = new WORD[Epos::MAX_ANALOG];
    outputs = new WORD[Epos::MAX_OUTPUTS];
    outputs[0] = outputs[1] = outputs[2] = outputs[3] = 0;

    this->maxErrorCurrent = 3000;            // mA
    this->maxErrorPosition = 2147483640;     // qc
    this->maxErrorVelocity = 3000;           // rpm
    this->baudRate = 115200;
    this->timeOut = 100;

    this->keyHandle = keyHandle;
    this->nodeId = nodeId;
    this->operationMode = operationMode;

    absolute = true;
    immediately = true;

    VCS_GetPositionIs( keyHandle, nodeId, &position, &errorCode );
    VCS_GetVelocityIs( keyHandle, nodeId, &velocity, &errorCode );
    VCS_GetCurrentIs( keyHandle, nodeId, &current, &errorCode );

    GetAllDigitalInputs();
    GetAllDigitalOutputs();
}

Epos::~Epos()
{

}

void Epos::SetState( EposOperationMode operationMode, long velocity, long acceleration, long deceleration, bool enableState )
{
    if( enableState )
    {
        switch( operationMode )
        {
            case PROFILE_POSITION_MODE:
                VCS_SetPositionProfile( keyHandle, nodeId, velocity, acceleration, deceleration, &errorCode );
                break;

            case PROFILE_VELOCITY_MODE:
                VCS_SetVelocityProfile( keyHandle, nodeId, acceleration, deceleration, &errorCode );
                break;

            case CURRENT_MODE:
                VCS_SetCurrentMust( keyHandle, nodeId, current, &errorCode );
                break;

            default:
                break;
        }
    }
}

long Epos::GetPosition()
{
    switch( operationMode )
    {
        case POSITION_MODE:
            VCS_GetPositionIs( keyHandle, nodeId, &position, &errorCode );
            break;
        case PROFILE_POSITION_MODE:
            VCS_GetPositionProfile( keyHandle, nodeId, (DWORD*)&velocity, (DWORD*)&acceleration, (DWORD*)&deceleration, &errorCode);
            break;
        case CURRENT_MODE:
        case HOMING_MODE:
        case MASTER_ENCODER_MODE:
        case STEP_DIRECTION_MODE:
        case PROFILE_VELOCITY_MODE:
        case VELOCITY_MODE:
            VCS_GetPositionIs( keyHandle, nodeId, &position, &errorCode );
            break;
        }

        return this->position;
}

void Epos::SetPosition( long position )
{
    if( enableState )
    {
        if( operationMode == POSITION_MODE )
        {
            if( abs( position - GetPosition() ) >= maxErrorPosition )
            {
                cout << "Erro: Variação de posição acima do limite permitido" << endl;
                cout << "Solução: Verificar Epos user interface" << endl;
            }
            else
            {
                this->position = position;
            }
    }
    else
        this->position = position;

        switch( operationMode )
        {
            case POSITION_MODE:
                VCS_SetPositionMust( keyHandle, nodeId, position, &errorCode );
                break;
            case PROFILE_POSITION_MODE:
                VCS_MoveToPosition( keyHandle, nodeId, position, absolute, immediately, &errorCode );
                break;
            default:
                break;
        }
    }
}

long Epos::GetVelocity()
{
    switch( operationMode )
    {
        case VELOCITY_MODE:
            VCS_GetVelocityIs( keyHandle, nodeId, &velocity, &errorCode );
            break;
        case PROFILE_VELOCITY_MODE:
            VCS_GetVelocityProfile( keyHandle, nodeId, (DWORD*)&acceleration, (DWORD*)&deceleration, &errorCode);
            break;
        case CURRENT_MODE:
        case HOMING_MODE:
        case MASTER_ENCODER_MODE:
        case STEP_DIRECTION_MODE:
        case PROFILE_POSITION_MODE:
        case POSITION_MODE:
            VCS_GetVelocityIs( keyHandle, nodeId, &velocity, &errorCode );
            break;
    }

    return this->velocity;
}

void Epos::SetVelocity( long velocity )
{
    if( enableState )
    {
        if( operationMode == VELOCITY_MODE )
        {
            if( abs( velocity - GetVelocity() ) >= maxErrorVelocity )
            {
                cout << "Erro: Variação de velocidade acima do limite permitido" << endl;
                cout << "Solução: Verificar Epos user interface" << endl;
            }
            else
            {
                this->velocity = velocity;
            }
        }
        else
            this->velocity = velocity;

        switch( operationMode )
        {
            case VELOCITY_MODE:
                VCS_SetVelocityMust( keyHandle, nodeId, velocity, &errorCode );
                break;
            case PROFILE_VELOCITY_MODE:
                VCS_MoveWithVelocity( keyHandle, nodeId, velocity, &errorCode );
                break;
            default:
                break;
        }
    }
}

short Epos::GetCurrent()
{
    VCS_GetCurrentIs( keyHandle, nodeId, &current, &errorCode );
    return this->current;
}

void Epos::SetCurrent( short current )
{
    if( enableState )
    {
        if( current > maxErrorCurrent )
        {
            cout << "Erro: Variação de corrente acima do limite permitido" << endl;
            cout << "Solução: Verificar Epos user interface" << endl;
        }
        else
            this->current = current;

        if( operationMode == CURRENT_MODE )
            VCS_SetCurrentMust( keyHandle, nodeId, current, &errorCode );
    }
}

WORD Epos::GetAnalog( WORD number )
{
    VCS_GetAnalogInput( keyHandle, nodeId, number, &analog[number - 1], &errorCode );
    return analog[number - 1];
}

void Epos::GetAllDigitalInputs()
{
    VCS_GetAllDigitalInputs( keyHandle, nodeId, &digitalInputs, &errorCode );
    this->sDigitalInputs = ( bitset<16>(digitalInputs) ).to_string< char,char_traits<char>,allocator<char> >();
}

WORD Epos::GetDigitalInput( int number )
{
    switch( number )
    {
        case 1:
            number = 0;
            break;
        case 2:
            number = 1;
            break;
        case 3:
            number = 2;
            break;
        case 4:
            number = 3;
            break;
        case 5:
            number = 14;
            break;
        case 6:
            number = 15;
            break;
    }

    stringstream ss( sDigitalInputs.substr(number, 1) );
    WORD input;
    ss >> input;

    return input;
}

void Epos::GetAllDigitalOutputs()
{
    VCS_GetAllDigitalOutputs( keyHandle, nodeId, &digitalOutputs, &errorCode );
    sDigitalOutputs = ( bitset<16>(digitalOutputs) ).to_string< char,char_traits<char>,allocator<char> >();
}

void Epos::SetDigitalOutputs( int numberOutput, bool on )
{
    if( on )
        outputs[numberOutput-1] = 1;
    else
        outputs[numberOutput-1] = 0;

    stringstream ss("");
    ss << outputs[0] << outputs[1] << outputs[2] << outputs[3] << "00000000000";

    sDigitalOutputs = ss.str();

    sDigitalOutputs += '1';
    digitalOutputs = (WORD)( ( bitset<16>(sDigitalOutputs) ).to_ulong() );
    VCS_SetAllDigitalOutputs( keyHandle, nodeId, digitalOutputs, &errorCode);
}

void Epos::Disable()
{
    VCS_SetDisableState( keyHandle, nodeId, &errorCode );
    VCS_SetAllDigitalOutputs( keyHandle, nodeId, 1, &errorCode );
}



const int EposManager::MAX_NODES = 128;

EposManager::EposManager()
{
    deviceName = "EPOS";
    protocolStackName = "MAXON_RS232";
    interfaceName = "RS232";
    portName = "COM1";
    baudRate = 115200;
    timeOut = 100;

    string data = ROOTDIR + "data/EposConfig.txt";

    ifstream configFile( data.c_str() );

    while( configFile >> data )
    {
        if( data == "EPOS:" )
        {
            while( configFile >> data and data != ":EPOS" )
            {
                if( data == "device_name=" )
                    deviceName = GetString( configFile );
                else if( data == "protocol_stack_name=" )
                    protocolStackName = GetString( configFile );
                else if( data == "interface_name=" )
                    interfaceName = GetString( configFile );
                else if( data == "port_name=" )
                    portName = GetString( configFile );
                else if( data == "operation_mode=" )
                    configFile >> mode;
                else if( data == "node_ID=" )
                    configFile >> nodeID;
                else if( data == "baud_rate=" )
                    configFile >> baudRate;
                else if( data == "time_out=" )
                    configFile >> timeOut;
                else if( data == "#" )
                    SkipLine( configFile );
            }

            OpenDevice();
            eposList.push_back( Epos(keyHandle, nodeID, (EposOperationMode)mode, 0, 0, 0, false) );
        }
    }

    configFile.close();
}

EposManager::~EposManager()
{
    //dtor
}

bool EposManager::OpenDevice()
{
    keyHandle = VCS_OpenDevice( (char*)deviceName.c_str(), (char*)protocolStackName.c_str(), (char*)interfaceName.c_str(), (char*)portName.c_str(), &errorCode );
    VCS_SetProtocolStackSettings( keyHandle, baudRate, timeOut, &errorCode );
    return true;
}

