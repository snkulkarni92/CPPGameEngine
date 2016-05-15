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
#include "../../Engine/Core/Audio.h"

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
		struct Bullet
		{
			Math::cVector Direction;
			Core::GameObject * gameObject;
			float time;
			bool hasFired;
		};
		enum GameMessages
		{
			ID_ServerPosition = ID_USER_PACKET_ENUM + 1,
			ID_ClientPosition = ID_USER_PACKET_ENUM + 2,
			ID_Shoot = 3,
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

			Audio::Initialize();

			Audio::AddAudioFile("data/Audio/music.wav",true, 0.8f);
			Audio::AddAudioFile("data/Audio/bg.wav", true);
			Audio::AddAudioFile("data/Audio/playerpickup.wav");
			Audio::AddAudioFile("data/Audio/playerscore.wav");
			Audio::AddAudioFile("data/Audio/enemypickup.wav");
			Audio::AddAudioFile("data/Audio/enemyscore.wav");
			Audio::AddAudioFile("data/Audio/reset.wav");
			Audio::AddAudioFile("data/Audio/run1.wav");
			Audio::AddAudioFile("data/Audio/run2.wav");
			Audio::AddAudioFile("data/Audio/sprint.wav");

			//Audio::PlayAudio(0);
			//Audio::PlayAudio(1);

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
			if (isServer)
			{
				playerList[0]->Initialize("data/Player.msh", "data/RedTeam.material");
				playerList[1]->Initialize("data/Player.msh", "data/BlueTeam.material");
			}
			else
			{
				playerList[0]->Initialize("data/Player.msh", "data/BlueTeam.material");
				playerList[1]->Initialize("data/Player.msh", "data/RedTeam.material");
			}

			Core::GameObject ** bulletList = new Core::GameObject *[20];
			for (uint32_t i = 0; i < 20; i++)
			{
				bulletList[i] = new Core::GameObject();
				if (i < 10)
				{
					bulletList[i]->Initialize("data/Bullet.msh", "data/RedTeam.material");

				}
				else
				{
					bulletList[i]->Initialize("data/Bullet.msh", "data/BlueTeam.material");
				}
			}
			uint32_t playerBulletCount = 0, otherBulletCount = 0;
			Bullet playerBulletList[10], otherBulletList[10];

			for (uint32_t i = 0; i < 10; i++)
			{
				if (isClient)
				{
					playerBulletList[i].gameObject = bulletList[i + 10];
					otherBulletList[i].gameObject = bulletList[i];
				}
				else
				{
					otherBulletList[i].gameObject = bulletList[i + 10];
					playerBulletList[i].gameObject = bulletList[i];
				}
				playerBulletList[i].hasFired = false;
				playerBulletList[i].time = 0;
				otherBulletList[i].hasFired = false;
				otherBulletList[i].time = 0;
			}

			uint32_t nObjects = 11;
			//-500 -200 1000
			Core::GameObject ** gameObjectList = new Core::GameObject *[nObjects];
			for (uint32_t i = 0; i < nObjects; i++)
				gameObjectList[i] = new Core::GameObject();
			gameObjectList[0]->Initialize("data/Flag.msh", "data/RedTeam.material");
			gameObjectList[1]->Initialize("data/Flag.msh", "data/BlueTeam.material");
			gameObjectList[2]->Initialize("data/Area.msh", "data/RedTeam.material");
			gameObjectList[3]->Initialize("data/Area.msh", "data/BlueTeam.material");
			gameObjectList[4]->Initialize("data/CTF0.msh", "data/OpaqueDefault.material");
			gameObjectList[5]->Initialize("data/CTF1.msh", "data/Floor.material");
			gameObjectList[6]->Initialize("data/CTF2.msh", "data/Railing.material");
			gameObjectList[7]->Initialize("data/CTF3.msh", "data/Cement.material");
			gameObjectList[8]->Initialize("data/CTF4.msh", "data/Metal.material");
			gameObjectList[9]->Initialize("data/CTF5.msh", "data/Cement.material");
			gameObjectList[10]->Initialize("data/CTF6.msh", "data/Walls.material");

			Core::GameObject * playerObject = playerList[0];
			Core::GameObject * otherObject = playerList[1];

			Math::cVector redAreaPos(-500, -250, 1000), blueAreaPos(500, -250, -1000);

			Core::GameObject *playerArea = isServer ? gameObjectList[2] : gameObjectList[3];
			playerArea->Position = isServer ? redAreaPos : blueAreaPos;

			Core::GameObject *otherArea = isClient ? gameObjectList[2] : gameObjectList[3];
			otherArea->Position = isClient ? redAreaPos : blueAreaPos;

			Core::GameObject *playerFlag = isServer ? gameObjectList[0] : gameObjectList[1];
			playerFlag->Position = playerArea->Position + Math::cVector(0, 100, 0);

			Core::GameObject *otherFlag = isClient ? gameObjectList[0] : gameObjectList[1];
			otherFlag->Position = otherArea->Position + Math::cVector(0, 100, 0);

			bool playerHasFlag = false, otherHasFlag = false;

			eae6320::Graphics::Renderable ** renderableList = new eae6320::Graphics::Renderable *[nPlayers + nObjects + 20];
			for (uint32_t i = 0; i < nPlayers; i++)
			{
				renderableList[i] = playerList[i]->Renderable;
				//renderableList[i]->Offset.y -= 200;
			}
			for (uint32_t i = 0; i < 20; i++)
			{
				renderableList[nPlayers + i] = bulletList[i]->Renderable;
			}
			for (uint32_t i = 0; i < nObjects; i++)
			{
				renderableList[20+nPlayers+i] = gameObjectList[i]->Renderable;
				//renderableList[i]->Offset.y -= 200;
			}

			int playerScore = 0, otherScore = 0;
			
			char * playerScoreString = new char[20];
			sprintf(playerScoreString, "0");
			Core::UI::CreateText("Player", playerScoreString);

			char *otherScoreString = new char[20];
			sprintf(otherScoreString, "0");
			Core::UI::CreateText("Enemy", otherScoreString);

			int staminaValue = 10;
			float stamina = 10;
			Core::UI::CreateSlider("Stamina", &staminaValue, 0, 10);

			int bulletsRemaining = 10;
			Core::UI::CreateSlider("Bullets", &bulletsRemaining, 0, 10);
			
			
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
					bool shotThisFrame = false;
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
								Vector dir;
								pos.x = player->getLocalZ().x;
								pos.y = player->getLocalZ().y;
								pos.z = player->getLocalZ().z;
								bsOut.Write(dir);
								bsOut.Write(shotThisFrame);
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
								Vector dir;
								pos.x = player->getLocalZ().x;
								pos.y = player->getLocalZ().y;
								pos.z = player->getLocalZ().z;
								bsOut.Write(dir);
								bsOut.Write(shotThisFrame);
								RakNet::ConnectionState s = peer->GetConnectionState(peer->GetMyGUID());
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

								Vector dir;
								bsIn.Read(dir);
								bool s;
								bsIn.Read(s);
								if (s)
								{
									otherBulletList[otherBulletCount].hasFired = true;
									otherBulletList[otherBulletCount].Direction = Math::cVector(-dir.x, -dir.y, -dir.z);
									otherBulletCount++;
								}
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
								Vector dir;
								bsIn.Read(dir);
								bool s;
								bsIn.Read(s);
								if (s)
								{
									otherBulletList[otherBulletCount].hasFired = true;
									otherBulletList[otherBulletCount].Direction = Math::cVector(-dir.x, -dir.y, -dir.z);
									otherBulletCount++;
								}
							}
							break;
						}
						case ID_Shoot:
						{
							if (other)
							{
								Vector dir;
								RakNet::BitStream bsIn(packet->data, packet->length, false);
								bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
								bsIn.Read(dir);
								otherBulletList[otherBulletCount].hasFired = true;
								otherBulletList[otherBulletCount].Direction = Math::cVector(-dir.x, -dir.y, -dir.z);
								otherBulletCount++;
							}
							break;
						}
						default:
							//printf("Message with identifier %i has arrived.\n", packet->data[0]);
							break;
						}
					}
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(200, 0, 0), Math::cVector(255, 0, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 200, 0), Math::cVector(0, 255, 0));
					Graphics::DebugShapes::AddLine(Math::cVector(0, 0, 0), Math::cVector(0, 0, 200), Math::cVector(0, 0, 255));

					if (player)
					{
						if ((player->Position - otherArea->Position).GetLength() < 150 && !playerHasFlag)
						{
							playerHasFlag = true;
							Audio::StopAudio(3);
							Audio::PlayAudio(2);
						}
					}
					if (other)
					{
						if ((other->Position - playerArea->Position).GetLength() < 150 && !otherHasFlag)
						{
							otherHasFlag = true;
							Audio::StopAudio(5);
							Audio::PlayAudio(4);
						}
					}
					if (player)
					{
						if ((player->Position - playerArea->Position).GetLength() < 150 && playerHasFlag)
						{
							Audio::StopAudio(2);
							Audio::StopAudio(4);
							Audio::PlayAudio(3);
							playerHasFlag = false;
							otherHasFlag = false;
							playerScore++;
							sprintf(playerScoreString, "%d", playerScore);
						}
					}
					if (other)
					{
						if ((other->Position - otherArea->Position).GetLength() < 150 && otherHasFlag)
						{
							Audio::StopAudio(2);
							Audio::StopAudio(4);
							Audio::PlayAudio(5);
							otherHasFlag = false;
							playerHasFlag = false;
							otherScore++;
							sprintf(otherScoreString, "%d", otherScore);
						}
					}
					if (other)
					{
						for (uint32_t i = 0; i < 10; i++)
						{
							if (playerBulletList[i].hasFired)
							{
								if ((playerBulletList[i].gameObject->Position - other->Position).GetLength() < 20)
								{
									otherHasFlag = false;
								}
							}
							if (otherBulletList[i].hasFired)
							{
								if ((otherBulletList[i].gameObject->Position - player->Position).GetLength() < 20)
								{
									playerHasFlag = false;
								}
							}
						}
					}
					//Input Processing
					float rotSpeed = 2.0f;
					{
						// Get the direction
						{
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
									if (UserInput::IsKeyPressed(VK_SHIFT) && stamina > 0)
									{
										player->speed = 16.0f;
										stamina -= Time::GetSecondsElapsedThisFrame() * 5;
									}
									else
									{
										player->speed = 8.0f;
										if(stamina < 10)
											stamina += Time::GetSecondsElapsedThisFrame() * 2;
									}
									staminaValue = stamina;
									bulletsRemaining = 10 - playerBulletCount;
									player->UpdateInput();
									if (UserInput::IsKeyUp(VK_SPACE))
									{
										if (playerBulletCount < 10)
										{
											shotThisFrame = true;
											playerBulletList[playerBulletCount].hasFired = true;
											playerBulletList[playerBulletCount].Direction = -player->getLocalZ();
											playerBulletCount++;
											RakNet::BitStream bsOut;
											bsOut.Write((RakNet::MessageID)ID_Shoot);
											Vector dir;
											dir.x = player->getLocalZ().x;
											dir.y = player->getLocalZ().y;
											dir.z = player->getLocalZ().z;
											bsOut.Write(dir);
										}
									}
								}
							}
						}
					}
					//Update all objects
					if (player)
					{
						Audio::SetVolume(1, (player->Position).GetLength(), 400, 200);

						if (playerHasFlag)
							otherFlag->Position = player->Position + Math::cVector(60, 50, 0);
						else
							otherFlag->Position = otherArea->Position + Math::cVector(0, 100, 0);
						if (otherHasFlag && other)
							playerFlag->Position = other->Position + Math::cVector(60, 50, 0);
						else
							playerFlag->Position = playerArea->Position + Math::cVector(0, 100, 0);
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
							Vector dir;
							dir.x = player->getLocalZ().x;
							dir.y = player->getLocalZ().y;
							dir.z = player->getLocalZ().z;
							bsOut.Write(dir);
							bsOut.Write(shotThisFrame);
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
							Vector dir;
							dir.x = player->getLocalZ().x;
							dir.y = player->getLocalZ().y;
							dir.z = player->getLocalZ().z;
							bsOut.Write(dir);
							bsOut.Write(shotThisFrame);
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
						for (uint32_t i = 0; i < 10; i++)
						{
							if (playerBulletList[i].hasFired && playerBulletList[i].time < 5)
							{
								playerBulletList[i].gameObject->Position += playerBulletList[i].Direction * Time::GetSecondsElapsedThisFrame() * 200;
								playerBulletList[i].time += Time::GetSecondsElapsedThisFrame();
							}
							else
							{
								playerBulletList[i].gameObject->Position = player->Position;
								playerBulletList[i].hasFired = false;
							}
						}
						if (playerBulletList[9].time >= 5)
						{
							playerBulletCount = 0;
							for (uint32_t i = 0; i < 10; i++)
							{
								playerBulletList[i].hasFired = false;
								playerBulletList[i].time = 0;
							}
						}
						for (uint32_t i = 0; i < 10; i++)
						{
							if (otherBulletList[i].hasFired && otherBulletList[i].time < 5)
							{
								otherBulletList[i].gameObject->Position += otherBulletList[i].Direction * Time::GetSecondsElapsedThisFrame() * 200;
								otherBulletList[i].time += Time::GetSecondsElapsedThisFrame();
							}
							else
							{
								if(other)
									otherBulletList[i].gameObject->Position = other->Position;
								otherBulletList[i].hasFired = false;
							}
						}
						if (otherBulletList[9].time >= 5)
						{
							playerBulletCount = 0;
							for (uint32_t i = 0; i < 10; i++)
							{
								otherBulletList[i].hasFired = false;
								otherBulletList[i].time = 0;
							}
						}
					}
					for (uint32_t i = 0; i < nObjects; i++)
					{
						if (flyCamActive)
							gameObjectList[i]->Update(flyCamera);
						else
							gameObjectList[i]->Update(playerCamera);
					}
					for (uint32_t i = 0; i < 20; i++)
					{
						if (flyCamActive)
							bulletList[i]->Update(flyCamera);
						else
							bulletList[i]->Update(playerCamera);
					}
					if (UIDelay > 10)
					{
						UIDelay = 0;
					}
					else
					{
						UIDelay++;
					}
					//eae6320::Core::UI::Update();
					eae6320::Graphics::Render(renderableList, nObjects+nPlayers+20, player, other);
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