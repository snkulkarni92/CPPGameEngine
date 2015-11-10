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

		void Effect::Bind()
		{
			// Set the shaders
			{
				HRESULT result = s_direct3dDevice->SetVertexShader(s_vertexShader);
				assert(SUCCEEDED(result));
				result = s_direct3dDevice->SetPixelShader(s_fragmentShader);
				assert(SUCCEEDED(result));
			}
		}

		void Effect::SetDrawCallUniforms(Math::cMatrix_transformation matrix1, Math::cMatrix_transformation matrix2, Math::cMatrix_transformation matrix3)
		{
			vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, localToWorld, reinterpret_cast<const D3DXMATRIX*>(&matrix1));
			vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, worldToView, reinterpret_cast<const D3DXMATRIX*>(&matrix2));
			vertexShaderConstantTable->SetMatrixTranspose(s_direct3dDevice, viewToScreen, reinterpret_cast<const D3DXMATRIX*>(&matrix3));
			//HRESULT result = vertexShaderConstantTable->SetFloatArray(s_direct3dDevice, positionHandle, floatArray, 2);
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
			// Load the source code from file and compile it
			//ID3DXBuffer* compiledShader;
			//{
			//	const char* sourceCodeFileName = fragmentShaderPath;
			//	const D3DXMACRO defines[] =
			//	{
			//		{ "EAE6320_PLATFORM_D3D", "1" },
			//		{ NULL, NULL }
			//	};
			//	ID3DXInclude* noIncludes = NULL;
			//	const char* entryPoint = "main";
			//	const char* profile = "ps_3_0";
			//	const DWORD noFlags = 0;
			//	ID3DXBuffer* errorMessages = NULL;
			//	ID3DXConstantTable** noConstants = NULL;
			//	HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
			//		&compiledShader, &errorMessages, noConstants);
			//	if (SUCCEEDED(result))
			//	{
			//		if (errorMessages)
			//		{
			//			errorMessages->Release();
			//		}
			//	}
			//	else
			//	{
			//		if (errorMessages)
			//		{
			//			std::stringstream errorMessage;
			//			errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName
			//				<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
			//			eae6320::UserOutput::Print(errorMessage.str());
			//			errorMessages->Release();
			//		}
			//		else
			//		{
			//			std::stringstream errorMessage;
			//			errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName;
			//			eae6320::UserOutput::Print(errorMessage.str());
			//		}
			//		return false;
			//	}
			//}

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
				free(buffer);
			}
			return !wereThereErrors;
		}
		
		bool Effect::LoadVertexShader()
		{
			// Load the source code from file and compile it
			//ID3DXBuffer* compiledShader;
			//{
			//	const char* sourceCodeFileName = vertexShaderPath;
			//	const D3DXMACRO defines[] =
			//	{
			//		{ "EAE6320_PLATFORM_D3D", "1" },
			//		{ NULL, NULL }
			//	};
			//	ID3DXInclude* noIncludes = NULL;
			//	const char* entryPoint = "main";
			//	const char* profile = "vs_3_0";
			//	const DWORD noFlags = 0;
			//	ID3DXBuffer* errorMessages = NULL;
			//	//ID3DXConstantTable** offsetTable = NULL;
			//	HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
			//		&compiledShader, &errorMessages, &vertexShaderConstantTable);
			//	if (SUCCEEDED(result))
			//	{
			//		if (errorMessages)
			//		{
			//			errorMessages->Release();
			//		}
			//		if (vertexShaderConstantTable)
			//		{
			//			positionHandle = vertexShaderConstantTable->GetConstantByName(NULL, "g_position_offset");
			//		}
			//	}
			//	else
			//	{
			//		if (errorMessages)
			//		{
			//			std::stringstream errorMessage;
			//			errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName
			//				<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
			//			eae6320::UserOutput::Print(errorMessage.str());
			//			errorMessages->Release();
			//		}
			//		else
			//		{
			//			std::stringstream errorMessage;
			//			errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName;
			//			eae6320::UserOutput::Print(errorMessage.str());
			//		}
			//		return false;
			//	}
			//}

			//Open the shader file
			FILE * iFile;
			void * buffer;
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
	}
}