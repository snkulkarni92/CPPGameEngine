#pragma once

#include "../Graphics/Sprite.h"

namespace eae6320
{
	namespace Core
	{
		namespace UI
		{
			void Initialize();
			void Draw();
			void ShutDown();
			void Update(float time);
			void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice);
		}
	}
}
