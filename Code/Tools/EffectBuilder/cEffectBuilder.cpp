// Header Files
//=============

#include "cEffectBuilder.h"

#include <cstdio>
#include <cassert>
#include <cstring>
#include <sstream>
#include "../../Engine/Windows/WindowsFunctions.h"
// Interface
//==========

// Build
//------

//Helper Functions
bool LoadTableWithKey(lua_State& io_luaState, const char * key);
bool LoadTableWithIndex(lua_State& io_luaState, const int index);
bool LoadValueWithIndex(lua_State& io_luaState, const int index);



bool eae6320::cEffectBuilder::Build(const std::vector<std::string>&)
{
	bool wereThereErrors = false;

	// Create a new Lua state
	lua_State* luaState = NULL;
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage("Failed to create a new Lua state", __FILE__);
			goto OnExit;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	{
		const int luaResult = luaL_loadfile(luaState, m_path_source);
		if (luaResult != LUA_OK)
		{
			wereThereErrors = true;
			eae6320::OutputErrorMessage(lua_tostring(luaState, -1), __FILE__);
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		const int argumentCount = 0;
		const int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		const int noMessageHandler = 0;
		const int luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const int returnedValueCount = lua_gettop(luaState);
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				if (!lua_istable(luaState, -1))
				{
					wereThereErrors = true;
					std::stringstream errorMessage;
					errorMessage << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")\n";
					eae6320::OutputErrorMessage(errorMessage.str().c_str(),__FILE__);
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				std::stringstream errorMessage;
				errorMessage << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)"
					"\n";
				eae6320::OutputErrorMessage(errorMessage.str().c_str(),__FILE__);
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			std::stringstream errorMessage;
			errorMessage << lua_tostring(luaState, -1);
			eae6320::OutputErrorMessage(errorMessage.str().c_str(),__FILE__);
			// Pop the error message
			lua_pop(luaState, 1);
			goto OnExit;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	/*if (!ProcessEffectData(*luaState))
	{
		wereThereErrors = true;
	}*/
	FILE * oFile;
	fopen_s(&oFile, m_path_target, "wb");
	if (oFile != NULL)
	{

		lua_pushstring(luaState, "vertex");
		lua_gettable(luaState, -2);
		const char * vertexShader = lua_tostring(luaState, -1);
		fwrite(vertexShader, strlen(vertexShader) + 1, 1, oFile);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "fragment");
		lua_gettable(luaState, -2);
		const char * fragmentShader = lua_tostring(luaState, -1);
		fwrite(fragmentShader, strlen(fragmentShader) + 1, 1, oFile);
		lua_pop(luaState, 1);
		//MessageBox(NULL, "", NULL, MB_OK);
		uint8_t renderStates = 0;
		//int test;

		lua_pushstring(luaState, "alphaTransparency");
		lua_gettable(luaState, -2);
		if(lua_toboolean(luaState, -1))
			renderStates |= RenderStates::ALPHA_TRANSPARENCY;
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "depthTesting");
		lua_gettable(luaState, -2);
		if (lua_toboolean(luaState, -1))
			renderStates |= RenderStates::DEPTH_TESTING;
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "depthWriting");
		lua_gettable(luaState, -2);
		if (lua_toboolean(luaState, -1))
			renderStates |= RenderStates::DEPTH_WRITING;
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "faceCulling");
		lua_gettable(luaState, -2);
		if (lua_toboolean(luaState, -1))
			renderStates |= RenderStates::FACE_CULLING;
		lua_pop(luaState, 1);
		
		fwrite(&renderStates, sizeof(renderStates), 1, oFile);

		fclose(oFile);
	}
	// Pop the table
	lua_pop(luaState, 1);
OnExit:

	if (luaState)
	{
		// If I haven't made any mistakes
		// there shouldn't be anything on the stack,
		// regardless of any errors encountered while loading the file:
		assert(lua_gettop(luaState) == 0);

		lua_close(luaState);
		luaState = NULL;
	}
	return !wereThereErrors;
}



bool LoadTableWithKey(lua_State& io_luaState, const char * key)
{
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	if (!lua_istable(&io_luaState, -1))
	{
		std::stringstream errorMessage;
		errorMessage << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
		//Pop the faulty entry
		lua_pop(&io_luaState, 1);
		return false;
	}
	return true;
}
bool LoadTableWithIndex(lua_State& io_luaState, const int index)
{
	lua_pushinteger(&io_luaState, index);
	lua_gettable(&io_luaState, -2);
	if (!lua_istable(&io_luaState, -1))
	{
		std::stringstream errorMessage;
		errorMessage << "The value at \"" << index << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")\n";
		eae6320::OutputErrorMessage(errorMessage.str().c_str(), __FILE__);
		//Pop the faulty entry
		lua_pop(&io_luaState, 1);
		return false;
	}
	return true;
}
bool LoadValueWithIndex(lua_State& io_luaState, const int index)
{
	lua_pushinteger(&io_luaState, index);
	lua_gettable(&io_luaState, -2);
	return true;
}
