#include "Game.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Core/Camera.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"
#include "../../Engine/Graphics/DebugShape.h"
#include "../../Engine/Core/UI.h"

//eae6320::Math::cVector WhiteCamera(10.0f, 10.0f, 0.0f);

namespace eae6320
{
	namespace Game
	{
		bool GameLoop(int& o_exitCode, HWND s_mainWindow)
		{
			Core::GameObject * selected = NULL;

			eae6320::Graphics::Initialize(s_mainWindow);

			eae6320::Core::Camera * Camera = new eae6320::Core::Camera();
			Camera->AspectRatio = (float)1024 / (float)768;
			
			Camera->Position = eae6320::Math::cVector(0, 0, 200);

			uint32_t nObjects = 7;

			eae6320::Core::GameObject ** gameObjectList = new eae6320::Core::GameObject *[nObjects];
			for (uint32_t i = 0; i < nObjects; i++)
				gameObjectList[i] = new eae6320::Core::GameObject();
			gameObjectList[0]->Initialize("data/CTF0.msh", "data/OpaqueDefault.material");
			gameObjectList[1]->Initialize("data/CTF1.msh", "data/Floor.material");
			gameObjectList[2]->Initialize("data/CTF2.msh", "data/Railing.material");
			gameObjectList[3]->Initialize("data/CTF3.msh", "data/Cement.material");
			gameObjectList[4]->Initialize("data/CTF4.msh", "data/Metal.material");
			gameObjectList[5]->Initialize("data/CTF5.msh", "data/Cement.material");
			gameObjectList[6]->Initialize("data/CTF6.msh", "data/Walls.material");
			eae6320::Graphics::Renderable ** renderableList = new eae6320::Graphics::Renderable *[nObjects];
			for (uint32_t i = 0; i < nObjects; i++)
				renderableList[i] = gameObjectList[i]->Renderable;

			MSG message = { 0 };
			do
			{
				bool hasWindowsSentAMessage;
				{
					HWND getMessagesFromAnyWindowBelongingToTheCurrentThread = NULL;
					unsigned int getAllMessageTypes = 0;
					unsigned int ifAMessageExistsRemoveItFromTheQueue = PM_REMOVE;
					hasWindowsSentAMessage = PeekMessage(&message, getMessagesFromAnyWindowBelongingToTheCurrentThread,
						getAllMessageTypes, getAllMessageTypes, ifAMessageExistsRemoveItFromTheQueue) == TRUE;
				}
				if (!hasWindowsSentAMessage)
				{
					

					eae6320::Time::OnNewFrame();
					
					Graphics::DebugShapes::AddBox(Math::cVector(0, -200, -600), Math::cVector(100, 100, 100), Math::cVector(255, 0, 0));
					Graphics::DebugShapes::AddBox(Math::cVector(-200, -200, -600), Math::cVector(100, 100, 100), Math::cVector(0, 255, 0));
					Graphics::DebugShapes::AddSphere(Math::cVector(200, -100, -600), 100, Math::cVector(0, 0, 255));
					Graphics::DebugShapes::AddSphere(Math::cVector(400, -100, -600), 100, Math::cVector(255, 0, 255));
					Graphics::DebugShapes::AddCylinder(Math::cVector(-200, -200, -200), 50, 50, 400, Math::cVector(255, 255, 0));
					Graphics::DebugShapes::AddCylinder(Math::cVector(+200, -200, -200), 50, 50, 400, Math::cVector(0, 255, 255));

					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(200, 0, 0), Math::cVector(255, 0, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 200, 0), Math::cVector(0, 255, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 0, 200), Math::cVector(0, 0, 255));

					eae6320::Math::cVector cameraOffset(0.0f, 0.0f, 0.0f);
					float rotSpeed = 2.0f;
					{
						// Get the direction
						{
							if (UserInput::IsKeyPressed(VK_LEFT))
							{
								Camera->eulerY -= rotSpeed;
							}
							if (UserInput::IsKeyPressed(VK_RIGHT))
							{
								Camera->eulerY += rotSpeed;
							}
							if (UserInput::IsKeyPressed(VK_UP))
							{
								Camera->eulerX -= rotSpeed;
							}
							if (UserInput::IsKeyPressed(VK_DOWN))
							{
								Camera->eulerX += rotSpeed;
							}
							if (UserInput::IsKeyPressed('Q'))
							{
								Camera->eulerZ -= rotSpeed;
							}
							if (UserInput::IsKeyPressed('E'))
							{
								Camera->eulerZ += rotSpeed;
							}
							if (UserInput::IsKeyPressed('A'))
							{
								cameraOffset -= Camera->getLocalX();
							}
							if (UserInput::IsKeyPressed('D'))
							{
								cameraOffset += Camera->getLocalX();
							}
							if (UserInput::IsKeyPressed('W'))
							{
								cameraOffset -= Camera->getLocalZ();
							}
							if (UserInput::IsKeyPressed('S'))
							{
								cameraOffset += Camera->getLocalZ();
							}
							// Get the speed
							const float unitsPerSecond = 700.0f;	// This is arbitrary
							const float unitsToMove = unitsPerSecond * eae6320::Time::GetSecondsElapsedThisFrame();	// This makes the speed frame-rate-independent
																													// Normalize the offset
							cameraOffset *= unitsToMove;
						}
					}
					Camera->Position += cameraOffset;
					Camera->Orientation = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(Camera->eulerX), eae6320::Math::cVector(1, 0, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(Camera->eulerY), eae6320::Math::cVector(0, 1, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(Camera->eulerZ), eae6320::Math::cVector(0, 0, 1));
					for (uint32_t i = 0; i < nObjects; i++)
						gameObjectList[i]->Update(Camera);

					eae6320::Core::UI::Update(eae6320::Time::GetSecondsElapsedThisFrame());

					eae6320::Graphics::Render(renderableList, nObjects);
				}
				else
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			} while (message.message != WM_QUIT);


			for (uint32_t i = 0; i < nObjects; i++)
			{
				gameObjectList[i]->ShutDown();
				delete gameObjectList[i];
			}
			delete renderableList;
			delete gameObjectList;
			eae6320::Graphics::ShutDown();
			o_exitCode = static_cast<int>(message.wParam);

			return true;
		}
	}
}