#include "include/Game.h"

SDLMappy* Game::Map = NULL;

LoggerClient& Game::logClient = LoggerClient::Activate( stringValues["server_address"], (uint16_t)numberValues["port_number"] );

vector<Object*> Game::actors;

bool Game::gameEnded = false;

/*********************************************/
/*              PYTHON INTERFACE             */
/*********************************************/

static PyObject* getStringValue( PyObject *self, PyObject *args )
{
    const char *key;

    if ( !PyArg_ParseTuple( args, "s", &key ) )
        return NULL;

    return Py_BuildValue( "s", GameScreen::stringValues[key].c_str() );
}

static PyObject* setStringValue( PyObject *self, PyObject *args )
{
    const char *key;
    const char *value;

    if ( !PyArg_ParseTuple( args, "ss", &key, &value ) )
        return NULL;

    GameScreen::stringValues[key] = value;

    return Py_BuildValue("");
}

static PyObject* getNumberValue( PyObject *self, PyObject *args )
{
    const char *key;

    if ( !PyArg_ParseTuple( args, "s", &key ) )
        return NULL;

    return Py_BuildValue( "i", GameScreen::numberValues[key] );
}

static PyObject* setNumberValue( PyObject *self, PyObject *args )
{
    const char *key;
    int value;

    if ( !PyArg_ParseTuple( args, "si", &key, &value ) )
        return NULL;

    GameScreen::numberValues[key] = value;

    return Py_BuildValue("");
}

static PyObject* actorsNumber( PyObject *self, PyObject *args )
{
    if ( !PyArg_ParseTuple( args, "" ) )
        return NULL;

    return Py_BuildValue( "i", Game::actors.size() );
}

static PyObject* elapsedTime( PyObject *self, PyObject *args )
{
    if ( !PyArg_ParseTuple( args, "" ) )
        return NULL;

    return Py_BuildValue( "i", GameScreen::elapsedTime );
}

static PyObject* getMapSize( PyObject *self, PyObject *args )
{
    if ( !PyArg_ParseTuple( args, "" ) )
        return NULL;

    return Py_BuildValue( "(ii)", Game::Map->GetMapWidth(), Game::Map->GetMapHeight() );
}

//Função auxiliar
bool isInsideRange( int position )
{
    if( position < 0 or position >= (int)Game::actors.size() )
    {
        cerr << "No object of index " << position << endl;
        return false;
    }

    return true;
}

static PyObject* isCharId( PyObject *self, PyObject *args )
{
    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    if( Game::actors[id]->movable )
        return Py_BuildValue( "i", 1 );
    else
        return Py_BuildValue( "i", 0 );
}

static PyObject* getObjIdState( PyObject *self, PyObject *args )
{
    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    return Py_BuildValue( "i", Game::actors[id]->state );
}

static PyObject* setItemId( PyObject *self, PyObject *args )
{
    int id, state;
    double scale = 1.0;
    const char* icon = NULL;

    if ( !PyArg_ParseTuple( args, "ii|sd", &id, &state, &icon, &scale ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    if( icon ) Game::actors[id] = new Object( Game::actors[id]->GetPosition(), ROOTDIR + "data/items/" + icon, scale );

    Game::actors[id]->state = abs(state);

    return Py_BuildValue("");
}

static PyObject* setCharId( PyObject *self, PyObject *args )
{
    int id, state;
    double scale = 1.0;
    const char* config = NULL;
    const char* controls = NULL;

    if ( !PyArg_ParseTuple( args, "ii|sd", &id, &state, &config, &scale, &controls ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    if( config )
    {
        if( controls ) Game::actors[id] = Game::SetChar( Game::actors[id]->GetPosition(), config, scale, controls );
        else Game::actors[id] = Game::SetChar( Game::actors[id]->GetPosition(), config, scale );
    }

    Game::actors[id]->state = abs(state);

    return Py_BuildValue("");
}

static PyObject* objIdCollision( PyObject *self, PyObject *args )
{
    int id1, id2;

    if ( !PyArg_ParseTuple( args, "ii", &id1, &id2 ) )
        return NULL;

    if( !isInsideRange(id1) or !isInsideRange(id2) ) return NULL;

    if( Game::actors[id1]->ObjCollision( Game::actors[id1]->GetPosition(), Game::actors[id2] ) )
        return Py_BuildValue( "i", 1 );
    else
        return Py_BuildValue( "i", 0 );

}

static PyObject* deleteObjId( PyObject *self, PyObject *args )
{
    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    delete Game::actors[id];

    return Py_BuildValue("");
}

static PyObject* createItem( PyObject *self, PyObject *args )
{
    int x, y;
    double scale = 1.0;
    const char* icon = NULL;

    if ( !PyArg_ParseTuple( args, "(ii)|sd", &x, &y, &icon, &scale ) )
        return NULL;

    if( icon ) Game::actors.push_back( new Object( Vec2(x, y), ROOTDIR + "data/items/" + icon, scale ) );
    else Game::actors.push_back( new Object( Vec2(x, y), ROOTDIR + "data/items/" + GameScreen::stringValues["item_icon"], scale ) );

    if( Game::actors.back()->Collision( Game::actors.back()->GetPosition() ) )
    {
        delete Game::actors.back();
        return Py_BuildValue( "i", -1 );
    }
    else
        return Py_BuildValue( "i", (int)Game::actors.size() - 1 );
}

static PyObject* createChar( PyObject *self, PyObject *args )
{
    int x, y;
    double scale = 1.0;
    const char* config = NULL;
    const char* controls = NULL;

    if ( !PyArg_ParseTuple( args, "(ii)|sds", &x, &y, &config, &scale, &controls ) )
        return NULL;

    if( config )
    {
        if( controls ) Game::actors.push_back( Game::SetChar( Vec2(x,y), config, scale, controls ) );
        else Game::actors.push_back( Game::SetChar( Vec2(x,y), config, scale ) );
    }
    else Game::actors.push_back( Game::SetChar( Vec2(x,y), GameScreen::stringValues["character"] ) );

    if( Game::actors.back()->Collision( Game::actors.back()->GetPosition() ) )
    {
        delete Game::actors.back();
        return Py_BuildValue( "i", -1 );
    }
    else
        return Py_BuildValue( "i", (int)Game::actors.size() - 1 );
}

static PyObject* registerValue( PyObject *self, PyObject *args )
{
    int value;

    if ( !PyArg_ParseTuple( args, "i", &value ) )
        return NULL;

    Game::logClient.StoreData( value );

    cout << value << endl;

    return Py_BuildValue("");
}

static PyObject* getObjIdPos( PyObject *self, PyObject *args )
{
    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    return Py_BuildValue( "(ii)", Game::actors[id]->GetXPosition(), Game::actors[id]->GetYPosition() );
}

static PyObject* setObjIdPos( PyObject *self, PyObject *args )
{
    int id, x, y;

    if ( !PyArg_ParseTuple( args, "i(ii)", &id, &x, &y ) )
        return NULL;

    if( !isInsideRange(id) ) return NULL;

    Game::actors[id]->SetPosition( x, y );

    if( Game::actors[id]->Collision( Game::actors[id]->GetPosition() ) )
        return Py_BuildValue( "i", -1 );
    else
        return Py_BuildValue( "i", 0 );
}

static PyObject* endGame( PyObject *self, PyObject *args )
{
    const char *screen;

    if ( !PyArg_ParseTuple( args, "s", &screen ) )
        return NULL;

    GameScreen::nextScreen = screen;

    Game::gameEnded = true;

    return Py_BuildValue("");
}

static PyObject* getEposIdValue( PyObject *self, PyObject *args )
{
    int id, mode;

    if ( !PyArg_ParseTuple( args, "ii", &id, &mode ) )
        return NULL;

    if( mode == POS )
        return Py_BuildValue( "i", GameScreen::eposMng.eposList[id].GetPosition() );
    else if( mode == VEL )
        return Py_BuildValue( "i", GameScreen::eposMng.eposList[id].GetVelocity() );
    else if( mode == CURR )
        return Py_BuildValue( "i", GameScreen::eposMng.eposList[id].GetCurrent() );
    else
        return Py_BuildValue( "i", 0 );
}

static PyObject* setEposIdState( PyObject *self, PyObject *args )
{
    int id, mode;
    int velocity = 0, acceleration = 0, deceleration = 0, enabled = 0;

    if ( !PyArg_ParseTuple( args, "ii|iiii", &id, &mode, &enabled, &velocity, &acceleration, &deceleration ) )
        return NULL;

    GameScreen::eposMng.eposList[id].SetState( (EposOperationMode)mode, velocity, acceleration, deceleration, (bool)enabled );

    return Py_BuildValue("");
}

static PyObject* works( PyObject *self, PyObject *args )
{
    const char* input;
    int value;

    if ( !PyArg_ParseTuple( args, "si", &input, &value ) )
        return NULL;

    cout << input << " " << value << endl;

    return Py_BuildValue("");
}

static PyMethodDef gameMethods[] = {
            { "getStringValue", getStringValue, METH_VARARGS, "Obter valor de string exposto." },
            { "setStringValue", setStringValue, METH_VARARGS, "Definir valor de string exposto." },
            { "getNumberValue", getNumberValue, METH_VARARGS, "Obter valor numérico exposto." },
            { "setNumberValue", setNumberValue, METH_VARARGS, "Definir valor numérico exposto." },
            { "actorsNumber", actorsNumber, METH_VARARGS, "Obter número de objetos e personagens ativos." },
            { "elapsedTime", elapsedTime, METH_VARARGS, "Intervalo de tempo (milissegundos) desde a última atualização" },
            { "getMapSize", getMapSize, METH_VARARGS, "Dimensões (tuple) do mapa" },
            { "isCharId", isCharId, METH_VARARGS, "Verifica se objeto é personagem." },
            { "getObjIdState", getObjIdState, METH_VARARGS, "Obter estado de objeto/personagem de ID fornecido." },
            { "setItemId", setItemId, METH_VARARGS, "Define estado de item de ID fornecido." },
            { "setCharId", setCharId, METH_VARARGS, "Define estado de personagem de ID fornecido." },
            { "objIdCollision", objIdCollision, METH_VARARGS, "Verifica colisão de objeto de ID fornecido com jogador." },
            { "createItem", createItem, METH_VARARGS, "Insere item no mapa na posição fornecida." },
            { "createChar", createChar, METH_VARARGS, "Insere personagem não jogável no mapa na posição fornecida." },
            { "deleteObjId", deleteObjId, METH_VARARGS, "Deleta objeto de ID fornecido." },
            { "registerValue", registerValue, METH_VARARGS, "Registra valor numérico no sistema de Log." },
            { "getObjIdPos", getObjIdPos, METH_VARARGS, "Obter posição (tuple) do objeto de ID fornecido." },
            { "setObjIdPos", setObjIdPos, METH_VARARGS, "Define posição (tuple) do objeto de ID fornecido." },
            { "endGame", endGame, METH_VARARGS, "Termina gameplay ( vai para tela de fim de jogo )." },
            { "getEposIdValue", getEposIdValue, METH_VARARGS, "Obter da Epos de id fornecido valor do tipo especificado." },
            { "setEposIdState", setEposIdState, METH_VARARGS, "Define modo de operação e estado (ativo ou não) da Epos de id fornecido." },
            { "works", works, METH_VARARGS, "Teste" },
            {NULL, NULL, 0, NULL}        /* Sentinel */
        };

PyMODINIT_FUNC initGameLogicInterface(void)
{
    (void) Py_InitModule( "GameLogicInterface", gameMethods );
}

string pythonCommands;
ifstream file;

void pythonRunningFrom( char* origin )
{
    Py_SetProgramName( origin );
}

void initPythonInterface( const char* initFile, const char* loopFile )
{
    Py_Initialize();

    initGameLogicInterface();

    file.open( initFile );
    pythonCommands = ReadFile( file );
    file.close();
    PyRun_SimpleString( pythonCommands.c_str() );
    pythonCommands.clear();

    file.open( loopFile );
    pythonCommands = ReadFile( file );
    file.close();
}

void runPythonLoop()
{
    PyRun_SimpleString( pythonCommands.c_str() );
}

void endPythonInterface()
{
    pythonCommands.clear();
    Py_Finalize();
}

/*********************************************/
/*            PYTHON INTERFACE END           */
/*********************************************/



Game::Game( string imageName, string initFile, string logicFile )
    :GameScreen( imageName )
{
    //ctor
    CreateMap();

    initPythonInterface( initFile.c_str(), logicFile.c_str() );
}

Game::~Game()
{
    //dtor
    logClient.StoreData( END );
    logClient.SendData();

    endPythonInterface();

    for( vector<Object*>::iterator it = actors.begin(); it != actors.end(); ++it )
        delete (*it);
    actors.clear();

    delete Map;
}

void Game::CreateMap()
{
    Map = new SDLMappy();

	if (Map->LoadMap( (ROOTDIR + "data/maps/" + stringValues["map_name"]).c_str(), 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT ) == -1 )
		cout << "Can't load the Map " << stringValues["map_name"] << endl;

    Map->MapInitAnims();

    actors.clear();
}

Character* Game::SetChar( Vec2 position, string charFolder, double scale, string controlsFile )
{
    static int w, h;
    static string spriteFile, data;
    static ifstream charData;

    Character* actor;

    spriteFile = ROOTDIR + "data/chars/" + charFolder + "/sprites.png";

    charData.open( (ROOTDIR + "data/chars/" + charFolder + "/config.txt").c_str() );
    if( !charData ) cout << "File " << charFolder + "/config.txt" << " couldn't be opened" << endl;

    while( charData >> data )
    {
        if( data == "sprite_sheet=" )
        {
            data = GetString( charData );

            spriteFile = ROOTDIR + "data/chars/" + charFolder + "/" + data;
        }
        if( data == "sprite_size=" )
        {
            charData >> w >> h;

            actor = new Character( position, Vec2(w, h), spriteFile, scale );

            actor->Configure( charData );
        }
        else if( data == "#" )
            SkipLine( charData );
    }

    charData.close();

    if( controlsFile != "" )
    {
        controlsFile = ROOTDIR + "data/" + controlsFile;

        charData.open( controlsFile.c_str() );
        if( !charData ) cout << "File " << controlsFile << " couldn't be opened" << endl;

        while( charData >> data )
        {
            if( data == "key_triggers=" )
            {
                int id;
                charData >> id;
                actor->animations[id].keyTriggers = ReadValues( charData );
            }
            else if( data == "#" )
                SkipLine( charData );
        }

        charData.close();
    }

    return actor;
}

bool Game::Update()
{
    if( gameEnded )
    {
        logClient.StoreData( END );

        endPythonInterface();

        gameEnded = false;
        return true;
    }
    else
    {
        runPythonLoop();

        for( vector<Object*>::iterator it = actors.begin(); it != actors.end(); ++it )
            (*it)->Update( elapsedTime );

        logClient.SendData();

        return GameScreen::Update();
    }
}

void Game::MoveMap()
{
    int mapX = actors.front()->GetXPosition() - SCREEN_WIDTH/2;
    int mapY = actors.front()->GetYPosition() - SCREEN_HEIGHT/2;

    Map->MapMoveTo( mapX , mapY );
}

void Game::UpdateMap()
{
    Map->MapChangeLayer(0);
    Map->MapDrawBGT( GameScreen::screen );
    Map->MapChangeLayer(1);
    Map->MapDrawBGT( GameScreen::screen );

    Map->MapUpdateAnims();
}

void Game::Draw()
{
    MoveMap();

    UpdateMap();

    for( int z = 0; z < Map->GetMapHeight(); z++ )
    {
        for( vector<Object*>::iterator it = actors.begin(); it != actors.end(); ++it )
        {
            if( (*it)->GetYPosition() == z ) (*it)->Draw();
        }
    }

    GameScreen::Draw();
}
