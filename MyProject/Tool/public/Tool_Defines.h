#pragma once

#define MAX_STR 255

namespace Tool {
}
	enum LEVEL { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_STAGE1, LEVEL_STAGE2, LEVEL_BOSS_STAGE, LEVEL_END };

	const unsigned int g_iWinCX = 1280;
	const unsigned int g_iWinCY = 720;
extern HWND g_hWnd;

using namespace Tool;

