#include "Texture.h"
#include "Mesh.h"

bool eae6320::Graphics::Texture::LoadTexture(const char * i_Path, eae6320::Graphics::SamplerID samplerID)
{
	const unsigned int useDimensionsFromFile = D3DX_DEFAULT_NONPOW2;
	const unsigned int useMipMapsFromFile = D3DX_FROM_FILE;
	const DWORD staticTexture = 0;
	const D3DFORMAT useFormatFromFile = D3DFMT_FROM_FILE;
	const D3DPOOL letD3dManageMemory = D3DPOOL_MANAGED;
	const DWORD useDefaultFiltering = D3DX_DEFAULT;
	const D3DCOLOR noColorKey = 0;
	D3DXIMAGE_INFO* noSourceInfo = NULL;
	PALETTEENTRY* noColorPalette = NULL;
	const HRESULT result = D3DXCreateTextureFromFileEx(eae6320::Graphics::Mesh::s_direct3dDevice, i_Path, useDimensionsFromFile, useDimensionsFromFile, useMipMapsFromFile,
		staticTexture, useFormatFromFile, letD3dManageMemory, useDefaultFiltering, useDefaultFiltering, noColorKey, noSourceInfo, noColorPalette,
		&m_Texture);

	m_SamplerID = samplerID;

	return true;
}

void eae6320::Graphics::Texture::BindTexture()
{
	Mesh::s_direct3dDevice->SetTexture(m_SamplerID, m_Texture);
}

void eae6320::Graphics::Texture::Release()
{
	m_Texture->Release();
}