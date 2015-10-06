#include "Effect.h"

#include <cassert>
#include <sstream>
#include "../UserOutput/UserOutput.h"

namespace eae6320
{
	namespace Graphics
	{
		Effect::Effect()
		{

		}
		bool Effect::Initialize(Context context)
		{
			if (!LoadVertexShader(context))
			{
				goto OnError;
			}
			if (!LoadFragmentShader(context))
			{
				goto OnError;
			}
			return true;
		OnError:
			ShutDown();
			return false;
		}

		void Effect::Bind(Context context)
		{
			// Set the shaders
			{
				HRESULT result = context.s_direct3dDevice->SetVertexShader(s_vertexShader);
				assert(SUCCEEDED(result));
				result = context.s_direct3dDevice->SetPixelShader(s_fragmentShader);
				assert(SUCCEEDED(result));
			}
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

		bool Effect::LoadFragmentShader(Context context)
		{
			// Load the source code from file and compile it
			ID3DXBuffer* compiledShader;
			{
				const char* sourceCodeFileName = "data/fragment.shader";
				const D3DXMACRO defines[] =
				{
					{ "EAE6320_PLATFORM_D3D", "1" },
					{ NULL, NULL }
				};
				ID3DXInclude* noIncludes = NULL;
				const char* entryPoint = "main";
				const char* profile = "ps_3_0";
				const DWORD noFlags = 0;
				ID3DXBuffer* errorMessages = NULL;
				ID3DXConstantTable** noConstants = NULL;
				HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
					&compiledShader, &errorMessages, noConstants);
				if (SUCCEEDED(result))
				{
					if (errorMessages)
					{
						errorMessages->Release();
					}
				}
				else
				{
					if (errorMessages)
					{
						std::stringstream errorMessage;
						errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName
							<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
						eae6320::UserOutput::Print(errorMessage.str());
						errorMessages->Release();
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "Direct3D failed to compile the fragment shader from the file " << sourceCodeFileName;
						eae6320::UserOutput::Print(errorMessage.str());
					}
					return false;
				}
			}
			// Create the fragment shader object
			bool wereThereErrors = false;
			{
				HRESULT result = context.s_direct3dDevice->CreatePixelShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
					&s_fragmentShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the fragment shader");
					wereThereErrors = true;
				}
				compiledShader->Release();
			}
			return !wereThereErrors;
		}
		
		bool Effect::LoadVertexShader(Context context)
		{
			// Load the source code from file and compile it
			ID3DXBuffer* compiledShader;
			{
				const char* sourceCodeFileName = "data/vertex.shader";
				const D3DXMACRO defines[] =
				{
					{ "EAE6320_PLATFORM_D3D", "1" },
					{ NULL, NULL }
				};
				ID3DXInclude* noIncludes = NULL;
				const char* entryPoint = "main";
				const char* profile = "vs_3_0";
				const DWORD noFlags = 0;
				ID3DXBuffer* errorMessages = NULL;
				ID3DXConstantTable** noConstants = NULL;
				HRESULT result = D3DXCompileShaderFromFile(sourceCodeFileName, defines, noIncludes, entryPoint, profile, noFlags,
					&compiledShader, &errorMessages, noConstants);
				if (SUCCEEDED(result))
				{
					if (errorMessages)
					{
						errorMessages->Release();
					}
				}
				else
				{
					if (errorMessages)
					{
						std::stringstream errorMessage;
						errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName
							<< ":\n" << reinterpret_cast<char*>(errorMessages->GetBufferPointer());
						eae6320::UserOutput::Print(errorMessage.str());
						errorMessages->Release();
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "Direct3D failed to compile the vertex shader from the file " << sourceCodeFileName;
						eae6320::UserOutput::Print(errorMessage.str());
					}
					return false;
				}
			}
			// Create the vertex shader object
			bool wereThereErrors = false;
			{
				HRESULT result = context.s_direct3dDevice->CreateVertexShader(reinterpret_cast<DWORD*>(compiledShader->GetBufferPointer()),
					&s_vertexShader);
				if (FAILED(result))
				{
					eae6320::UserOutput::Print("Direct3D failed to create the vertex shader");
					wereThereErrors = true;
				}
				compiledShader->Release();
			}
			return !wereThereErrors;
		}
	}
}