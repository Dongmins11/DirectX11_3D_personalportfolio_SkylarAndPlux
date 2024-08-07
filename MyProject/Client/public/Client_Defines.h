#pragma once

namespace Client {
	enum LEVEL {LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_STAGE1,LEVEL_STAGE2,LEVEL_BOSS_STAGE,LEVEL_END};

	const unsigned int  g_iWinCX = 1280;
	const unsigned int  g_iWinCY = 720;

}

extern HINSTANCE g_hInst;
extern HWND		 g_hWnd;
#define MAX_KEY 5

using namespace Client;


#include <io.h>
#include "fmod.h"



