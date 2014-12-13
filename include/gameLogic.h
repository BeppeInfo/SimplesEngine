#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#if	defined _WIN32
    #include "C:\Python27\include\Python.h"
#else
	#include </usr/include/python2.7/Python.h>
#endif

static PyObject* getStringValue( PyObject *self, PyObject *args );

static PyObject* setStringValue( PyObject *self, PyObject *args );

static PyObject* getNumberValue( PyObject *self, PyObject *args );

static PyObject* setNumberValue( PyObject *self, PyObject *args );

static PyObject* actorsNumber( PyObject *self, PyObject *args );

static PyObject* elapsedTime( PyObject *self, PyObject *args );

static PyObject* isCharId( PyObject *self, PyObject *args );

static PyObject* objIdCollision( PyObject *self, PyObject *args );

static PyObject* deleteObjId( PyObject *self, PyObject *args );

static PyObject* registerValue( PyObject *self, PyObject *args );

static PyObject* registerObjIdPos( PyObject *self, PyObject *args );

static PyObject* endGame( PyObject *self, PyObject *args );

static PyObject* works( PyObject *self, PyObject *args );

static PyMethodDef gameMethods[] = {
            { "getStringValue", getStringValue, METH_VARARGS, "Obter valor de string exposto." },
            { "setStringValue", setStringValue, METH_VARARGS, "Definir valor de string exposto." },
            { "getNumberValue", getNumberValue, METH_VARARGS, "Obter valor numérico exposto." },
            { "setNumberValue", setNumberValue, METH_VARARGS, "Definir valor numérico exposto." },
            { "actorsNumber", actorsNumber, METH_VARARGS, "Obter número de objetos e personagens ativos." },
            { "elapsedTime", elapsedTime, METH_VARARGS, "Intervalo de tempo (milissegundos) desde a última atualização" },
            { "isCharId", isCharId, METH_VARARGS, "Verfica se objeto é personagem personagem." },
            { "objIdCollision", objIdCollision, METH_VARARGS, "Verfica colisão de objeto de ID fornecido com jogador." },
            { "deleteObjId", deleteObjId, METH_VARARGS, "Deleta objeto de ID fornecido." },
            { "registerValue", registerValue, METH_VARARGS, "Registra valor numérico no sistema de Log." },
            { "registerObjIdPos", registerObjIdPos, METH_VARARGS, "Registra posição do objeto de ID fornecido no Sistema de Log." },
            { "endGame", endGame, METH_VARARGS, "Termina gameplay ( vai para tela de fim de jogo )." },
            { "works", works, METH_VARARGS, "Teste" },
            {NULL, NULL, 0, NULL}        /* Sentinel */
        };

PyMODINIT_FUNC initGameLogicInterface(void)
{
    (void) Py_InitModule( "GameLogicInterface", gameMethods );
}

char fileName[50];
PyObject* pyFile;

void pythonRunningFrom( char* origin )
{
    Py_SetProgramName( origin );
}

void initPythonInterface( const char* initFile, const char* loopFile )
{
    Py_Initialize();

    initGameLogicInterface();

    PyRun_SimpleString( "from GameLogicInterface import getStringValue as getStringValue" );
    PyRun_SimpleString( "from GameLogicInterface import setStringValue as setStringValue" );
    PyRun_SimpleString( "from GameLogicInterface import getNumberValue as getNumberValue" );
    PyRun_SimpleString( "from GameLogicInterface import setNumberValue as setNumberValue" );
    PyRun_SimpleString( "from GameLogicInterface import actorsNumber as actorsNumber" );
    PyRun_SimpleString( "from GameLogicInterface import elapsedTime as elapsedTime" );
    PyRun_SimpleString( "from GameLogicInterface import isCharId as isCharId" );
    PyRun_SimpleString( "from GameLogicInterface import objIdCollision as objIdCollision" );
    PyRun_SimpleString( "from GameLogicInterface import deleteObjId as deleteObjId" );
    PyRun_SimpleString( "from GameLogicInterface import registerValue as registerValue" );
    PyRun_SimpleString( "from GameLogicInterface import registerObjIdPos as registerObjIdPos" );
    PyRun_SimpleString( "from GameLogicInterface import endGame as endGame" );
    PyRun_SimpleString( "from GameLogicInterface import works as works" );

    strcpy( fileName, loopFile );
    pyFile = PyFile_FromString( fileName, "r" );
}

void runPythonLoop()
{
    PyRun_SimpleFile( PyFile_AsFile(pyFile), fileName );
}

void endPythonInterface()
{
    Py_Finalize();
}

#endif //GAMELOGIC_H
