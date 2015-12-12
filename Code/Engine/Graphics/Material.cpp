#include "Material.h"
#include "../UserOutput/UserOutput.h"

#include <cstdio>

namespace eae6320
{
	namespace Graphics
	{
		Material::Material()
		{
			m_Effect = new Effect();
			m_Texture = new Texture();
		}
		void Material::Load(const char * i_Path)
		{
			FILE * iFile;
			void * buffer;
			size_t fileSize;

			fopen_s(&iFile, i_Path, "rb");
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
				char * iPointer = reinterpret_cast<char *>(buffer);
								
				void * buffer2 = m_Effect->LoadEffect(iPointer);
				m_Effect->Initialize(buffer2);
				
				
				iPointer += strlen(iPointer) + 1;
				m_ParameterCount = *reinterpret_cast<uint8_t *>(iPointer);
				iPointer += sizeof(uint8_t);
				m_Parameters = new MatParameters[m_ParameterCount];
				std::memcpy(m_Parameters, iPointer, sizeof(MatParameters) * m_ParameterCount);
				iPointer += sizeof(MatParameters) * m_ParameterCount;
				for (uint8_t i = 0; i < m_ParameterCount; i++)
				{
					m_Effect->GetUniform(iPointer, &m_Parameters[i]);
					iPointer += strlen(iPointer) + 1;
				}

				char * uniformName = iPointer;
				iPointer += strlen(uniformName) + 1;

				char * texturePath = iPointer;
				iPointer += strlen(texturePath) + 1;

				SamplerID samplerID = m_Effect->GetSamplerID(uniformName);
				m_Texture->LoadTexture(texturePath, samplerID);



				free(buffer);
				fclose(iFile);
			}
		}
		void Material::ShutDown()
		{
			if (m_Effect)
			{
				m_Effect->ShutDown();
				delete m_Effect;
			}
			if (m_Texture)
			{
				m_Texture->Release();
			}
			if (m_Parameters)
			{
				delete[] m_Parameters;
			}
		}
		void Material::Bind()
		{
			m_Effect->Bind();
			m_Texture->BindTexture();
		}
		void Material::SetUniforms()
		{
			for (uint8_t i = 0; i < m_ParameterCount; i++)
			{
				m_Effect->SetUniform(&m_Parameters[i]);
			}
		}
	}
}