#include "UI.h"

namespace eae6320
{
	namespace Core
	{
		namespace UI
		{
			IDirect3DDevice9* s_direct3dDevice = NULL;
			bool DebugMenu = false;
			void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice)
			{
				s_direct3dDevice = i_direct3dDevice;
			}
			ID3DXFont *s_font = NULL;
			UIElement **UIElements;
			uint32_t elementCount = 0;

			void Initialize()
			{
				HRESULT hr = D3DXCreateFont(s_direct3dDevice, 22, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier", &s_font);
				UIElements = new UIElement*[50];
			}

			void Update(float time)
			{
				
			}
			void ToggleDebugMenu()
			{
				DebugMenu = !DebugMenu;
			}
			void Draw()
			{
				if (DebugMenu)
				{
					RECT font_rect;
					SetRect(&font_rect, 0, 0, 800, 600);
					int a = s_font->DrawText(NULL, "Aasdfasdfas", -1, &font_rect, DT_LEFT | DT_NOCLIP, 0xFF00FF00);
				}
			}

			void UIText::Create(const char *i_name, char *i_value, float left, float top)
			{
				SetRect(&fontRect, left, top, 1280, 768);
				value = i_value;
				name = strdup(i_name);
				UIElements[elementCount] = this;
				elementCount++;
			}

		}
	}
}