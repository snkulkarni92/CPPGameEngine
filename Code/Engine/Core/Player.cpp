#include "Player.h"
#include "CollisionSystem.h"
#include "../Graphics/DebugShape.h"
#include "../UserInput/UserInput.h"
#include "../Math/Functions.h"

namespace eae6320
{
	namespace Core
	{
		void Player::UpdateInput()
		{
			Math::cVector VelocityOffset;
			if (UserInput::IsKeyPressed('W'))
				VelocityOffset.z -= 5.0f;
			if (UserInput::IsKeyPressed('S'))
				VelocityOffset.z += 5.0f;
			if (UserInput::IsKeyPressed('A'))
				VelocityOffset.x -= 5.0f;
			if (UserInput::IsKeyPressed('D'))
				VelocityOffset.x += 5.0f;
			if (UserInput::IsKeyPressed(VK_LEFT))
				eulerY -= 2.0f;
			if (UserInput::IsKeyPressed(VK_RIGHT))
				eulerY += 2.0f;
			Velocity = VelocityOffset * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
		}

		void Player::Update(float dt)
		{
			Graphics::DebugShapes::AddBox(Position - Math::cVector(0, height / 2, 0), Math::cVector(50, height, 50), Math::cVector(255, 255, 255));
			Orientation = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(eulerX), eae6320::Math::cVector(1, 0, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(eulerY), eae6320::Math::cVector(0, 1, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(eulerZ), eae6320::Math::cVector(0, 0, 1));
			Math::cVector start, end;
			start = Position - Math::cVector(0, height, 0);
			end = start + Velocity;
			Math::cVector collisionNormal = CollisionSystem::CheckCollision(start, end);
			Math::cVector negativeVelocity = collisionNormal * Math::Dot(collisionNormal, Velocity);

			Velocity -= negativeVelocity;

			Position += Velocity;

			Velocity += Math::cVector(0.0f, -10.0f, 0.0f);
			start = Position - Math::cVector(0, height, 0);
			end = start + Velocity;
			collisionNormal = CollisionSystem::CheckCollision(start, end);
			negativeVelocity = collisionNormal * Math::Dot(collisionNormal, Velocity);

			Velocity -= negativeVelocity;

			Position += Velocity;
		}

		Math::cVector Player::getLocalX()
		{
			return Math::cVector(1.0f, 0.0f, 0.0f) * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
		}
		Math::cVector Player::getLocalY()
		{
			return Math::cVector(0.0f, 1.0f, 0.0f) * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
		}
		Math::cVector Player::getLocalZ()
		{
			return Math::cVector(0.0f, 0.0f, 1.0f) * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
		}
	}
}