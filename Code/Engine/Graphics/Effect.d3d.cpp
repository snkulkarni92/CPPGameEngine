#include "Effect.h"

#include <cassert>
#include <sstream>
#include "../UserOutput/UserOutput.h"

namespace eae6320
{
	namespace Graphics
	{
		IDirect3DDevice9* Effect::s_direct3dDevice = NULL;
		Effect::Effect()
		{

		}
		void Effect::SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice)
		{
			s_direct3dDevice = i_direct3dDevice;
		}
		void Effect::ReleaseDirect3dDevice()
		{
			s_direct3dDevice = NULL;
		}
		bool Effect::Initialize(void * buffer)
		{
			bool wereThereErrors = false;
			if (!LoadVertexShader())
			{
				wereThereErrors = true;
				goto OnError;
			}
			if (!LoadFragmentShader())
			{
				wereThereErrors = true;
				goto OnError;
			}
		OnError:
			free(buffer);
			if(wereThereErrors)
				ShutDown();
			return !wereThereErrors;
		}

		void Effect::SetShaders()
		{
			HRESULT result = s_direct3dDevice->SetVertexShader(s_vertexShader);
			assert(SUCCEEDED(result));
			result = s_direct3dDevice->SetPixelShader(s_fragmentShader);
			assert(SUCCEEDED(result));
		}

		void Effect::Bind()
		{
			// Set the shaders
			{
				HRESULT result = s_direct3dDevice->SetVertexShader(s_vertexShader);
				assert(SUCCEEDED(result));
				result = s_direct3dDevice->SetPixelShader(s_fragmentShader);
				assert(SUCCEEDED(result));
			}
			if (renderStates & RenderStates::ALPHA_TRANSPARENCY)
			{
				s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				s_direct3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				s_direct3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
			else
			{
				s_direct3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			}
			if (renderStates & RenderStates::DEPTH_TESTING)
			{
				s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				s_direct3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
			else
			{
				s_direct3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			}
			if (renderStates & RenderStates::DEPTH_WRITING)
			{
				s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			else
			{
				s_direct3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}
			if (renderStates & RenderStates::FACE_CULLING)
			{
				s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			else
			{
				s_direct3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
		}

		void Effect::SetDrawCallUniforms(Math::cMatrix_transformation matrix1, Math::cMatrix_transformation matrix2, Math::cMatrix_transformation matrix3, float * cameraPos)
		{
			vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, localToWorld, reinterpret_cast<const D3DXMATRIX*>(&matrix1));
			vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, worldToView, reinterpret_cast<const D3DXMATRIX*>(&matrix2));
			vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, viewToScreen, reinterpret_cast<const D3DXMATRIX*>(&matrix3));
			fragmentShaderConstantTable->SetFloatArray(s_direct3dDevice, cameraPosition, cameraPos, 3);
			//HRESULT result = vertexShaderConstantTable->SetFloatArray(s_direct3dDevice, positionHandle, floatArray, 2);
		}
		void Effect::SetUniform(MatParameters * i_Uniform)
		{
			ID3DXConstantTable * constTable = (i_Uniform->shaderType == ShaderTypes::Fragment) ? fragmentShaderConstantTable : vertexShaderConstantTable;
			constTable->SetFloatArray(s_direct3dDevice, i_Uniform->handle, i_Uniform->values, i_Uniform->valueCount);
		}
		void Effect::ShutDown()
		{
			if (s_vertexShader)
			{
				s_vertexShader->Release();
				s_vertexShader = NULL;
			}
			if (s_fragmentShader)
			{
				s_fragmentShader->Release();
				s_fragmentShader = NULL;
			}
			
		}

		bool Effect::LoadFragmentShader()
		{
			//Open the shader file
			FILE * iFile;
			void * buffer;
			size_t fileSize;

			fopen_s(&iFile, fragmentShaderPath, "rb");
			if (iFile != NULL)
			{
				fseek(iFile, 0, SEEK_END);
				fileSize = ftell(iFile);
				rewind(iFile);

				buffer = (void *)malloc(fileSize);

				size_t result = fread(buffer, 1, fileSize, iFile);
				if (result != fileSize)
				{
					eae6320::UserOutput::Print("Error reading file");
					free(buffer);
				}
			}

			// Create the fragment shader object
			bool wereThereErrors = false;
			{
				HRESULT result = s_direct3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(buffer),
					&s_fragmentShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
					wereThereErrors = true;
				}

				//Get constant table
				D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(buffer), &fragmentShaderConstantTable);

				if (fragmentShaderConstantTable)
				{
					cameraPosition = fragmentShaderConstantTable->GetConstantByName(NULL, "g_CameraPosition");
				}

				free(buffer);
			}
			return !wereThereErrors;
		}
		
		bool Effect::LoadVertexShader()
		{
			//Open the shader file
			FILE * iFile;
			void * buffer = NULL;
			size_t fileSize;

			fopen_s(&iFile, vertexShaderPath, "rb");
			if (iFile != NULL)
			{
				fseek(iFile, 0, SEEK_END);
				fileSize = ftell(iFile);
				rewind(iFile);

				buffer = (void *)malloc(fileSize);

				size_t result = fread(buffer, 1, fileSize, iFile);
				if (result != fileSize)
				{
					eae6320::UserOutput::Print("Error reading file");
					free(buffer);
				}
			}

			// Create the vertex shader object
			bool wereThereErrors = false;
			{
				HRESULT result = s_direct3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(buffer),
					&s_vertexShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
					wereThereErrors = true;
				}

				//Get constant table
				D3DXGetShaderConstantTable(reinterpret_cast<const DWORD*>(buffer), &vertexShaderConstantTable);

				if (vertexShaderConstantTable)
				{
					//positionHandle = vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");
					localToWorld = vertexShaderConstantTable->GetConstantByName(NULL, "g_Local_To_World");
					worldToView = vertexShaderConstantTable->GetConstantByName(NULL, "g_World_To_View");
					viewToScreen = vertexShaderConstantTable->GetConstantByName(NULL, "g_View_To_Screen");
				}
				free(buffer);
			}
			return !wereThereErrors;
		}
		SamplerID Effect::GetSamplerID(const char * i_Name)
		{
			UniformHandle handle = fragmentShaderConstantTable->GetConstantByName(NULL, i_Name);
			return static_cast<SamplerID>(fragmentShaderConstantTable->GetSamplerIndex(handle));
		}
		void Effect::GetUniform(const char * i_Name, MatParameters *o_Uniform)
		{
			ID3DXConstantTable * constTable = (o_Uniform->shaderType == ShaderTypes::Fragment) ? fragmentShaderConstantTable : vertexShaderConstantTable;
			o_Uniform->handle = constTable->GetConstantByName(NULL, i_Name);
		}
	}
}