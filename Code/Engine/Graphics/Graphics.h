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
		void InitializeDebugShapes();
		void ShutDownDebugShapes();
		void Render(Renderable ** i_renderingList, const unsigned int i_RenderingListLength);
		bool ShutDown();

		Renderable * InitializeDebugLine();
		Renderable * InitializeDebugBox();
		Renderable * InitializeDebugSphere();
		Renderable * InitializeDebugCylinder();
		
		void ClearFrame();
		void BeginFrame();
		void EndFrame();
		void DrawFrame();

		void DebugLine(eae6320::Math::cVector startPoint, eae6320::Math::cVector endPoint, eae6320::Math::cVector color);
		void DebugBox(eae6320::Math::cVector startPoint, eae6320::Math::cVector endPoint, eae6320::Math::cVector color);
	}
}

#endif	// EAE6320_GRAPHICS_H
