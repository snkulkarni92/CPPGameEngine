// Header Files
//=============

#include "UserInput.h"

#include "../Windows/Includes.h"

// Helper Function Declarations
//=============================

namespace
{
	bool IsVirtualKeyPressed( const int i_virtualKeyCode );
}

bool lastFramePressed = false;
// Interface
//==========

bool eae6320::UserInput::IsKeyPressed( const int i_virtualKeyCode )
{
	return IsVirtualKeyPressed( i_virtualKeyCode );
}

bool eae6320::UserInput::IsMouseButtonPressed( const int i_virtualButtonCode )
{
	return IsVirtualKeyPressed( i_virtualButtonCode );
}

bool eae6320::UserInput::IsKeyUp(const int i_virtualKeyCode)
{
	if (IsVirtualKeyPressed(i_virtualKeyCode) && !lastFramePressed)
	{
		lastFramePressed = true;
		return true;
	}
	else if (!IsVirtualKeyPressed(i_virtualKeyCode) && lastFramePressed)
	{
		lastFramePressed = false;
	}
	return false;
}

// Helper Function Definitions
//============================

namespace
{
	bool IsVirtualKeyPressed( const int i_virtualKeyCode )
	{
		short keyState = GetAsyncKeyState( i_virtualKeyCode );
		const short isKeyDownMask = ~1;
		return ( keyState & isKeyDownMask ) != 0;
	}
}
