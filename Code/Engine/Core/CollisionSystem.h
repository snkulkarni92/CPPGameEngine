#pragma once

#include "../Math/cVector.h"

namespace eae6320
{
	namespace Core
	{
		namespace CollisionSystem
		{
			void Initialize(const char * collisionDataFileName);
			void ShutDown();

			Math::cVector CheckCollision(Math::cVector startPoint, Math::cVector endPoint);
		}
	}
}