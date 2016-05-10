#include "Player.h"
#include "CollisionSystem.h"
#include "../Graphics/DebugShape.h"
#include "../UserInput/UserInput.h"
#include "../Math/Functions.h"
#include "../Time/Time.h"

namespace eae6320
{
	namespace Core
	{
		Math::cVector camDefault(0, 80, 400);
		Math::cVector camOffset(0, 80, 400);
		void Player::UpdateInput()
		{
			Math::cVector VelocityOffset;
			if (UserInput::IsKeyPressed('W'))
				VelocityOffset.z -= 8.0f;
			if (UserInput::IsKeyPressed('S'))
				VelocityOffset.z += 8.0f;
			/*if (UserInput::IsKeyPressed('A'))
				VelocityOffset.x -= 5.0f;
			if (UserInput::IsKeyPressed('D'))
				VelocityOffset.x += 5.0f;*/
			if (UserInput::IsKeyPressed('A'))
				eulerY -= 2.0f;
			if (UserInput::IsKeyPressed('D'))
				eulerY += 2.0f;
			Velocity = VelocityOffset * Math::cMatrix_transformation(Orientation, Math::cVector(0.0f, 0.0f, 0.0f));
		}

		void Player::Update(float dt)
		{
			//Graphics::DebugShapes::AddBox(Position - Math::cVector(0, height / 2, 0), Math::cVector(50, height, 50), Math::cVector(255, 255, 255));
			Graphics::DebugShapes::AddLine(Position, Position - getLocalZ() * 100, Math::cVector(255, 0, 0));
			Orientation = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(eulerX), eae6320::Math::cVector(1, 0, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(eulerY), eae6320::Math::cVector(0, 1, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(eulerZ), eae6320::Math::cVector(0, 0, 1));
			
			Velocity += Math::cVector(0.0f, -5.0f, 0.0f);

			Math::cVector start = Position - Math::cVector(0, height, 0);
			
			CollisionSystem::CollidedPoint collisionPoint;
			Math::cVector end = start + Math::cVector(Velocity.x, 0, 0);
			collisionPoint = CollisionSystem::CheckCollision(start, end);
			Velocity -= collisionPoint.Normal * Math::Dot(collisionPoint.Normal, Velocity);

			end = start + Math::cVector(0, Velocity.y, 0);
			collisionPoint = CollisionSystem::CheckCollision(start, end);
			Velocity -= collisionPoint.Normal * Math::Dot(collisionPoint.Normal, Velocity);

			end = start + Math::cVector(0, 0, Velocity.z);
			collisionPoint = CollisionSystem::CheckCollision(start, end);
			Velocity -= collisionPoint.Normal * Math::Dot(collisionPoint.Normal, Velocity);

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

		void Player::UpdateCamera(Camera * camera)
		{
			eae6320::Math::cMatrix_transformation i_localToWorld = eae6320::Math::cMatrix_transformation(camera->Orientation, Position);
			Math::cVector valDefault = camDefault * i_localToWorld;

			CollisionSystem::CollidedPoint collisionPoint1 = CollisionSystem::CheckCollision(Position, valDefault + Velocity.z * 10);
			CollisionSystem::CollidedPoint collisionPoint2 = CollisionSystem::CheckCollision(Position, valDefault + Velocity.x * 10);
			CollisionSystem::CollidedPoint collisionPoint3 = CollisionSystem::CheckCollision(Position, valDefault + Velocity.y * 10);
			eae6320::Math::cVector val;
			if (collisionPoint1.isCollided || collisionPoint2.isCollided || collisionPoint3.isCollided)
			{
				CollisionSystem::CollidedPoint cameraCollision = CollisionSystem::CheckCollision(Position, camera->Position);
				if(cameraCollision.isCollided == true)
					camOffset.z -= 1000 * Time::GetSecondsElapsedThisFrame();
			}
			else
			{
				camOffset += (camDefault - camOffset) * Time::GetSecondsElapsedThisFrame() * 5;
			}

			val = camOffset * i_localToWorld;
			camera->Position += (val - camera->Position) * eae6320::Time::GetSecondsElapsedThisFrame() * 10;
			camera->Orientation = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(camera->eulerX), eae6320::Math::cVector(1, 0, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(camera->eulerY), eae6320::Math::cVector(0, 1, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(camera->eulerZ), eae6320::Math::cVector(0, 0, 1));
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