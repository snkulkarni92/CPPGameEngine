#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "../Graphics/Renderable.h"
#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"
#include "Camera.h"

namespace eae6320
{
	namespace Core
	{
		class GameObject
		{
		public:
			GameObject();
			bool Initialize(const char * i_MeshPath, const char * i_EffectPath);
			bool Initialize(const char * i_EffectPath);
			void Update(Camera * i_Camera);
			void ShutDown();

		public:
			Graphics::Renderable * Renderable;
			Math::cVector Position;
			Math::cQuaternion Orientation;
		};
	}
}

#endif //_GAMEOBJECT_H