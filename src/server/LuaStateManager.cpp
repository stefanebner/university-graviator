#include "LuaStateManager.h"

LuaStateManager* LuaStateManager::instance = 0;

LuaStateManager& LuaStateManager::getInstance()
{
	if ( !instance )
		instance = new LuaStateManager();
	return *instance;

}

void LuaStateManager::init(const char* luaFileName)
{
	fileName = luaFileName;
	test=true;
}

float LuaStateManager::getFloat( const char* variable )
{
	int openFile = mGlobalState->DoFile(fileName);
	if (openFile!=0)
	{
		printf("Could not open ->%s<- with Errorcode %i\n", fileName, openFile);
		return -1;
	}
	else
	{
		LuaPlus::LuaObject luaObject = mGlobalState->GetGlobal(variable);
		float var = luaObject.GetFloat();
		return var;
	}

}

int LuaStateManager::getInt( const char* variable )
{
	int openFile = mGlobalState->DoFile(fileName);
	if (openFile==1)
	{
		printf("Could not open ->%s<- with Errorcode %i accessing %s\n", fileName, openFile, variable);
		return -1;
	}
	else
	{
		LuaObject luaObject = mGlobalState->GetGlobal(variable);
		int var = luaObject.GetInteger();
		return var;
	}

}

bool LuaStateManager::openLuaFile()
{
	int openFile = mGlobalState->DoFile(fileName);
	if (openFile!=0)
	{
		printf("Could not open ->%s<- with Errorcode %i\n", fileName, openFile);
	}
	return true;
}

