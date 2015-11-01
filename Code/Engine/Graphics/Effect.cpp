#include "Effect.h"

#include <cstdio>
#include <cstring>

#include "../UserOutput/UserOutput.h"

void * eae6320::Graphics::Effect::LoadEffect(const char * i_path)
{
	FILE * iFile;
	void * buffer;
	size_t fileSize;

	fopen_s(&iFile, i_path, "rb");
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
			return NULL;
		}
		char * iPointer = reinterpret_cast<char *>(buffer);
		vertexShaderPath = iPointer;
		iPointer += strlen(vertexShaderPath) + 1;
		fragmentShaderPath = iPointer;
		iPointer += strlen(fragmentShaderPath) + 1;
		return buffer;
	}
	return NULL;
}