#ifndef __DEBUG_SHAPE_H
#define __DEBUG_SHAPE_H
#include <cstdint>
#include <d3d9.h>
#include <d3dx9shader.h>
#include "Effect.h"
#include "../Math/cVector.h"


namespace eae6320
{
	namespace Graphics
	{
		namespace DebugShapes
		{
			void Initialize();
			void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice);
			void ShutDown();
			void ReleaseDirect3dDevice();
			void AlterMesh(Math::cVector translation, Math::cVector color, uint8_t index);
			void AddLine(Math::cVector startPoint, Math::cVector endPoint, Math::cVector color);
			void AddBox(Math::cVector center, Math::cVector extents, Math::cVector color);
			void AddSphere(Math::cVector center, float radius, Math::cVector color);
			void AddCylinder(Math::cVector center, float radius1, float radius2, float length, Math::cVector color);
			void Draw();
		}
	}
}

#endif