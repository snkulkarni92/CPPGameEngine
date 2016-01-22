#include "Game.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Core/Camera.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"

eae6320::Math::cVector WhiteCamera(10.0f, 10.0f, 0.0f);

namespace eae6320
{
	namespace Game
	{
		bool GameLoop(int& o_exitCode, HWND s_mainWindow)
		{
			Core::GameObject * selected = NULL;

			eae6320::Graphics::Initialize(s_mainWindow);

			eae6320::Core::Camera * Camera = new eae6320::Core::Camera();
			Camera->AspectRatio = (float)800 / (float)600;

			Camera->Position = WhiteCamera;
			Camera->Orientation = Math::cQuaternion(Math::ConvertDegreesToRadians(0), Math::cVector(1.0f, 0.0f, 0.0f)) * Math::cQuaternion(Math::ConvertDegreesToRadians(270), Math::cVector(0.0f, 1.0f, 0.0f));

			FILE * iFile;
			void * buffer;
			size_t fileSize;

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
			/*fopen_s(&iFile, "data/chess.scene", "rb");
			if (iFile != NULL)
			{
				fseek(iFile, 0, SEEK_END);
				fileSize = ftell(iFile);
				rewind(iFile);

				buffer = (void *)malloc(fileSize);

				size_t result = fread(buffer, 1, fileSize, iFile);
				if (result != fileSize)
				{
					free(buffer);
					return NULL;
				}
				char * iPointer = reinterpret_cast<char *>(buffer);

				nObjects = *reinterpret_cast<uint32_t *>(iPointer);
				iPointer += sizeof(nObjects);

				gameObjectList = new Core::GameObject *[nObjects + 64];

				for (uint32_t i = 0; i < nObjects; i++)
				{
					gameObjectList[i] = new Core::GameObject();
					const char * mesh = iPointer;
					iPointer += strlen(mesh) + 1;
					const char * material = iPointer;
					iPointer += strlen(material) + 1;
					gameObjectList[i]->Initialize(mesh, material);
					gameObjectList[i]->Position.x = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);
					gameObjectList[i]->Position.y = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);
					gameObjectList[i]->Position.z = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);

					float x, y, z, w;
					w = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);
					x = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);
					y = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);
					z = *reinterpret_cast<float *>(iPointer);
					iPointer += sizeof(float);

					gameObjectList[i]->Orientation = Math::cQuaternion(Math::ConvertDegreesToRadians(w), Math::cVector(x, y, z));
				}
				free(buffer);
				fclose(iFile);
			}
*/

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
					eae6320::Math::cVector offset(0.0f, 0.0f);
					eae6320::Math::cVector cameraOffset(0.0f, 0.0f, 0.0f);
					eae6320::Math::cQuaternion unitRotationAroundY(eae6320::Math::ConvertDegreesToRadians(2.0f), eae6320::Math::cVector(0, -1, 0));
					{
						// Get the direction
						{
							if (UserInput::IsKeyPressed(VK_LEFT))
							{
								offset.z += 1.0f;
							}
							if (UserInput::IsKeyPressed(VK_RIGHT))
							{
								offset.z -= 1.0f;
							}
							if (UserInput::IsKeyPressed(VK_UP))
							{
								offset.x -= 1.0f;
							}
							if (UserInput::IsKeyPressed(VK_DOWN))
							{
								offset.x += 1.0f;
							}
							// Get the speed
							const float unitsPerSecond = 500.0f;	// This is arbitrary
							const float unitsToMove = unitsPerSecond * eae6320::Time::GetSecondsElapsedThisFrame();	// This makes the speed frame-rate-independent
																													// Normalize the offset
							offset *= unitsToMove;
							cameraOffset *= unitsToMove;
						}
					}
					Camera->Position += offset;
					for (uint32_t i = 0; i < nObjects; i++)
						gameObjectList[i]->Update(Camera);
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