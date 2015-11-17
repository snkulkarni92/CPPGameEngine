#ifndef EAE6320_EFFECT_H
#define EAE6320_EFFECT_H

#include <cstdint>

#if defined EAE6320_PLATFORM_GL
#include "../../Externals/OpenGlExtensions/OpenGlExtensions.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#elif defined EAE6320_PLATFORM_D3D
#include <d3d9.h>
#include <d3dx9shader.h>
#endif //Platform Check

#include "../Math/cMatrix_transformation.h"

namespace eae6320
{
	namespace Graphics
	{
		class Effect
		{

			enum RenderStates : uint8_t
			{
				ALPHA_TRANSPARENCY = 1 << 0,
				DEPTH_TESTING = 1 << 1,
				DEPTH_WRITING = 1 << 2,
				FACE_CULLING = 1 << 3,
			};

			const char * vertexShaderPath;
			const char * fragmentShaderPath;
			uint8_t renderStates;
#if defined EAE6320_PLATFORM_GL
			// OpenGL encapsulates a matching vertex shader and fragment shader into what it calls a "program".
			GLuint s_programId = 0;
			GLint positionOffset = -1;
			GLint localToWorld = -1;
			GLint worldToView = -1;
			GLint viewToScreen = -1;
#elif defined EAE6320_PLATFORM_D3D
			// The vertex shader is a program that operates on vertices.
			IDirect3DVertexShader9* s_vertexShader = NULL;
			// The fragment shader is a program that operates on fragments
			IDirect3DPixelShader9* s_fragmentShader = NULL;
#endif //Platform Check

		public:
			Effect();
			bool Initialize(void * buffer);
			void * LoadEffect(const char * i_path);
			void Bind();
			void SetDrawCallUniforms(Math::cMatrix_transformation matrix1, Math::cMatrix_transformation matrix2, Math::cMatrix_transformation matrix3);
			void ShutDown();
#if defined EAE6320_PLATFORM_GL

			struct sLogInfo
			{
				GLchar* memory;
				sLogInfo(const size_t i_size) { memory = reinterpret_cast<GLchar*>(malloc(i_size)); }
				~sLogInfo() { if (memory) free(memory); }
			};

			bool CreateProgram();
			bool LoadAndAllocateShaderProgram(const char* i_path, void*& o_shader, size_t& o_size, std::string* o_errorMessage);
			bool LoadFragmentShader(const GLuint i_programId);
			bool LoadVertexShader(const GLuint i_programId);
#elif defined EAE6320_PLATFORM_D3D
			static IDirect3DDevice9* s_direct3dDevice;
			static void SetDirect3dDevice(IDirect3DDevice9* i_direct3dDevice);
			static void ReleaseDirect3dDevice();
			ID3DXConstantTable * vertexShaderConstantTable;
			D3DXHANDLE positionHandle;
			D3DXHANDLE localToWorld;
			D3DXHANDLE worldToView;
			D3DXHANDLE viewToScreen;
			bool LoadFragmentShader();
			bool LoadVertexShader();

#endif //Platform Check
		};
	}
}

#endif //EAE6320_EFFECT_H