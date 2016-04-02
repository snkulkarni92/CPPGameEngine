#include "Game.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Core/Camera.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"
#include "../../Engine/Graphics/DebugShape.h"
#include "../../Engine/Core/UI.h"
#include "../../Engine/Core/CollisionSystem.h"
#include "../../Engine/Core/Player.h"

//eae6320::Math::cVector WhiteCamera(10.0f, 10.0f, 0.0f);

namespace eae6320
{
	namespace Game
	{
		const int defaultRadius = 100;
		int radius = defaultRadius;
		void ResetSphere()
		{
			radius = defaultRadius;
		}
		bool GameLoop(int& o_exitCode, HWND s_mainWindow)
		{
			Graphics::Initialize(s_mainWindow);

			Core::CollisionSystem::Initialize("data/CTF1.msh");
		
			Core::Player * player = new Core::Player();

			player->Position = Math::cVector(0, -104, 0);

			Core::Camera * playerCamera = new eae6320::Core::Camera();

			playerCamera->AspectRatio = (float)1024 / (float)768;
			playerCamera->Position = eae6320::Math::cVector(0, -200, 400);

			uint32_t nObjects = 1;

			Core::GameObject ** gameObjectList = new Core::GameObject *[nObjects];
			for (uint32_t i = 0; i < nObjects; i++)
				gameObjectList[i] = new Core::GameObject();
			gameObjectList[0]->Initialize("data/CTF1.msh", "data/OpaqueDefault.material");
			/*gameObjectList[1]->Initialize("data/CTF1.msh", "data/Floor.material");
			gameObjectList[2]->Initialize("data/CTF2.msh", "data/Railing.material");
			gameObjectList[3]->Initialize("data/CTF3.msh", "data/Cement.material");
			gameObjectList[4]->Initialize("data/CTF4.msh", "data/Metal.material");
			gameObjectList[5]->Initialize("data/CTF5.msh", "data/Cement.material");
			gameObjectList[6]->Initialize("data/CTF6.msh", "data/Walls.material");*/
			eae6320::Graphics::Renderable ** renderableList = new eae6320::Graphics::Renderable *[nObjects];
			for (uint32_t i = 0; i < nObjects; i++)
				renderableList[i] = gameObjectList[i]->Renderable;
			
			
			char * fpsString = new char[20];
			sprintf(fpsString, "Abc");
			Core::UI::CreateText("FPS", fpsString);
			
			bool sphereEnabled = false;
			Core::UI::CreateCheckBox("Sphere", &sphereEnabled);

			Core::UI::CreateSlider("Radius", &radius, 60, 460);
			Core::UI::CreateButton("Default", &ResetSphere);
			
			int UIDelay = 0;
			
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
					
					if(sphereEnabled)
						Graphics::DebugShapes::AddSphere(Math::cVector(0, -150, -600), radius, Math::cVector(255, 0, 255));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(200, 0, 0), Math::cVector(255, 0, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 200, 0), Math::cVector(0, 255, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 0, 200), Math::cVector(0, 0, 255));

					eae6320::Math::cVector cameraOffset(0.0f, 0.0f, 0.0f);
					float rotSpeed = 2.0f;
					{
						// Get the direction
						{
							if (UserInput::IsKeyUp(VK_OEM_3))
							{
								Core::UI::ToggleDebugMenu();
							}
							if (Core::UI::IsDebugMenuActive())
							{
								if (UserInput::IsKeyUp(VK_UP))
									Core::UI::Update(Core::UI::Up);
								else if (UserInput::IsKeyUp(VK_DOWN))
									Core::UI::Update(Core::UI::Down);
								else if (UserInput::IsKeyUp(VK_SPACE))
									Core::UI::Update(Core::UI::Interact);
								else if (UserInput::IsKeyUp(VK_LEFT))
									Core::UI::Update(Core::UI::Left);
								else if (UserInput::IsKeyUp(VK_RIGHT))
									Core::UI::Update(Core::UI::Right);
							}
							else
							{
								if (UserInput::IsKeyPressed(VK_LEFT))
								{
									playerCamera->eulerY -= rotSpeed;
								}
								if (UserInput::IsKeyPressed(VK_RIGHT))
								{
									playerCamera->eulerY += rotSpeed;
								}
								if (UserInput::IsKeyPressed(VK_UP))
								{
									playerCamera->eulerX -= rotSpeed;
								}
								if (UserInput::IsKeyPressed(VK_DOWN))
								{
									playerCamera->eulerX += rotSpeed;
								}
								if (UserInput::IsKeyPressed('A'))
								{
									cameraOffset -= playerCamera->getLocalX();
								}
								if (UserInput::IsKeyPressed('D'))
								{
									cameraOffset += playerCamera->getLocalX();
								}
								if (UserInput::IsKeyPressed('W'))
								{
									cameraOffset -= playerCamera->getLocalZ();
								}
								if (UserInput::IsKeyPressed('S'))
								{
									cameraOffset += playerCamera->getLocalZ();
								}
							}
							// Get the speed
							const float unitsPerSecond = 700.0f;	// This is arbitrary
							const float unitsToMove = unitsPerSecond * eae6320::Time::GetSecondsElapsedThisFrame();	// This makes the speed frame-rate-independent
																													// Normalize the offset
							cameraOffset *= unitsToMove;
						}
					}
					playerCamera->Position += cameraOffset;
					playerCamera->Orientation = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(playerCamera->eulerX), eae6320::Math::cVector(1, 0, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(playerCamera->eulerY), eae6320::Math::cVector(0, 1, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(playerCamera->eulerZ), eae6320::Math::cVector(0, 0, 1));

					player->Update(Time::GetSecondsElapsedThisFrame());

					//playerCamera->Position = player->Position;
					//playerCamera->Orientation = player->Orientation;
					for (uint32_t i = 0; i < nObjects; i++)
						gameObjectList[i]->Update(playerCamera);
					if (UIDelay > 10)
					{
						sprintf(fpsString, "%2.0f", 1 / eae6320::Time::GetSecondsElapsedThisFrame());
						UIDelay = 0;
					}
					else
					{
						UIDelay++;
					}
					//eae6320::Core::UI::Update();

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