#ifndef EAE6320_TEXTURE_H
#define EAE6320_TEXTURE_H

#include "Includes.h"

#if defined EAE6320_PLATFORM_GL
#include "../../Externals/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#elif defined EAE6320_PLATFORM_D3D
#include <d3d9.h>
#include <d3dx9shader.h>
#endif //Platform Check

namespace eae6320
{
	namespace Graphics
	{
		class Texture
		{
#if defined EAE6320_PLATFORM_GL
			GLuint m_Texture;
			GLint m_SamplerID;
			static uint8_t s_Count;
			uint8_t m_TextureUnit;
#elif defined EAE6320_PLATFORM_D3D
			IDirect3DTexture9 * m_Texture;
			DWORD m_SamplerID;
#endif //Platform Check

		public:
			Texture() {}
			bool LoadTexture(const char * path, SamplerID samplerID);
			void BindTexture();
			void Release();
		};
	}
}

#endif //EAE6320_TEXTURE_Hs
