#include "Game.h"

#include "../../Engine/Graphics/Graphics.h"
#include "../../Engine/Core/Camera.h"
#include "../../Engine/Core/GameObject.h"
#include "../../Engine/Time/Time.h"
#include "../../Engine/UserInput/UserInput.h"
#include "../../Engine/Math/Functions.h"

eae6320::Math::cVector WhiteCamera(7.0f, 8.0f, 0.0f);
eae6320::Math::cVector BlackCamera(-7.0f, 8.0f, 0.0f);

struct ChessPiece
{
	int val;
	int color;
	eae6320::Core::GameObject * piece;
};

struct BoardSquare
{
	bool hasPiece;
	ChessPiece piece;
	eae6320::Core::GameObject * square;
};

int QueenX[] = { -1, -1, -1, 0, 1, 1, 1, 0 };
int QueenY[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

int movesX[8][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -1, -1, -1, 0, 1, 1, 1, 0 },
	{ -1, -1, -1, 0, 1, 1, 1, 0 },
	{ -1, -1, 1, 1, 0, 0, 0, 0 },
	{ -2, -2, -1, 1, 2, 2, 1, -1 },
	{ -1, 0, 1, 0, 0, 0, 0, 0 },
};
int movesY[8][8] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ -1, 0, 1, 1, 1, 0, -1, -1 },
	{ -1, 0, 1, 1, 1, 0, -1, -1 },
	{ -1, 1, 1, -1, 0, 0, 0, 0 },
	{ -1, 1, 2, 2, 1, -1, -2, -2 },
	{ 0, 1, 0, -1, 0, 0, 0, 0 },
};
namespace eae6320
{
	namespace Game
	{
		bool GameLoop(int& o_exitCode, HWND s_mainWindow)
		{
			bool leftKeyPressed = false, rightKeyPressed = false, upKeyPressed = false, downKeyPressed = false, spacePressed = false;
			int turn = 1;

			Core::GameObject * selected = NULL;

			eae6320::Graphics::Initialize(s_mainWindow);

			eae6320::Core::Camera * Camera = new eae6320::Core::Camera();
			Camera->AspectRatio = (float)800 / (float)600;

			Camera->Position = WhiteCamera;
			Camera->Orientation = Math::cQuaternion(Math::ConvertDegreesToRadians(50), Math::cVector(1.0f, 0.0f, 0.0f)) * Math::cQuaternion(Math::ConvertDegreesToRadians(270), Math::cVector(0.0f, 1.0f, 0.0f));

			FILE * iFile;
			void * buffer;
			size_t fileSize;

			uint32_t nObjects = 0;
			eae6320::Core::GameObject ** gameObjectList = NULL;

			fopen_s(&iFile, "data/chess.scene", "rb");
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

			Core::GameObject * SelBox = gameObjectList[33];

			BoardSquare board[8][8];
			for (uint8_t i = 0; i < 8; i++)
			{
				for (uint8_t j = 0; j < 8; j++)
				{
					int n = nObjects + i * 8 + j;
					gameObjectList[n] = board[i][j].square = new Core::GameObject();
					board[i][j].square->Initialize("data/smallbox.msh", "data/Selection.material");
					board[i][j].square->Position.z = 3.5f - j;
					board[i][j].square->Position.x = 3.5f - i;
					board[i][j].hasPiece = false;
					board[i][j].square->Renderable->Material->m_Parameters[1].values[0] = 0.0f;
				}
			}
			nObjects += 64;

			//Assignments
			{
				ChessPiece temp;
				//Kings
				temp.piece = gameObjectList[1];
				temp.val = 1;
				temp.color = 1;
				board[0][4].hasPiece = true;
				board[0][4].piece = temp;

				temp.piece = gameObjectList[17];
				temp.val = 1;
				temp.color = -1;
				board[7][4].hasPiece = true;
				board[7][4].piece = temp;

				//Queens
				temp.piece = gameObjectList[2];
				temp.val = 2;
				temp.color = 1;
				board[0][3].hasPiece = true;
				board[0][3].piece = temp;

				temp.piece = gameObjectList[18];
				temp.val = 2;
				temp.color = -1;
				board[7][3].hasPiece = true;
				board[7][3].piece = temp;

				//Bishops
				temp.piece = gameObjectList[3];
				temp.val = 3;
				temp.color = 1;
				board[0][2].hasPiece = true;
				board[0][2].piece = temp;

				temp.piece = gameObjectList[4];
				temp.val = 3;
				temp.color = 1;
				board[0][5].hasPiece = true;
				board[0][5].piece = temp;

				temp.piece = gameObjectList[19];
				temp.val = 3;
				temp.color = -1;
				board[7][2].hasPiece = true;
				board[7][2].piece = temp;

				temp.piece = gameObjectList[20];
				temp.val = 3;
				temp.color = -1;
				board[7][5].hasPiece = true;
				board[7][5].piece = temp;

				//Knights
				temp.piece = gameObjectList[5];
				temp.val = 4;
				temp.color = 1;
				board[0][1].hasPiece = true;
				board[0][1].piece = temp;

				temp.piece = gameObjectList[6];
				temp.val = 4;
				temp.color = 1;
				board[0][6].hasPiece = true;
				board[0][6].piece = temp;

				temp.piece = gameObjectList[21];
				temp.val = 4;
				temp.color = -1;
				board[7][1].hasPiece = true;
				board[7][1].piece = temp;

				temp.piece = gameObjectList[22];
				temp.val = 4;
				temp.color = -1;
				board[7][6].hasPiece = true;
				board[7][6].piece = temp;

				//Rooks

				temp.piece = gameObjectList[7];
				temp.val = 5;
				temp.color = 1;
				board[0][0].hasPiece = true;
				board[0][0].piece = temp;

				temp.piece = gameObjectList[8];
				temp.val = 5;
				temp.color = 1;
				board[0][7].hasPiece = true;
				board[0][7].piece = temp;

				temp.piece = gameObjectList[23];
				temp.val = 5;
				temp.color = -1;
				board[7][0].hasPiece = true;
				board[7][0].piece = temp;

				temp.piece = gameObjectList[24];
				temp.val = 5;
				temp.color = -1;
				board[7][7].hasPiece = true;
				board[7][7].piece = temp;

				//Pawns
				for (uint8_t i = 0; i < 8; i++)
				{
					temp.piece = gameObjectList[9 + i];
					temp.val = 6;
					temp.color = 1;
					board[1][7 - i].hasPiece = true;
					board[1][7 - i].piece = temp;
				}
				for (uint8_t i = 0; i < 8; i++)
				{
					temp.piece = gameObjectList[25 + i];
					temp.val = 6;
					temp.color = -1;
					board[6][7 - i].hasPiece = true;
					board[6][7 - i].piece = temp;
				}
			}
			

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
							if (UserInput::IsKeyPressed(VK_LEFT) && !leftKeyPressed)
							{
								leftKeyPressed = true;
								offset.z += 1.0f * turn;
							}
							else if (!UserInput::IsKeyPressed(VK_LEFT) && leftKeyPressed)
							{
								leftKeyPressed = false;
							}
							if (UserInput::IsKeyPressed(VK_RIGHT) && !rightKeyPressed)
							{
								rightKeyPressed = true;
								offset.z -= 1.0f * turn;
							}
							else if (!UserInput::IsKeyPressed(VK_RIGHT) && rightKeyPressed)
							{
								rightKeyPressed = false;
							}
							if (UserInput::IsKeyPressed(VK_UP) && !upKeyPressed)
							{
								upKeyPressed = true;
								offset.x -= 1.0f * turn;
							}
							else if (!UserInput::IsKeyPressed(VK_UP) && upKeyPressed)
							{
								upKeyPressed = false;
							}
							if (UserInput::IsKeyPressed(VK_DOWN) && !downKeyPressed)
							{
								downKeyPressed = true;
								offset.x += 1.0f * turn;
							}
							else if (!UserInput::IsKeyPressed(VK_DOWN) && downKeyPressed)
							{
								downKeyPressed = false;
							}
						}
					}
					
					SelBox->Position += offset;

					if (UserInput::IsKeyPressed(VK_SPACE) && !spacePressed)
					{
						int x = (int)(3.5f - SelBox->Position.x);
						int y = (int)(3.5f - SelBox->Position.z);
						if (x >= 0 && x < 8 && y >= 0 && y < 8)
						{
							if (selected == NULL)
							{
								if (board[x][y].hasPiece)
								{
									if (board[x][y].piece.color == turn)
									{
										selected = board[x][y].piece.piece;
										if (board[x][y].piece.val == 1)
										{
											for (int i = 0; i < 8; i++)
											{
												int m = x + movesX[board[x][y].piece.val][i];
												int n = y + movesY[board[x][y].piece.val][i];
												if (m < 0 || m > 7 || n < 0 || n > 7)
													continue;
												if (!board[m][n].hasPiece)
													board[m][n].square->Renderable->Material->m_Parameters[1].values[0] = 0.2f;
											}
										}
										else if (board[x][y].piece.val == 6)
										{
											if (x < 7 || x > 0)
											{
												if (!board[x + turn][y].hasPiece)
													board[x + turn][y].square->Renderable->Material->m_Parameters[1].values[0] = 0.2f;
												if (x == 1 && turn == 1 || x == 6 && turn == -1)
													if (!board[x + turn + turn][y].hasPiece)
														board[x + turn + turn][y].square->Renderable->Material->m_Parameters[1].values[0] = 0.2f;
											}
										}
										else if (board[x][y].piece.val == 4)
										{
											for (int i = 0; i < 8; i++)
											{
												int m = x + movesX[board[x][y].piece.val][i];
												int n = y + movesY[board[x][y].piece.val][i];
												if (m < 0 || m > 7 || n < 0 || n > 7)
													continue;
												if (!board[m][n].hasPiece)
													board[m][n].square->Renderable->Material->m_Parameters[1].values[0] = 0.2f;
											}
										}
										else if (board[x][y].piece.val != 4 || board[x][y].piece.val == -4 || board[x][y].piece.val != 6 || board[x][y].piece.val != -6)
										{
											for (int i = 0; i < 8; i++)
											{
												for (int j = 1; j < 8; j++)
												{
													int m = x + movesX[abs(board[x][y].piece.val)][i] * j;
													int n = y + movesY[abs(board[x][y].piece.val)][i] * j;
													if (m < 0 || m > 7 || n < 0 || n > 7)
														break;
													if (board[m][n].hasPiece)
														break;
													board[m][n].square->Renderable->Material->m_Parameters[1].values[0] = 0.2f;
												}
											}
										}
									}
								}
							}
							else
							{
								if (selected && !board[x][y].hasPiece && board[x][y].square->Renderable->Material->m_Parameters[1].values[0] > 0.0f)
								{
									uint8_t i = (uint8_t)(3.5f - selected->Position.x);
									uint8_t j = (uint8_t)(3.5f - selected->Position.z);
									board[i][j].hasPiece = false;
									board[x][y].hasPiece = true;
									board[x][y].piece = board[i][j].piece;
									board[i][j].piece.piece->Position = board[x][y].square->Position;
									selected = NULL;
									for (int m = 0; m < 8; m++)
									{
										for (int n = 0; n < 8; n++)
										{
											board[m][n].square->Renderable->Material->m_Parameters[1].values[0] = 0.0f;
										}
									}
									turn = turn > 0 ? -1 : 1;
									Camera->Orientation = Camera->Orientation * Math::cQuaternion(Math::ConvertDegreesToRadians(180), Math::cVector(0.0f, 1.0f, 0.0f));
									Camera->Position = turn == 1 ? WhiteCamera : BlackCamera;
								}
							}
							spacePressed = true;
						}
					}
					else if (!UserInput::IsKeyPressed(VK_SPACE) && spacePressed)
					{
						spacePressed = false;
					}
					
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