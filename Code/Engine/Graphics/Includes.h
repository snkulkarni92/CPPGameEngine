#ifndef GRAPHICS_INCLUDES_H
#define GRAPHICS_INCLUDES_H

#ifdef _DEBUG
#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

#include <cstdint>

namespace eae6320
{
	namespace Graphics
	{

		struct sVertex
		{
			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			float x, y, z;
			// COLOR0
			// 4 uint8_ts == 4 bytes
			// Offset = 12
#if defined EAE6320_PLATFORM_GL
			uint8_t r, g, b, a;	// 8 bits [0,255] per RGBA channel (the alpha channel is unused but is present so that color uses a full 4 bytes)
#elif defined EAE6320_PLATFORM_D3D
			uint8_t b, g, r, a;	// Direct3D expects the byte layout of a color to be different from what you might expect
#endif //Platform Check
			//Texture Coordinates
			// 2 floats == 8 bytes
			// Offset = 16
			float u, v;
		};

		typedef
#if defined( EAE6320_PLATFORM_D3D )
			// This is conceptually a D3DXHANDLE but is defined this way
			// so that external files aren't required to specify the #include path to the DirectX SDK
			const char*
#elif defined( EAE6320_PLATFORM_GL )
			int
#endif
			UniformHandle;

		typedef
#if defined( EAE6320_PLATFORM_D3D )
			DWORD
#elif defined( EAE6320_PLATFORM_GL )
			int
#endif
			SamplerID;

		enum ShaderTypes : uint8_t
		{
			Unknown,
			Vertex,
			Fragment
		};

		struct MatParameters
		{
			float values[4];
			UniformHandle handle;
			ShaderTypes shaderType;
			uint8_t valueCount;
		};
	}
}

#endif //GRAPHICS_INCLUDES_H