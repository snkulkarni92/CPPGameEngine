#ifndef GRAPHICS_INCLUDES_H
#define GRAPHICS_INCLUDES_H

#ifdef _DEBUG
#define EAE6320_GRAPHICS_AREDEBUGSHADERSENABLED
#endif

namespace eae6320
{
	namespace Graphics
	{
		enum ShaderTypes
		{
			Unknown,
			Vertex,
			Fragment
		};
	}
}

#endif //GRAPHICS_INCLUDES_H