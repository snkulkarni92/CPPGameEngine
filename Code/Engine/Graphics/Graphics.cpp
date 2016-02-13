// Header Files
//=============

#include "Graphics.h"
#include "DebugShape.h"
#include "../Core/UI.h"
// Interface
//==========



void eae6320::Graphics::Render(eae6320::Graphics::Renderable ** i_RenderingList, const unsigned int i_RenderingListLength)
{
	ClearFrame();
	
	BeginFrame();
	{
		for (unsigned int i = 0; i < i_RenderingListLength; i++)
		{
			i_RenderingList[i]->Draw();
		}
		DebugShapes::Draw();
		Core::UI::Draw();
	}
	EndFrame();

	DrawFrame();
}