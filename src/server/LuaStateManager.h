//*********************************************************
//	Graviator - QPT2a - FH Salzburg
//	Stefan Ebner - Malte Langkabel - Christian Winkler
//
//	Lua
//	Implementation as a Singleton, allows access to the
//	Lua file from everywhere
//
//*********************************************************

#ifndef LUA_STATE_MANAGER_H
#define LUA_STATE_MANAGER_H

#include "LuaPlus/LuaPlus.h"
#include <string>
#include <iostream>


using namespace LuaPlus;
using namespace std;

class LuaStateManager
{
protected:
	LuaStateManager():
		 mGlobalState(true)
		 {
		 }
		 ~LuaStateManager() {LuaState::Destroy(mGlobalState);}
		 LuaStateManager(const LuaStateManager&) {}

		 static LuaStateManager* instance;

public:
	static LuaStateManager& getInstance();
	void init(const char* luaFileName);
	float getFloat(const char* variable);
	int getInt(const char* variable);
	
private:
	bool openLuaFile();
	LuaStateOwner mGlobalState;
	LuaObject mMetaTable;
	const char* fileName;
	bool test;
};

#endif