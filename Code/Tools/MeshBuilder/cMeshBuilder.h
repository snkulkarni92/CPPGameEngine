#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Header Files
//=============

#include "../BuilderHelper/cbBuilder.h"
#include "../../Engine/Graphics/Includes.h"
#include "../../Externals/Lua/Includes.h"

// Class Declaration
//==================

namespace eae6320
{
	class cMeshBuilder : public cbBuilder
	{
	public:
	private:
		uint32_t mVertexCount, mIndexCount;
		eae6320::Graphics::sVertex * mVertexData;
		uint32_t * mIndexData;

		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build(const std::vector<std::string>& i_arguments);

		bool ProcessMeshData(lua_State& io_luaState);
	};
}

#endif	// EAE6320_CMESHBUILDER_H
