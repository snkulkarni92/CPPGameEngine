#include "Player.h"
#include "CollisionSystem.h"

namespace eae6320
{
	namespace Core
	{
		void Player::Update(float dt)
		{
			Velocity += Math::cVector(0.0f, -1.0f, 0.0f);
			Math::cVector start, end;
			start = Position - Math::cVector(0, height, 0);
			end = start + Velocity;
			Math::cVector collisionNormal = CollisionSystem::CheckCollision(start, end);

			if (collisionNormal.y != 0)
				return;
			Velocity -= Velocity * (Math::Dot(collisionNormal, Velocity));

			Position += Velocity;
		}
	}
}