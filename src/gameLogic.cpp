#include "include/gameLogic.h"

#include "include/Game.h"

static PyObject* getStringValue( PyObject *self, PyObject *args )
{
    const char *key;

    if ( !PyArg_ParseTuple( args, "s", &key ) )
        return NULL;

    return Py_BuildValue( "s", Game::stringValues[key].c_str() );
}

static PyObject* setStringValue( PyObject *self, PyObject *args )
{
    const char *key;
    const char *value;

    if ( !PyArg_ParseTuple( args, "ss", &key, &value ) )
        return NULL;

    Game::stringValues[key] = value;

    Py_RETURN_NONE;
}

static PyObject* getNumberValue( PyObject *self, PyObject *args )
{
    const char *key;

    if ( !PyArg_ParseTuple( args, "s", &key ) )
        return NULL;

    return Py_BuildValue( "i", Game::numberValues[key] );
}

static PyObject* setNumberValue( PyObject *self, PyObject *args )
{
    const char *key;
    int value;

    if ( !PyArg_ParseTuple( args, "si", &key, &value ) )
        return NULL;

    Game::numberValues[key] = value;

    Py_RETURN_NONE;
}

static PyObject* actorsNumber( PyObject *self, PyObject *args )
{
    return Py_BuildValue( "i", Game::actors.size() );
}

static PyObject* elapsedTime( PyObject *self, PyObject *args )
{
    return Py_BuildValue( "i", Game::elapsedTime );
}

static PyObject* isCharId( PyObject *self, PyObject *args )
{
    cout << "isCharId" << endl;

    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    if( Game::actors[id]->Update( GameScreen::elapsedTime ) )
        return Py_BuildValue( "i", 1 );
    else
        return Py_BuildValue( "i", 0 );
}

static PyObject* objIdCollision( PyObject *self, PyObject *args )
{
    cout << "objCollision" << endl;

    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    if( Game::world->arbiters.erase( ArbiterKey( Game::actors.front()->body, Game::actors[id]->body ) ) )
        return Py_BuildValue( "i", 1 );
    else
        return Py_BuildValue( "i", 0 );
}

static PyObject* deleteObjId( PyObject *self, PyObject *args )
{
    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    delete Game::actors[id];

    Py_RETURN_NONE;
}

static PyObject* registerValue( PyObject *self, PyObject *args )
{
    int value;

    if ( !PyArg_ParseTuple( args, "i", &value ) )
        return NULL;

    Game::logClient.StoreData( value );

    Py_RETURN_NONE;
}

static PyObject* registerObjIdPos( PyObject *self, PyObject *args )
{
    int id;

    if ( !PyArg_ParseTuple( args, "i", &id ) )
        return NULL;

    Game::logClient.StoreData( (int)( Game::actors[id]->body->position.x ) );
    Game::logClient.StoreData( (int)( Game::actors[id]->body->position.y ) );

    Py_RETURN_NONE;
}

static PyObject* endGame( PyObject *self, PyObject *args )
{
    Game::gameEnded = true;

    Py_RETURN_NONE;
}

static PyObject* works( PyObject *self, PyObject *args )
{
    const char* input;
    int value;

    if ( !PyArg_ParseTuple( args, "si", &input, &value ) )
        return NULL;

    cout << input << " " << value << endl;

    Py_RETURN_NONE;
}
