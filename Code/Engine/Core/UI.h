#pragma once

#include "../Graphics/Sprite.h"
#include "UIElements.h"

namespace eae6320
{
	namespace Core
	{
		namespace UI
		{
			void Initialize();
			void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice);
			void ShutDown();

			void CreateText(const char * name, char * text);
			void CreateCheckBox(const char * name, bool * value);
			void CreateSlider(const char * name, int *value, int min, int max);
			void CreateButton(const char* name, void(*callback)(void));

			void ToggleDebugMenu();
			bool IsDebugMenuActive();
			void Draw();
			void Update(UIInput input);
		}
	}
}
