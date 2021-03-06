/*
	This file contains the function declarations for graphics
*/

#ifndef EAE6320_GRAPHICS_H
#define EAE6320_GRAPHICS_H

// Header Files
//=============

#include "../Windows/Includes.h"
#include "Renderable.h"

// Interface
//==========

namespace eae6320
{
	namespace Graphics
	{
		bool Initialize( const HWND i_renderingWindow );
		void Render(Renderable ** i_renderingList, const unsigned int i_RenderingListLength, bool drawPlayer, bool drawOther);
		bool ShutDown();
		void DrawDebugLine();
		void ClearFrame();
		void BeginFrame();
		void EndFrame();
		void DrawFrame();
	}
}

#endif	// EAE6320_GRAPHICS_H
