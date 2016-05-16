#ifndef EAE6320_GAME_H
#define EAE6320_GAME_H

#include "WindowsCreate.h"

namespace eae6320
{
	namespace Game
	{
		void SetClient();
		void SetServer();
		void SetAudio(bool music, bool fx);
		bool GameLoop(int& o_exitCode, HWND s_mainWindow);
	}
}

#endif //EAE6320_GAME_H