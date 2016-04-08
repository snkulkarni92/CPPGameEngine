#pragma once

#include "../Math/cVector.h"
#include "../Math/cQuaternion.h"

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
			void Update(float dt);

			void UpdateInput();

			Math::cVector getLocalX();
			Math::cVector getLocalY();
			Math::cVector getLocalZ();

		private:
			float height = 100;
			float eulerX, eulerY, eulerZ;
		};
	}
}