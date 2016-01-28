#ifndef _CAMERA_H
#define _CAMERA_H

#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"
#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Core
	{
		class Camera
		{
		public:
			Math::cVector Position;
			Math::cQuaternion Orientation;
			float eulerX, eulerY, eulerZ;
			float AspectRatio;
			Camera() { eulerX = eulerY = eulerZ = 0; }
			Math::cVector getLocalX()
			{
				return Math::cVector(1.0f, 0.0f, 0.0f) * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
			}
			Math::cVector getLocalY()
			{
				return Math::cVector(0.0f, 1.0f, 0.0f) * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
			}
			Math::cVector getLocalZ()
			{
				return Math::cVector(0.0f, 0.0f, 1.0f) * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
			}
			Math::cVector getGlobalX()
			{
				return Math::cVector(1.0f, 0.0f, 0.0f);
			}
			Math::cVector getGlobalY()
			{
				return Math::cVector(0.0f, 1.0f, 0.0f);
			}
			Math::cVector getGlobalZ()
			{
				return Math::cVector(0.0f, 0.0f, 1.0f);
			}
		};
	}
}

#endif //_CAMERA_H