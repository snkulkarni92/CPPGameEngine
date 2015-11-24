#ifndef GRAPHICS_INCLUDES_H
#define GRAPHICS_INCLUDES_H

#ifdef _DEBUG
#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

namespace eae6320
{
	namespace Graphics
	{
		typedef
#if defined( EAE6320_PLATFORM_D3D )
			// This is conceptually a D3DXHANDLE but is defined this way
			// so that external files aren't required to specify the #include path to the DirectX SDK
			const char*
#elif defined( EAE6320_PLATFORM_GL )
			int
#endif
			UniformHandle;

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