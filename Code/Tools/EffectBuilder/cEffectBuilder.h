#ifndef EAE6320_CMESHBUILDER_H
#define EAE6320_CMESHBUILDER_H

// Header Files
//=============

#include "../BuilderHelper/cbBuilder.h"

#include "../../Externals/Lua/Includes.h"

// Class Declaration
//==================

namespace eae6320
{
	class cEffectBuilder : public cbBuilder
	{
	private:
		// Interface
		//==========

	public:

		enum RenderStates : uint8_t
		{
			ALPHA_TRANSPARENCY = 1 << 0,
			DEPTH_TESTING = 1 << 1,
			DEPTH_WRITING = 1 << 2,
			FACE_CULLING = 1 << 3,
		};

		// Build
		//------

		virtual bool Build(const std::vector<std::string>& i_arguments);
	};
}

#endif	// EAE6320_CMESHBUILDER_H
