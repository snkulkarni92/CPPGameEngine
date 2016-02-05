#include "DebugShapes.h"


#include <cassert>
#include "../UserOutput/UserOutput.h"

namespace eae6320
{
	namespace Graphics
	{
		IDirect3DDevice9* DebugShape::s_direct3dDevice = NULL;
		bool DebugShape::Initialize()
		{
			bool wereThereErrors = false;
			if (!CreateVertexBuffer())
			{
				wereThereErrors = true;
				goto OnError;
			}
			if (!CreateIndexBuffer())
			{
				wereThereErrors = true;
				goto OnError;
			}
		OnError:
			if (wereThereErrors)
				ShutDown();
			return !wereThereErrors;
		}
		void DebugShape::SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice)
		{
			s_direct3dDevice = i_direct3dDevice;
		}
		void DebugShape::ReleaseDirect3dDevice()
		{
			s_direct3dDevice = NULL;
		}

		bool DebugShape::CreateIndexBuffer()
		{
			// The usage tells Direct3D how this vertex buffer will be used
			DWORD usage = 0;
			{
				// The type of vertex processing should match what was specified when the device interface was created with CreateDevice()
				const HRESULT result = GetVertexProcessingUsage(usage);
				if (FAILED(result))
				{
					return false;
				}
				// Our code will only ever write to the buffer
				usage |= D3DUSAGE_WRITEONLY;
			}

			// Create an index buffer
			unsigned int bufferSize;
			{
				bufferSize = mIndexCount * sizeof(uint32_t);
				// We'll use 32-bit indices in this class to keep things simple
				// (i.e. every index will be a 32 bit unsigned integer)
				const D3DFORMAT format = D3DFMT_INDEX32;
				// Place the index buffer into memory that Direct3D thinks is the most appropriate
				const D3DPOOL useDefaultPool = D3DPOOL_DEFAULT;
				HANDLE* notUsed = NULL;
				const HRESULT result = s_direct3dDevice->CreateIndexBuffer(bufferSize, usage, format, useDefaultPool,
					&s_indexBuffer, notUsed);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create an index buffer");
					return false;
				}
			}
			// Fill the index buffer with the triangles' connectivity data
			{
				// Before the index buffer can be changed it must be "locked"
				uint32_t* indexData;
				{
					const unsigned int lockEntireBuffer = 0;
					const DWORD useDefaultLockingBehavior = 0;
					const HRESULT result = s_indexBuffer->Lock(lockEntireBuffer, lockEntireBuffer,
						reinterpret_cast<void**>(&indexData), useDefaultLockingBehavior);
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to lock the index buffer");
						return false;
					}
				}
				// Fill the buffer
				{
					for (unsigned int i = 0; i < mIndexCount; i++)
						indexData[i] = mIndexData[i];
				}
				// The buffer must be "unlocked" before it can be used
				{
					const HRESULT result = s_indexBuffer->Unlock();
					if (FAILED(result))
					{
						eae6320::UserOutput::Print("Direct3D failed to unlock the index buffer");
						return false;
					}
				}
			}

			return true;
		}
		HRESULT GetVertexProcessingUsage(DWORD& o_usage)
		{
			D3DDEVICE_CREATION_PARAMETERS deviceCreationParameters;
			const HRESULT result = Mesh::s_direct3dDevice->GetCreationParameters(&deviceCreationParameters);
			if (SUCCEEDED(result))
			{
				DWORD vertexProcessingType = deviceCreationParameters.BehaviorFlags &
					(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_SOFTWARE_VERTEXPROCESSING);
				o_usage = (vertexProcessingType != D3DCREATE_SOFTWARE_VERTEXPROCESSING) ? 0 : D3DUSAGE_SOFTWAREPROCESSING;
			}
			else
			{
				eae6320::UserOutput::Print("Direct3D failed to get the device's creation parameters");
			}
			return result;
		}
	}
}