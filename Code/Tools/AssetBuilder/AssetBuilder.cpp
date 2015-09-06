// Header Files
//=============

#include "AssetBuilder.h"

#include <iostream>
#include <string>
#include "../../Engine/Windows/WindowsFunctions.h"
#include "../../Externals/Lua/Includes.h"

// Static Data Initialization
//===========================

namespace
{
	lua_State* s_luaState = NULL;
}

// Helper Function Declarations
//=============================

namespace
{
	// Errors can be formatted a specific way so that they show up
	// in Visual Studio's "Error List" tab
	void OutputErrorMessage( const char* const i_errorMessage, const char* const i_optionalFileName = NULL );

	// Lua Wrapper Functions
	//----------------------

	int luaCopyFile( lua_State* io_luaState );
	int luaCreateDirectoryIfNecessary( lua_State* io_luaState );
	int luaDoesFileExist( lua_State* io_luaState );
	int luaGetEnvironmentVariable( lua_State* io_luaState );
	int luaGetLastWriteTime( lua_State* io_luaState );
	int luaOutputErrorMessage( lua_State* io_luaState );
}

// Interface
//==========

bool eae6320::AssetBuilder::BuildAsset( const char* i_relativePath )
{
	// The only thing that this C/C++ function does
	// is call the corresponding Lua BuildAsset() function

	// To call a function it must be pushed onto the stack
	lua_getglobal( s_luaState, "BuildAsset" );
	// This function has a single argument
	const int argumentCount = 1;
	{
		lua_pushstring( s_luaState, i_relativePath );
	}
	// This function will return a boolean indicating success or failure
	// (if the call fails the Lua function itself should output the reason)
	const int returnValueCount = 1;
	const int noMessageHandler = 0;
	int result = lua_pcall( s_luaState, argumentCount, returnValueCount, noMessageHandler );
	if ( result == LUA_OK )
	{
		result = lua_toboolean( s_luaState, -1 );
		lua_pop( s_luaState, returnValueCount );
		return result != 0;
	}
	else
	{
		// If this branch is reached it doesn't just mean that the asset failed to build
		// (because in that case the branch above would have been reached with false as the return value),
		// but it means that some exceptional error was thrown in the function call
		const char* const errorMessage = lua_tostring( s_luaState, -1 );
		std::cerr << errorMessage << "\n";
		lua_pop( s_luaState, 1 );

		return false;
	}
}

// Initialization / Shutdown
//--------------------------

bool eae6320::AssetBuilder::Initialize()
{
	// Create a new Lua state
	{
		s_luaState = luaL_newstate();
		if ( !s_luaState )
		{
			OutputErrorMessage( "Memory allocation error creating Lua state", __FILE__ );
			return false;
		}
	}
	// Open the standard libraries
	luaL_openlibs( s_luaState );
	// Register custom functions
	{
		lua_register( s_luaState, "CopyFile", luaCopyFile );
		lua_register( s_luaState, "CreateDirectoryIfNecessary", luaCreateDirectoryIfNecessary );
		lua_register( s_luaState, "DoesFileExist", luaDoesFileExist );
		lua_register( s_luaState, "GetEnvironmentVariable", luaGetEnvironmentVariable );
		lua_register( s_luaState, "GetLastWriteTime", luaGetLastWriteTime );
		lua_register( s_luaState, "OutputErrorMessage", luaOutputErrorMessage );
	}

	// Load and execute the build script
	{
		std::string path;
		{
			std::string scriptDir;
			std::string errorMessage;
			if ( GetEnvironmentVariable( "ScriptDir", scriptDir, &errorMessage ) )
			{
				path = scriptDir + "BuildAssets.lua";
			}
			else
			{
				OutputErrorMessage( errorMessage.c_str(), __FILE__ );
				return false;
			}
		}
		const int result = luaL_dofile( s_luaState, path.c_str() );
		if ( result != LUA_OK )
		{
			const char* errorMessage = lua_tostring( s_luaState, -1 );
			std::cerr << errorMessage << "\n";
			lua_pop( s_luaState, 1 );

			return false;
		}
	}

	return true;
}

bool eae6320::AssetBuilder::ShutDown()
{
	bool wereThereErrors = false;

	if ( s_luaState )
	{
		lua_close( s_luaState );
		s_luaState = NULL;
	}

	return !wereThereErrors;
}

// Helper Function Definitions
//============================

namespace
{
	void OutputErrorMessage( const char* const i_errorMessage, const char* const i_optionalFileName )
	{
		// This formatting causes the errors to show up in Visual Studio's "Error List" tab
		std::cerr << ( i_optionalFileName ? i_optionalFileName : "Asset Build" ) << ": error: " <<
			i_errorMessage << "\n";
	}

	// Lua Wrapper Functions
	//----------------------

	int luaCopyFile( lua_State* io_luaState )
	{
		// Argument #1: The source path
		const char* i_path_source;
		i_path_source = lua_tostring(io_luaState, 1);	// How do you get the source path from the Lua state?
		// Argument #2: The target path
		const char* i_path_target;
		i_path_target = lua_tostring(io_luaState, 2);	// How do you get the target path from the Lua state?

		// Copy the file
		{
			std::string errorMessage;
			// There are many reasons that a source should be rebuilt,
			// and so even if the target already exists it should just be written over
			const bool noErrorIfTargetAlreadyExists = false;
			// Since we rely on timestamps to determine when a target was built
			// its file time should be updated when the source gets copied
			const bool updateTheTargetFileTime = true;
			if ( eae6320::CopyFile( i_path_source, i_path_target, noErrorIfTargetAlreadyExists, updateTheTargetFileTime, &errorMessage ) )
			{
				lua_pushboolean(io_luaState, true);	// Return a boolean [true]
			}
			else
			{
				lua_pushboolean(io_luaState, false);
				lua_pushstring(io_luaState, errorMessage.c_str());		// On failure return two values: a boolean [false] and the error message
			}
		}
		const int returnValueCount = 2;
		return returnValueCount;
	}

	int luaCreateDirectoryIfNecessary( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		i_path = lua_tostring(io_luaState, 1);	// How do you get the path from the Lua state?

		std::string errorMessage;
		if ( eae6320::CreateDirectoryIfNecessary( i_path, &errorMessage ) )
		{
			const int returnValueCount = 0;
			return returnValueCount;	// Return nothing
		}
		else
		{
			return luaL_error(io_luaState, errorMessage.c_str());	// Throw a Lua error with the error message
		}
		const int returnValueCount = 1;
		return returnValueCount;
	}

	int luaDoesFileExist( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		if ( lua_isstring( io_luaState, 1 ) )
		{
			i_path = lua_tostring( io_luaState, 1 );
		}
		else
		{
			return luaL_error( io_luaState,
				"Argument #1 must be a string (instead of a %s)",
				luaL_typename( io_luaState, 1 ) );
		}

		DWORD errorCode;
		std::string errorMessage;
		if ( eae6320::DoesFileExist( i_path, &errorMessage, &errorCode ) )
		{
			lua_pushboolean( io_luaState, true );
			const int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			// If DoesFileExist() returns false because the file doesn't exist
			// (i.e. the common case)
			// then the Lua function should also return false
			// (the "error message" will also be returned, although it's implied by the return value)
			if ( ( errorCode == ERROR_FILE_NOT_FOUND ) || ( errorCode == ERROR_PATH_NOT_FOUND ) )
			{
				lua_pushboolean( io_luaState, false );
				lua_pushstring( io_luaState, errorMessage.c_str() );
				const int returnValueCount = 2;
				return returnValueCount;
			}
			else
			{
				// If DoesFileExist() fails for a non-standard error, though,
				// then the Lua function will throw an error
				return luaL_error( io_luaState, errorMessage.c_str() );
			}
		}
	}

	int luaGetEnvironmentVariable( lua_State* io_luaState )
	{
		// Argument #1: The key
		const char* i_key;
		i_key = lua_tostring(io_luaState, 1);	// How do you get the key from the Lua state?

		std::string value;
		std::string errorMessage;
		if ( eae6320::GetEnvironmentVariable( i_key, value, &errorMessage ) )
		{
			lua_pushstring(io_luaState, value.c_str());	// Return the value as a string
			const int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			lua_pushnil(io_luaState);	// On failure return two values: a nil and the error message
			lua_pushstring(io_luaState, errorMessage.c_str());
			const int returnValueCount = 2;
			return returnValueCount;
		}
	}

	int luaGetLastWriteTime( lua_State* io_luaState )
	{
		// Argument #1: The path
		const char* i_path;
		i_path = lua_tostring(io_luaState, 1);	// How do you get the path from the Lua state?

		// Get the last time that the file was written to
		uint64_t lastWriteTime;
		std::string errorMessage;
		if ( eae6320::GetLastWriteTime( i_path, lastWriteTime, &errorMessage ) )
		{
			const lua_Number lastWriteTime_asNumber = static_cast<lua_Number>( lastWriteTime );
			lua_pushnumber(io_luaState, lastWriteTime_asNumber);	// Return the last write time as a number
			const int returnValueCount = 1;
			return returnValueCount;
		}
		else
		{
			return luaL_error(io_luaState, errorMessage.c_str());	// Throw a Lua error with the error message
		}
	}

	int luaOutputErrorMessage( lua_State* io_luaState )
	{
		// Argument #1: The error message
		const char* i_errorMessage;
		i_errorMessage = lua_tostring(io_luaState, 1);	// How do you get the error message from the Lua state?
		// Argument #2: An optional file name
		const char* i_optionalFileName = NULL;
		if ( !lua_isnil( io_luaState, 2 ) )
		{
			i_optionalFileName = lua_tostring(io_luaState, 2);	// How do you get the file name from the Lua state?
		}

		// Output the error message
		OutputErrorMessage( i_errorMessage, i_optionalFileName );

		const int returnValueCount = 0;
		return returnValueCount;	// Return nothing
	}
}
