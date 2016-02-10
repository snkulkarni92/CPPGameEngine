#include "DebugShape.h"
#include "../UserOutput/UserOutput.h"


#include <cassert>

namespace eae6320
{
	namespace Graphics
	{
		namespace DebugShapes
		{
			struct sVertex1
			{
				float x, y, z;
				uint8_t b, g, r, a;
			};
			IDirect3DDevice9* s_direct3dDevice = NULL;
			void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice)
			{
				s_direct3dDevice = i_direct3dDevice;
			}
			void ReleaseDirect3dDevice()
			{
				s_direct3dDevice = NULL;
			}

			ID3DXMesh *baseBoxMesh, *baseSphereMesh, *baseCylinderMesh;
			ID3DXMesh **DebugShapeList;
			uint8_t DebugShapeCount;
			Effect *DebugEffect;
			IDirect3DVertexBuffer9* s_vertexBuffer = NULL;
			sVertex1 *DebugLineList;
			uint8_t DebugLineCount;
			IDirect3DVertexDeclaration9* vertexDeclaration = NULL;

			void Initialize()
			{
				D3DXCreateCylinder(s_direct3dDevice, 1, 1, 1, 10, 10, &baseCylinderMesh, 0);
				DebugShapeList = new ID3DXMesh *[255];
				DebugShapeCount = 0;

				DebugEffect = new Effect();
				void * buffer = DebugEffect->LoadEffect("data/debug.bineffect");
				DebugEffect->Initialize(buffer);
				
				DWORD usage = 0;
				{
					D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
					const HRESULT result = s_direct3dDevice->GetCreationParameters(&deviceCreationParameters);
					if (SUCCEEDED(result))
					{
						DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
							(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
						usage = (vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
					}
					usage |= D3DUSAGE_WRITEONLY;
				}

				{
					D3DVERTEXELEMENT9 vertexElements[] =
					{
						{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

						{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },

						// The following marker signals the end of the vertex declaration
						D3DDECL_END()
					};
					s_direct3dDevice->CreateVertexDeclaration(vertexElements, &vertexDeclaration);
					s_direct3dDevice->SetVertexDeclaration(vertexDeclaration);
				}

				// Create a vertex buffer
				{
					const unsigned int bufferSize = 256 * sizeof(sVertex1);
					// We will define our own vertex format
					const DWORD useSeparateVertexDeclaration = 0;
					// Place the vertex buffer into memory that Direct3D thinks is the most appropriate
					const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
					HANDLE* const notUsed = NULL;
					const HRESULT result = s_direct3dDevice->CreateVertexBuffer(128 * sizeof(sVertex1), usage, 0, D3DPOOL_DEFAULT, &s_vertexBuffer, NULL);
				}

				DebugLineList = new sVertex1[256];
				DebugLineCount = 0;
			}

			void AddLine(Math::cVector startPoint, Math::cVector endPoint, Math::cVector color)
			{
				sVertex1 startVertex, endVertex;
				startVertex.x = startPoint.x;
				startVertex.y = startPoint.y;
				startVertex.z = startPoint.z;
				startVertex.r = color.x;
				startVertex.g = color.y;
				startVertex.b = color.z;
				endVertex.x = endPoint.x;
				endVertex.y = endPoint.y;
				endVertex.z = endPoint.z;
				endVertex.r = color.x;
				endVertex.g = color.y;
				endVertex.b = color.z;
				DebugLineList[DebugLineCount ] = startVertex;
				DebugLineList[DebugLineCount + 1] = endVertex;
				DebugLineCount+=2;
			}

			void AddBox(Math::cVector center, Math::cVector extents, Math::cVector color)
			{
				D3DXCreateBox(s_direct3dDevice, extents.x, extents.y, extents.z, &baseBoxMesh, 0);
				baseBoxMesh->CloneMeshFVF(0, D3DFVF_XYZ | D3DFVF_DIFFUSE, s_direct3dDevice, DebugShapeList + DebugShapeCount);
				baseBoxMesh->Release();
				
				AlterMesh(center, color, DebugShapeCount);
				DebugShapeCount++;
			}
			void AddSphere(Math::cVector center, float radius, Math::cVector color)
			{
				D3DXCreateSphere(s_direct3dDevice, radius, 10, 10, &baseSphereMesh, 0);
				baseSphereMesh->CloneMeshFVF(0, D3DFVF_XYZ | D3DFVF_DIFFUSE, s_direct3dDevice, DebugShapeList + DebugShapeCount);
				baseSphereMesh->Release();
				AlterMesh(center, color, DebugShapeCount);
				DebugShapeCount++;
			}
			void AddCylinder(Math::cVector center, float radius, float radius2, float length, Math::cVector color)
			{
				D3DXCreateCylinder(s_direct3dDevice, radius, radius2, length, 10, 10, &baseSphereMesh, 0);
				baseSphereMesh->CloneMeshFVF(0, D3DFVF_XYZ | D3DFVF_DIFFUSE, s_direct3dDevice, DebugShapeList + DebugShapeCount);
				baseSphereMesh->Release();
				AlterMesh(center, color, DebugShapeCount);
				DebugShapeCount++;
			}
			void AlterMesh(Math::cVector translation, Math::cVector color, uint8_t index)
			{
				sVertex1* vertexData;
				DebugShapeList[index]->LockVertexBuffer(0, reinterpret_cast<void **>(&vertexData));
				for (uint32_t i = 0; i < DebugShapeList[index]->GetNumVertices(); i++)
				{
					vertexData[i].x = vertexData[i].x + translation.x;
					vertexData[i].y = vertexData[i].y + translation.y;
					vertexData[i].z = vertexData[i].z + translation.z;
					vertexData[i].r = (uint8_t)color.x;
					vertexData[i].g = (uint8_t)color.y;
					vertexData[i].b = (uint8_t)color.z;
				}
				DebugShapeList[index]->UnlockVertexBuffer();
			}
			void Draw()
			{
				DebugEffect->SetShaders();
				s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
				s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				for (uint8_t i = 0; i < DebugShapeCount; i++)
				{
					IDirect3DVertexBuffer9 *vertexBuffer;
					DebugShapeList[i]->GetVertexBuffer(&vertexBuffer);
					s_direct3dDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(sVertex1));
					IDirect3DIndexBuffer9 *indexBuffer;
					DebugShapeList[i]->GetIndexBuffer(&indexBuffer);
					s_direct3dDevice->SetIndices(indexBuffer);
					s_direct3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, DebugShapeList[i]->GetNumVertices(), 0, DebugShapeList[i]->GetNumFaces());
					vertexBuffer->Release();
					indexBuffer->Release();
					DebugShapeList[i]->Release();
				}
				DebugShapeCount = 0;
				s_direct3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				sVertex1* vertexData;
				s_vertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&vertexData), 0);
				for (uint8_t i = 0; i < DebugLineCount; i++)
				{
					vertexData[i] = DebugLineList[i];
				}
				s_vertexBuffer->Unlock();
				s_direct3dDevice->SetStreamSource(0, s_vertexBuffer, 0, sizeof(sVertex1));
				s_direct3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, DebugLineCount);
				DebugLineCount = 0;
			}
		}
	}
}