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
	class cMaterialBuilder : public cbBuilder
	{
	public:
		
	private:
		
		// Interface
		//==========

	public:

		// Build
		//------

		virtual bool Build(const std::vector<std::string>& i_arguments);
	};
}

#endif	// EAE6320_CMESHBUILDER_H
