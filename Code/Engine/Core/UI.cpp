#include "UI.h"

namespace eae6320
{
	namespace Core
	{
		namespace UI
		{
			IDirect3DDevice9* s_direct3dDevice = NULL;
			void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice)
			{
				s_direct3dDevice = i_direct3dDevice;
			}

			Graphics::GameSprite s_logo;
			eae6320::Graphics::GameSprite* s_numbers = NULL;
			eae6320::Graphics::GameSprite* s_numbers1 = NULL;

			void Initialize()
			{
				s_logo = Graphics::GameSprite(10, 10);
				s_numbers = new Graphics::GameSprite(900, 50);
				s_numbers1 = new Graphics::GameSprite(950, 50);
				s_logo.Initialize(s_direct3dDevice, "data/logo.texture", 256, 256);
				s_numbers->Initialize(s_direct3dDevice, "data/numbers.texture", 512, 64);
				s_numbers1->Initialize(s_direct3dDevice, "data/numbers.texture", 512, 64);
			}

			void Update(float time)
			{
				float fps = 1/time;

				s_numbers->m_texPortion->left = 512 / 10 * (int)(fps / 10);
				s_numbers->m_texPortion->right = 512 / 10 * ((int)(fps / 10) + 1);

				s_numbers1->m_texPortion->left = 512 / 10 * ((int)fps % 10);
				s_numbers1->m_texPortion->right = 512 / 10 * ((int)fps % 10 + 1);
			}

			void Draw()
			{
				s_logo.Draw();
				s_numbers->Draw();
				s_numbers1->Draw();
			}
		}
	}
}