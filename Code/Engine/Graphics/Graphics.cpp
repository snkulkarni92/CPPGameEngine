// Header Files
//=============

#include "Graphics.h"

// Interface
//==========

namespace
{
	eae6320::Graphics::Renderable ** DebugShapes = NULL;
	uint32_t DebugShapeCount = 1;
}

void eae6320::Graphics::InitializeDebugShapes()
{
	DebugShapes = new Renderable *[128];
	DebugShapes[0] = InitializeDebugLine();
	DebugShapes[1] = InitializeDebugBox();
	for (uint32_t i = 2; i < 128; i++)
	{
		DebugShapes[i] = new Renderable();
		DebugShapes[i]->Initialize("data/OpaqueDefault.material");
	}
}

void eae6320::Graphics::ShutDownDebugShapes()
{
	for (uint32_t i = 0; i < 64; i++)
	{
		DebugShapes[i]->ShutDown();
		delete DebugShapes[i];
	}
	delete[] DebugShapes;
}

void eae6320::Graphics::Render(eae6320::Graphics::Renderable ** i_RenderingList, const unsigned int i_RenderingListLength)
{
	ClearFrame();
	
	BeginFrame();
	{
		for (unsigned int i = 0; i < i_RenderingListLength; i++)
		{
			i_RenderingList[i]->Draw();
		}
		DebugShapes[0]->Mesh->DrawLines();
		DebugShapes[1]->Mesh->Draw();
		/*for (uint32_t i = 1; i < DebugShapeCount; i++)
		{
			DebugShapes[i]->Draw();
		}
		DebugShapeCount = 1;*/
	}
	EndFrame();

	DrawFrame();
}

void eae6320::Graphics::DebugLine(eae6320::Math::cVector startPoint, eae6320::Math::cVector endPoint, eae6320::Math::cVector color)
{
	sVertex startVertex, endVertex;
	startVertex.x = startPoint.x;
	startVertex.y = startPoint.y;
	startVertex.z = startPoint.z;
	startVertex.r = (uint8_t)(color.x * 255);
	startVertex.g = (uint8_t)(color.y * 255);
	startVertex.b = (uint8_t)(color.z * 255);
	endVertex.x = endPoint.x;
	endVertex.y = endPoint.y;
	endVertex.z = endPoint.z;
	endVertex.r = (uint8_t)(color.x * 255);
	endVertex.g = (uint8_t)(color.y * 255);
	endVertex.b = (uint8_t)(color.z * 255);
	startVertex.nx = endVertex.nx = 1.0f;
	startVertex.ny = endVertex.ny = 0.0f;
	startVertex.nz = endVertex.nz = 0.0f;
	DebugShapes[0]->Mesh->AddLine(startVertex, endVertex);
}

void eae6320::Graphics::DebugBox(eae6320::Math::cVector startPoint, eae6320::Math::cVector endPoint, eae6320::Math::cVector color)
{
	//DebugShapes[DebugShapeCount]->Mesh->AddBox((endPoint - startPoint), startPoint, color);
	//DebugShapeCount++;
}