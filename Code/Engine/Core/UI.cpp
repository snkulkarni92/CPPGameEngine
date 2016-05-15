#include "UI.h"
#include "UIElements.h"

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
			uint32_t activeElement = 0;

			void Initialize()
			{
				HRESULT hr = D3DXCreateFont(s_direct3dDevice, 22, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New", &s_font);
				CreateUIFont(s_font);
				UIElements = new UIElement*[50];
			}

			void CreateText(const char * name, char *text)
			{
				UIText * textElement = new UIText();
				textElement->Create(name, text, elementCount * 25);
				UIElements[elementCount] = textElement;
				elementCount++;
			}

			void CreateCheckBox(const char * name, bool * value)
			{
				UICheckBox * checkBox = new UICheckBox();
				checkBox->Create(name, value, elementCount * 25);
				UIElements[elementCount] = checkBox;
				elementCount++;
			}

			void CreateSlider(const char *name, int *value, int min, int max)
			{
				UISlider *slider = new UISlider();
				slider->Create(name, value, min, max, elementCount * 25);
				UIElements[elementCount] = slider;
				elementCount++;
			}

			void CreateButton(const char* name, void(*callback)(void))
			{
				UIButton *button = new UIButton();
				button->Create(name, callback, elementCount * 25);
				UIElements[elementCount] = button;
				elementCount++;
			}
			void Update(UIInput input)
			{
				switch (input)
				{
				case Up:
					activeElement = (activeElement == 0 ? elementCount - 1 : activeElement - 1);
					break;
				case Down:
					activeElement = (activeElement == elementCount - 1 ? 0 : activeElement + 1);
					break;
				case Left:
					UIElements[activeElement]->Update(Left);
					break;
				case Right:
					UIElements[activeElement]->Update(Right);
					break;
				case Interact:
					UIElements[activeElement]->Update(Interact);
					break;
				default:
					break;
				}
			}
			void Draw()
			{
					for (uint32_t i = 0; i < elementCount; i++)
					{

						UIElements[i]->Draw(true);
					}
				}

		}
	}
}