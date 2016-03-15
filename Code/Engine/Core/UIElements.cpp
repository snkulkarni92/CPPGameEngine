#include "UIElements.h"

namespace eae6320
{
	namespace Core
	{
		namespace UI
		{
			ID3DXFont *sFont = NULL;
			void CreateUIFont(ID3DXFont *font) { sFont = font; }
			void UIText::Create(const char *i_name, char *i_value,float top)
			{
				SetRect(&nameRect, 10, top, 1280, 768);
				SetRect(&valueRect, 150, top, 1280, 768);
				value = i_value;
				name = strdup(i_name);
			}
			void UIText::Draw(bool isActive)
			{
				D3DCOLOR color = (isActive ? 0xFF00FF00 : 0x7700FF00);
				sFont->DrawText(NULL, name, -1, &nameRect, DT_LEFT | DT_NOCLIP, color);
				sFont->DrawText(NULL, value, -1, &valueRect, DT_LEFT | DT_NOCLIP, color);
			}

			void UICheckBox::Create(const char *i_name, bool *i_value, float top)
			{
				SetRect(&nameRect, 10, top, 1280, 768);
				SetRect(&valueRect, 150, top, 1280, 720);
				value = i_value;
				name = strdup(i_name);
			}
			void UICheckBox::Draw(bool isActive)
			{
				D3DCOLOR color = (isActive ? 0xFF00FF00 : 0x7700FF00);
				sFont->DrawText(NULL, name, -1, &nameRect, DT_LEFT | DT_NOCLIP, color);
				if(*value)
					sFont->DrawText(NULL, "[x]", -1, &valueRect, DT_LEFT | DT_NOCLIP, color);
				else
					sFont->DrawText(NULL, "[ ]", -1, &valueRect, DT_LEFT | DT_NOCLIP, color);
			}
			void UICheckBox::Update(UIInput input)
			{
				if (input == Interact)
					*value = !*value;
			}

			void UISlider::Create(const char *i_name, int *i_value, int i_min, int i_max, float top)
			{
				SetRect(&nameRect, 10, top, 1280, 768);
				SetRect(&valueRect, 150, top, 1280, 720);
				value = i_value;
				min = i_min;
				max = i_max;
				name = strdup(i_name);
			}
			void UISlider::Draw(bool isActive)
			{
				D3DCOLOR color = (isActive ? 0xFF00FF00 : 0x7700FF00);
				sFont->DrawText(NULL, name, -1, &nameRect, DT_LEFT | DT_NOCLIP, color);
				char slider[] = "[....................]";
				for (int i = 0; i < ((*value - min) * 20 / (max - min)); i++)
					slider[i + 1] = '#';
				sFont->DrawText(NULL, slider, -1, &valueRect, DT_LEFT | DT_NOCLIP, color);
			}
			void UISlider::Update(UIInput input)
			{
				int change = (max - min) / 20;
				if (input == Left && *value >= min + change)
					*value -= change;
				else if (input == Right && *value <= max - change)
					*value += change;
			}

			void UIButton::Create(const char* i_name, void(*i_callback)(void), float top)
			{
				SetRect(&nameRect, 10, top, 1280, 768);
				SetRect(&valueRect, 150, top, 1280, 720);
				callback = i_callback;
				name = strdup(i_name);
			}
			void UIButton::Draw(bool isActive)
			{
				D3DCOLOR color = (isActive ? 0xFF00FF00 : 0x7700FF00);
				sFont->DrawText(NULL, name, -1, &nameRect, DT_LEFT | DT_NOCLIP, color);
				sFont->DrawText(NULL, "[+]", -1, &valueRect, DT_LEFT | DT_NOCLIP, color);
			}
			void UIButton::Update(UIInput input)
			{
				if (input == Interact)
					(*callback)();
			}
		}
	}
}