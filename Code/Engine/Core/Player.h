#pragma once

#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"
#include "Camera.h"

namespace eae6320
{
	namespace Core
	{
		class Player
		{
		public:
			Math::cVector Position;
			Math::cQuaternion Orientation;
			Math::cVector Velocity;
			float eulerX, eulerY, eulerZ;
			void Update(float dt);

			void UpdateInput();
			void UpdateCamera(Camera * camera);

			Math::cVector getLocalX();
			Math::cVector getLocalY();
			Math::cVector getLocalZ();

		private:
			float height = 100;
			
		};
	}
}