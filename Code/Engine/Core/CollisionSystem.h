#pragma once

#include "../Math/cVector.h"

namespace eae6320
{
	namespace Core
	{
		namespace CollisionSystem
		{
			struct CollidedPoint
			{
				bool isCollided;
				Math::cVector Point;
				Math::cVector Normal;
			};
			void Initialize(const char * collisionDataFileName);
			void ShutDown();

			CollidedPoint CheckCollision(Math::cVector& startPoint, Math::cVector& endPoint);
		}
	}
}