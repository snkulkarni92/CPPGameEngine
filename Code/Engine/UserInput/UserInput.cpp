// Header Files
//=============

#include "UserInput.h"

#include "../Windows/Includes.h"

// Helper Function Declarations
//=============================

namespace
{
	bool IsVirtualKeyPressed( const int i_virtualKeyCode );
	HWND s_mainWindow = NULL;
}
void eae6320::UserInput::Initialize(HWND mainWindow)
{
	s_mainWindow = mainWindow;
}
int lastFramePressed = 0;
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
	if (IsVirtualKeyPressed(i_virtualKeyCode) && lastFramePressed != i_virtualKeyCode)
	{
		lastFramePressed = i_virtualKeyCode;
		return true;
	}
	else if (!IsVirtualKeyPressed(i_virtualKeyCode) && lastFramePressed == i_virtualKeyCode)
	{
		lastFramePressed = 0;
	}
	return false;
}

// Helper Function Definitions
//============================

namespace
{
	bool IsVirtualKeyPressed( const int i_virtualKeyCode )
	{
		if (GetForegroundWindow() == s_mainWindow)
		{
			short keyState = GetAsyncKeyState(i_virtualKeyCode);
			const short isKeyDownMask = ~1;
			return (keyState & isKeyDownMask) != 0;
		}
		else
			return false;
	}
}
