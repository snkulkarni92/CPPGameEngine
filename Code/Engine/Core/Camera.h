#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"

namespace eae6320
{
	namespace Core
	{
		class Camera
		{
		public:
			Math::cVector Position;
			Math::cQuaternion Orientation;
			float AspectRatio;
			Camera() {}
		};
	}
}

#endif //_CAMERA_H