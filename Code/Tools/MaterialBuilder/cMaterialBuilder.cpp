// Header Files
//=============

#include "cMaterialBuilder.h"
#include "../../Engine/Graphics/Includes.h"
#include <cstdio>
#include <cassert>
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

bool eae6320::cMaterialBuilder::Build(const std::vector<std::string>&)
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
	
	//MessageBox(NULL, "", NULL, MB_OK);
	//Write to file
	if (!wereThereErrors)
	{
		FILE * oFile;
		fopen_s(&oFile, m_path_target, "wb");
		if (oFile != NULL)
		{
			lua_pushstring(luaState, "effect");
			lua_gettable(luaState, -2);
			const char * effectPath = lua_tostring(luaState, -1);
			fwrite(effectPath, strlen(effectPath) + 1, 1, oFile);
			lua_pop(luaState, 1);

			int a = sizeof(eae6320::Graphics::MatParameters);

			LoadTableWithKey(*luaState, "uniforms");

			uint8_t uniformCount = luaL_len(luaState, -1);
			fwrite(&uniformCount, sizeof(uniformCount), 1, oFile);

			for (uint8_t i = 1; i <= uniformCount; i++)
			{
				LoadTableWithIndex(*luaState, i);
				eae6320::Graphics::MatParameters uniform;
				
				uniform.handle = NULL;

				lua_pushstring(luaState, "shader");
				lua_gettable(luaState, -2);
				const char * shaderType = lua_tostring(luaState, -1);
				if (shaderType[0] == 'f')
					uniform.shaderType = eae6320::Graphics::ShaderTypes::Fragment;
				else
					uniform.shaderType = eae6320::Graphics::ShaderTypes::Vertex;
				lua_pop(luaState, 1);

				LoadTableWithKey(*luaState, "value");
				
				uniform.valueCount = luaL_len(luaState, -1);
				for (uint8_t j = 1; j <= uniform.valueCount; j++)
				{
					LoadValueWithIndex(*luaState, j);
					uniform.values[j-1] = (float)lua_tonumber(luaState, -1);
					lua_pop(luaState, 1);
				}
				lua_pop(luaState, 1);

				fwrite(&uniform, sizeof(uniform), 1, oFile);

				lua_pop(luaState, 1);
			}

			for (uint8_t i = 1; i <= uniformCount; i++)
			{
				LoadTableWithIndex(*luaState, i);

				lua_pushstring(luaState, "name");
				lua_gettable(luaState, -2);
				const char * uniformName = lua_tostring(luaState, -1);
				fwrite(uniformName, strlen(uniformName) + 1, 1, oFile);
				lua_pop(luaState, 1);

				lua_pop(luaState, 1);
			}
			lua_pop(luaState, 1);

			//Texture Data
			LoadTableWithKey(*luaState, "texture");

			lua_pushstring(luaState, "sampler");
			lua_gettable(luaState, -2);
			const char * samplerName = lua_tostring(luaState, -1);
			fwrite(samplerName, strlen(samplerName) + 1, 1, oFile);
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "path");
			lua_gettable(luaState, -2);
			const char * path = lua_tostring(luaState, -1);
			fwrite(path, strlen(path) + 1, 1, oFile);
			lua_pop(luaState, 1);


			lua_pop(luaState, 1);
		}
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
