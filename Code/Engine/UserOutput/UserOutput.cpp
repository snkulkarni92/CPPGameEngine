// Header Files

#include "UserOutput.h"

#include "../../Externals/Windows/Includes.h"
namespace eae6320
{
	namespace UserOutput
	{
		void Print(std::string i_pMsg)
		{
			OutputDebugString(i_pMsg.c_str());
		}
	}
}