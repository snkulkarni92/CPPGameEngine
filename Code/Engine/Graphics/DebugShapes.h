#pragma once

#include <cstdint>

#include "Includes.h"
#include "../Math/cVector.h"


#include <d3d9.h>
#include <d3dx9shader.h>

namespace eae6320
{
	namespace Graphics
	{
		class DebugShape
		{
			IDirect3DVertexDeclaration9* s_vertexDeclaration = NULL;
			IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
			IDirect3DIndexBuffer9* s_indexBuffer = NULL;

			static IDirect3DDevice9* s_direct3dDevice;
			static void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice);
			static void ReleaseDirect3dDevice();

		public:
			DebugShape();
			
			bool Initialize();
			void Draw();
			bool ShutDown();

			
			void AddLine(sVertex startPoint, sVertex endPoint);
			void AddBox(eae6320::Math::cVector extents, eae6320::Math::cVector offset, eae6320::Math::cVector color);
			void DrawLines();

			bool CreateVertexBuffer();
			bool CreateIndexBuffer();
		};
	}
}