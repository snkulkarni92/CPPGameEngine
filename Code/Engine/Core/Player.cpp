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
			
			Velocity += Math::cVector(0.0f, -5.0f, 0.0f);

			Math::cVector start = Position - Math::cVector(0, height, 0);
			
			Math::cVector collNormal;
			Math::cVector end = start + Math::cVector(Velocity.x, 0, 0);
			collNormal = CollisionSystem::CheckCollision(start, end);
			Velocity -= collNormal * Math::Dot(collNormal, Velocity);

			end = start + Math::cVector(0, Velocity.y, 0);
			collNormal = CollisionSystem::CheckCollision(start, end);
			Velocity -= collNormal * Math::Dot(collNormal, Velocity);

			end = start + Math::cVector(0, 0, Velocity.z);
			collNormal = CollisionSystem::CheckCollision(start, end);
			Velocity -= collNormal * Math::Dot(collNormal, Velocity);

			//Velocity += Math::cVector(0.0f, -10.0f, 0.0f);
			//Math::cVector start, end;
			//start = Position - Math::cVector(0, height, 0);
			//end = start + Velocity;
			//Math::cVector collisionNormal = CollisionSystem::CheckCollision(start, end);
			//Math::cVector negativeVelocity = collisionNormal * Math::Dot(collisionNormal, Velocity);

			////Velocity -= negativeVelocity;

			//end = start + Math::cVector(Velocity.x, 0, Velocity.z);
			//collisionNormal = CollisionSystem::CheckCollision(start, end);
			//negativeVelocity = collisionNormal * Math::Dot(collisionNormal, Velocity);

			//Velocity -= negativeVelocity;

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