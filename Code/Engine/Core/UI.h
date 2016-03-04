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
			void ToggleDebugMenu();

			class UIElement
			{
			protected:
				RECT fontRect;
				const char *name;
				bool isSelected;
			public:
				virtual void Update() = 0;
			};

			class UIText : public UIElement
			{
			private:
				char *value;
			public:
				void Create(const char *name, char *value, float left, float top);
				void Update();
			};
		}
	}
}
