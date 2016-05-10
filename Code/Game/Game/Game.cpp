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

#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"  // MessageID
//eae6320::Math::cVector WhiteCamera(10.0f, 10.0f, 0.0f);

namespace eae6320
{
	namespace Game
	{
		struct Vector
		{
			float x, y, z;
		};
		enum GameMessages
		{
			ID_ServerPosition = ID_USER_PACKET_ENUM + 1,
			ID_ClientPosition = ID_USER_PACKET_ENUM + 2,
		};
		const int defaultRadius = 100;
		int radius = defaultRadius;
		void ResetSphere()
		{
			radius = defaultRadius;
		}

		bool isClient = false;
		bool isServer = false;
		void SetClient()
		{
			isClient = true;
			isServer = false;
		}
		void SetServer()
		{
			isClient = false;
			isServer = true;
		}
		bool GameLoop(int& o_exitCode, HWND s_mainWindow)
		{
			Graphics::Initialize(s_mainWindow);
			UserInput::Initialize(s_mainWindow);

			RakNet::SystemAddress serverAdress;

			Core::CollisionSystem::Initialize("data/CTF7.msh");
			RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
			Core::Player *player = NULL, *other = NULL;
			if (isServer)
			{
				RakNet::SocketDescriptor sd(60000, 0);
				peer->Startup(2, &sd, 1);
				peer->SetMaximumIncomingConnections(2);
				player = new Core::Player();
			}
			else if (isClient)
			{
				RakNet::SocketDescriptor sd;
				peer->Startup(1, &sd, 1);
				RakNet::ConnectionAttemptResult res = peer->Connect("127.0.0.1", 60000, 0, 0);
				peer->SetMaximumIncomingConnections(2);
			}

			//player->Position = Math::cVector(0, -120, 0);

			Core::Camera * playerCamera = new eae6320::Core::Camera();
			Core::Camera * flyCamera = new eae6320::Core::Camera();

			bool flyCamActive = false;

			playerCamera->AspectRatio = (float)1024 / (float)768;
			flyCamera->AspectRatio = (float)1024 / (float)768;
			//playerCamera->Position = eae6320::Math::cVector(150, -200, 200);

			uint32_t nPlayers = 2;
			Core::GameObject ** playerList = new Core::GameObject *[nPlayers];
			for (uint32_t i = 0; i < nPlayers; i++)
				playerList[i] = new Core::GameObject();

			playerList[0]->Initialize("data/Player.msh", "data/OpaqueDefault.material");
			playerList[1]->Initialize("data/Player.msh", "data/OpaqueDefault.material");

			uint32_t nObjects = 7;

			Core::GameObject ** gameObjectList = new Core::GameObject *[nObjects];
			for (uint32_t i = 0; i < nObjects; i++)
				gameObjectList[i] = new Core::GameObject();
			gameObjectList[0]->Initialize("data/CTF0.msh", "data/OpaqueDefault.material");
			gameObjectList[1]->Initialize("data/CTF1.msh", "data/Floor.material");
			gameObjectList[2]->Initialize("data/CTF2.msh", "data/Railing.material");
			gameObjectList[3]->Initialize("data/CTF3.msh", "data/Cement.material");
			gameObjectList[4]->Initialize("data/CTF4.msh", "data/Metal.material");
			gameObjectList[5]->Initialize("data/CTF5.msh", "data/Cement.material");
			gameObjectList[6]->Initialize("data/CTF6.msh", "data/Walls.material");

			Core::GameObject * playerObject = playerList[0];
			Core::GameObject * otherObject = playerList[1];
			eae6320::Graphics::Renderable ** renderableList = new eae6320::Graphics::Renderable *[nPlayers + nObjects];
			for (uint32_t i = 0; i < nPlayers; i++)
			{
				renderableList[i] = playerList[i]->Renderable;
				//renderableList[i]->Offset.y -= 200;
			}
			for (uint32_t i = 0; i < nObjects; i++)
			{
				renderableList[nPlayers+i] = gameObjectList[i]->Renderable;
				//renderableList[i]->Offset.y -= 200;
			}
			
			char * fpsString = new char[20];
			sprintf(fpsString, "Abc");
			Core::UI::CreateText("FPS", fpsString);
			
			bool sphereEnabled = false;
			Core::UI::CreateCheckBox("Sphere", &sphereEnabled);

			Core::UI::CreateSlider("Radius", &radius, 60, 460);
			Core::UI::CreateButton("Default", &ResetSphere);
			
			int UIDelay = 0;
			float rotationOffset = 0;
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

					RakNet::Packet *packet;
					for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
					{
						switch (packet->data[0])
						{
						case ID_REMOTE_DISCONNECTION_NOTIFICATION:
							//printf("Another client has disconnected.\n");
							break;
						case ID_REMOTE_CONNECTION_LOST:
							//printf("Another client has lost the connection.\n");
							break;
						case ID_REMOTE_NEW_INCOMING_CONNECTION:
							//printf("Another client has connected.\n");
							break;
						case ID_CONNECTION_REQUEST_ACCEPTED:
						{
							if (isClient)
							{
								serverAdress = packet->systemAddress;
								player = new Core::Player();
								player->Position = Math::cVector(0, -120, 100);
								RakNet::BitStream bsOut;
								bsOut.Write((RakNet::MessageID)ID_ClientPosition);
								Vector pos;
								pos.x = player->Position.x;
								pos.y = player->Position.y;
								pos.z = player->Position.z;
								bsOut.Write(pos);
								RakNet::ConnectionState s = peer->GetConnectionState(peer->GetMyGUID());
								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
							}
							break;
						}
						case ID_NEW_INCOMING_CONNECTION:
						{	
							if (isServer)
							{
								RakNet::BitStream bsOut;
								bsOut.Write((RakNet::MessageID)ID_ServerPosition);
								Vector pos;
								pos.x = player->Position.x;
								pos.y = player->Position.y;
								pos.z = player->Position.z;
								bsOut.Write(pos);
								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
							}
							break;
						}
						case ID_NO_FREE_INCOMING_CONNECTIONS:
							//printf("The server is full.\n");
							break;
						case ID_DISCONNECTION_NOTIFICATION:
							if (isServer) {
								//printf("A client has disconnected.\n");
							}
							else {
								//printf("We have been disconnected.\n");
							}
							break;
						case ID_CONNECTION_LOST:
							if (isServer) {
								//printf("A client lost the connection.\n");
							}
							else {
								//printf("Connection lost.\n");
							}
							break;
						case ID_ServerPosition:
						{
							if (isClient)
							{
								Vector pos;
								RakNet::BitStream bsIn(packet->data, packet->length, false);
								bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
								bsIn.Read(pos);
								if (!other)
									other = new Core::Player();
								other->Position = Math::cVector(pos.x, pos.y, pos.z);
								break;
							}
						}
						case ID_ClientPosition:
						{
							if (isServer)
							{
								Vector pos;
								RakNet::BitStream bsIn(packet->data, packet->length, false);
								bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
								bsIn.Read(pos);
								if (!other)
									other = new Core::Player();
								other->Position = Math::cVector(pos.x, pos.y, pos.z);
								break;
							}
						}
						default:
							//printf("Message with identifier %i has arrived.\n", packet->data[0]);
							break;
						}
					}
					if(sphereEnabled)
						Graphics::DebugShapes::AddSphere(Math::cVector(0, -150, -600), radius, Math::cVector(255, 0, 255));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(200, 0, 0), Math::cVector(255, 0, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 200, 0), Math::cVector(0, 255, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 0, 200), Math::cVector(0, 0, 255));

					float rotSpeed = 2.0f;
					{
						// Get the direction
						{
							if (UserInput::IsKeyUp(VK_OEM_3))
							{
								Core::UI::ToggleDebugMenu();
							}
							if (UserInput::IsKeyUp('F'))
							{
								flyCamActive = !flyCamActive;
								if (flyCamActive)
								{
									if (player)
									{
										flyCamera->eulerX = playerCamera->eulerX;
										flyCamera->eulerY = playerCamera->eulerY;
										flyCamera->eulerZ = playerCamera->eulerZ;
										flyCamera->Position = player->Position + Math::cVector(0, 10, 0) + flyCamera->getLocalZ() * 100;
									}
									else
									{
										flyCamera->eulerX = flyCamera->eulerY = flyCamera->eulerZ = 0;
										flyCamera->Position = Math::cVector(0, 0, 0);
									}
								}
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
								if (flyCamActive)
								{
									eae6320::Math::cVector Offset(0.0f, 0.0f, 0.0f);
									if (UserInput::IsKeyPressed(VK_LEFT))
									{
										flyCamera->eulerY -= rotSpeed;
									}
									if (UserInput::IsKeyPressed(VK_RIGHT))
									{
										flyCamera->eulerY += rotSpeed;
									}
									if (UserInput::IsKeyPressed(VK_UP))
									{
										flyCamera->eulerX -= rotSpeed;
									}
									if (UserInput::IsKeyPressed(VK_DOWN))
									{
										flyCamera->eulerX += rotSpeed;
									}
									if (UserInput::IsKeyPressed('A'))
									{
										Offset -= flyCamera->getLocalX();
									}
									if (UserInput::IsKeyPressed('D'))
									{
										Offset += flyCamera->getLocalX();
									}
									if (UserInput::IsKeyPressed('W'))
									{
										Offset -= flyCamera->getLocalZ();
									}
									if (UserInput::IsKeyPressed('S'))
									{
										Offset += flyCamera->getLocalZ();
									}
									const float unitsPerSecond = 700.0f;	// This is arbitrary
									const float unitsToMove = unitsPerSecond * eae6320::Time::GetSecondsElapsedThisFrame();	// This makes the speed frame-rate-independent
																															// Normalize the offset
									Offset *= unitsToMove;
									flyCamera->Position += Offset;
									flyCamera->Orientation = eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(flyCamera->eulerX), eae6320::Math::cVector(1, 0, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(flyCamera->eulerY), eae6320::Math::cVector(0, 1, 0)) * eae6320::Math::cQuaternion(eae6320::Math::ConvertDegreesToRadians(flyCamera->eulerZ), eae6320::Math::cVector(0, 0, 1));
								}
								else if(player)
								{
									if (UserInput::IsKeyPressed(VK_LEFT))
									{
										rotationOffset = -45;
									}
									else if (UserInput::IsKeyPressed(VK_RIGHT))
									{
										rotationOffset = 45;
									}
									else
										rotationOffset = 0;
									player->UpdateInput();
								}
							}
						}
					}
					if (player)
					{
						player->Update(Time::GetSecondsElapsedThisFrame());
						if (isClient && other && player)
						{
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID)ID_ClientPosition);
							Vector pos;
							pos.x = player->Position.x;
							pos.y = player->Position.y;
							pos.z = player->Position.z;
							bsOut.Write(pos);
							RakNet::ConnectionState s = peer->GetConnectionState(peer->GetMyGUID());
							peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverAdress, false);
						}
						else if (isServer && other && player)
						{
							RakNet::BitStream bsOut;
							bsOut.Write((RakNet::MessageID)ID_ServerPosition);
							Vector pos;
							pos.x = player->Position.x;
							pos.y = player->Position.y;
							pos.z = player->Position.z;
							bsOut.Write(pos);
							RakNet::ConnectionState s = peer->GetConnectionState(peer->GetMyGUID());
								peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::AddressOrGUID(), true);
						}

						playerCamera->eulerX = 5;
						playerCamera->eulerY += (player->eulerY + rotationOffset - playerCamera->eulerY) * eae6320::Time::GetSecondsElapsedThisFrame() * 3;

						player->UpdateCamera(playerCamera);

						playerObject->Position = player->Position;
						playerObject->Orientation = player->Orientation;
						if (other)
						{
							otherObject->Position = other->Position;
							otherObject->Orientation = other->Orientation;
						}
						if (flyCamActive)
							playerList[0]->Update(flyCamera);
						else
							playerList[0]->Update(playerCamera);

						if (other)
						{
							if (flyCamActive)
								playerList[1]->Update(flyCamera);
							else
								playerList[1]->Update(playerCamera);
						}
					}
					for (uint32_t i = 0; i < nObjects; i++)
					{
						if (flyCamActive)
							gameObjectList[i]->Update(flyCamera);
						else
							gameObjectList[i]->Update(playerCamera);
					}
					if (UIDelay > 10)
					{
						sprintf(fpsString, "%f", playerCamera->Position.y);
						UIDelay = 0;
					}
					else
					{
						UIDelay++;
					}
					//eae6320::Core::UI::Update();
					eae6320::Graphics::Render(renderableList, nObjects+nPlayers, player, other);
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

			RakNet::RakPeerInterface::DestroyInstance(peer);
			eae6320::Graphics::ShutDown();
			o_exitCode = static_cast<int>(message.wParam);

			return true;
		}
	}
}