// Header Files
//=============

#include "Graphics.h"
#include "DebugShape.h"
#include "../Core/UI.h"
// Interface
//==========



void eae6320::Graphics::Render(eae6320::Graphics::Renderable ** i_RenderingList, const unsigned int i_RenderingListLength, bool drawPlayer, bool drawOther)
{
	ClearFrame();
	
	BeginFrame();
	{
		if (drawPlayer)
			i_RenderingList[0]->Draw();
		if (drawOther)
			i_RenderingList[1]->Draw();
		for (unsigned int i = 2; i < i_RenderingListLength; i++)
		{
			i_RenderingList[i]->Draw();
		}
		DebugShapes::Draw();
		Core::UI::Draw();
	}
	EndFrame();

	DrawFrame();
}