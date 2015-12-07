#ifndef EAE6320_MATERIAL_H
#define EAE6320_MATERIAL_H

#include "Effect.h"
#include "Texture.h"
#include "Includes.h"

namespace eae6320
{
	namespace Graphics
	{
		class Material
		{
		public:
			Effect * m_Effect;
			Texture * m_Texture;
			MatParameters * m_Parameters;
			uint8_t m_ParameterCount;

		public:
			Material();
			void Load(const char * i_Path);
			void ShutDown();
			void Bind();
			void SetUniforms();
		};
	}
}

#endif //EAE6320_MATERIAL_H