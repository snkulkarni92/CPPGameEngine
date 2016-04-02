#include "Player.h"
#include "CollisionSystem.h"
#include "../Graphics/DebugShape.h"

namespace eae6320
{
	namespace Core
	{
		void Player::Update(float dt)
		{
			Graphics::DebugShapes::AddBox(Position - Math::cVector(0, height / 2, 0), Math::cVector(50, height, 50), Math::cVector(255, 255, 255));
			//Velocity = Math::cVector(0.0f, -1.0f, 0.0f);
			Math::cVector start, end;
			start = Position - Math::cVector(0, height, 0);
			end = start + Velocity;
			Math::cVector collisionNormal = CollisionSystem::CheckCollision(start, start + Math::cVector(0.0f, -52.0f, 0.0f));

			if (collisionNormal.y != 0)
				return;
			Velocity -= Velocity * (Math::Dot(collisionNormal, Velocity));

			Position += Velocity;
		}
	}
}