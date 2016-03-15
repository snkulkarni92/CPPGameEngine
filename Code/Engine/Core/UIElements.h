#pragma once

#include <d3dx9.h>

namespace eae6320
{
	namespace Core
	{
		namespace UI
		{
			void CreateUIFont(ID3DXFont *font);
			enum UIInput
			{
				Up,
				Down,
				Left, 
				Right,
				Interact
			};
			class UIElement
			{
			protected:
				RECT valueRect, nameRect;
				const char *name;
				bool isSelected;
			public:
				virtual void Update(UIInput input) = 0;
				virtual void Draw(bool isActive) = 0;
			};

			class UIText : public UIElement
			{
			private:
				char *value;
			public:
				void Create(const char *name, char *text, float top);
				void Update(UIInput input) {};
				void Draw(bool isActive);
			};

			class UICheckBox : public UIElement
			{
			private:
				bool *value;
			public:
				void Create(const char *name, bool *value, float top);
				void Update(UIInput input);
				void Draw(bool isActive);
			};

			class UISlider : public UIElement
			{
			private:
				int *value;
				int min, max;
			public:
				void Create(const char *name, int *value, int min, int max, float top);
				void Update(UIInput input);
				void Draw(bool isActive);
			};

			class UIButton : public UIElement
			{
			private:
				void(*callback) (void);
			public:
				void Create(const char* name, void(*callback)(void), float top);
				void Update(UIInput input);
				void Draw(bool isActive);
			};
		}
	}
}
