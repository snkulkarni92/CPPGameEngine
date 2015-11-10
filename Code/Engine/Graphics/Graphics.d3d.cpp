// Header Files
//=============

#include "Graphics.h"

#include <cassert>
#include <d3d9.h>
#include <sstream>
#include "../UserOutput/UserOutput.h"

// Static Data Initialization
//===========================

namespace
{
	HWND s_renderingWindow = NULL;
	IDirect3D9* s_direct3dInterface = NULL;
	IDirect3DDevice9* s_direct3dDevice = NULL;	
}

// Helper Function Declarations
//=============================

namespace
{
	bool CreateDevice();
	bool CreateInterface();
}

// Interface
//==========

bool eae6320::Graphics::Initialize( const HWND i_renderingWindow )
{
	s_renderingWindow = i_renderingWindow;

	// Initialize the interface to the Direct3D9 library
	if ( !CreateInterface() )
	{
		return false;
	}
	// Create an interface to a Direct3D device
	if ( !CreateDevice() )
	{
		goto OnError;
	}
	
	Mesh::SetDirect3dDevice(s_direct3dDevice);
	Effect::SetDirect3dDevice(s_direct3dDevice);
	HRESULT result = s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	result = s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	result = s_direct3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	return true;

OnError:

	ShutDown();
	return false;
}

bool eae6320::Graphics::ShutDown()
{
	bool wereThereErrors = false;

	if (s_direct3dInterface)
	{
		if (s_direct3dDevice)
		{
			Mesh::ReleaseDirect3dDevice();
			Effect::ReleaseDirect3dDevice();

			s_direct3dDevice->Release();
			s_direct3dDevice = NULL;
		}

		s_direct3dInterface->Release();
		s_direct3dInterface = NULL;
	}
	s_renderingWindow = NULL;

	return !wereThereErrors;
}

void eae6320::Graphics::ClearFrame()
{
	// Every frame an entirely new image will be created.
	// Before drawing anything, then, the previous image will be erased
	// by "clearing" the image buffer (filling it with a solid color)
	const D3DRECT* subRectanglesToClear = NULL;
	const DWORD subRectangleCount = 0;
	const DWORD clearTheRenderTarget = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	D3DCOLOR clearColor;

	// Black is usually used:
	clearColor = D3DCOLOR_XRGB(0, 0, 0);

	const float ZBuffer = 1.0f;
	const DWORD noStencilBuffer = 0;
	HRESULT result = s_direct3dDevice->Clear(subRectangleCount, subRectanglesToClear,
		clearTheRenderTarget, clearColor, ZBuffer, noStencilBuffer);
	assert(SUCCEEDED(result));
}

void eae6320::Graphics::BeginFrame()
{
	HRESULT result = s_direct3dDevice->BeginScene();
	assert(SUCCEEDED(result));
}

void eae6320::Graphics::EndFrame()
{
	HRESULT result = s_direct3dDevice->EndScene();
	assert(SUCCEEDED(result));
}

void eae6320::Graphics::DrawFrame()
{
	const RECT* noSourceRectangle = NULL;
	const RECT* noDestinationRectangle = NULL;
	const HWND useDefaultWindow = NULL;
	const RGNDATA* noDirtyRegion = NULL;
	HRESULT result = s_direct3dDevice->Present(noSourceRectangle, noDestinationRectangle, useDefaultWindow, noDirtyRegion);
	assert(SUCCEEDED(result));
}

// Helper Function Definitions
//============================

namespace
{
	bool CreateDevice()
	{
		const UINT useDefaultDevice = D3DADAPTER_DEFAULT;
		const D3DDEVTYPE useHardwareRendering = D3DDEVTYPE_HAL;
		const DWORD useHardwareVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		D3DPRESENT_PARAMETERS presentationParameters = { 0 };
		{
			{
				const unsigned int useRenderingWindowDimensions = 0;
				presentationParameters.BackBufferWidth = useRenderingWindowDimensions;
				presentationParameters.BackBufferHeight = useRenderingWindowDimensions;
			}
			presentationParameters.BackBufferFormat = D3DFMT_X8R8G8B8;
			presentationParameters.BackBufferCount = 1;
			presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
			presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
			presentationParameters.hDeviceWindow = s_renderingWindow;
			presentationParameters.Windowed = TRUE;
			presentationParameters.EnableAutoDepthStencil = TRUE;
			presentationParameters.AutoDepthStencilFormat = D3DFMT_D16;
			presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		HRESULT result = s_direct3dInterface->CreateDevice( useDefaultDevice, useHardwareRendering,
			s_renderingWindow, useHardwareVertexProcessing, &presentationParameters, &s_direct3dDevice );
		if ( SUCCEEDED( result ) )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "Direct3D failed to create a Direct3D9 device" );
			return false;
		}
	}


	bool CreateInterface()
	{
		// D3D_SDK_VERSION is #defined by the Direct3D header files,
		// and is just a way to make sure that everything is up-to-date
		s_direct3dInterface = Direct3DCreate9( D3D_SDK_VERSION );
		if ( s_direct3dInterface )
		{
			return true;
		}
		else
		{
			eae6320::UserOutput::Print( "DirectX failed to create a Direct3D9 interface" );
			return false;
		}
	}
}
