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