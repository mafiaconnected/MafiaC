#pragma once

#define LAUNCHER_MAJOR 1
#define LAUNCHER_MINOR 2
#define LAUNCHER_PATCH 0
#define LAUNCHER_BUILD 0
#define LAUNCHER_VERSION_PATCH_STRING "0"
#define LAUNCHER_VERSION "1.2.0.0"
#define LAUNCHER_DISPLAY_VERSION "1.2.0"
#define LAUNCHER_PRODUCT "MafiaC"
#define LAUNCHER_PRODUCT_NAME "Mafia Connected"
#define LAUNCHER_PRODUCT_NAME_FULL "Mafia Connected " LAUNCHER_DISPLAY_VERSION

enum eGame
{
	GAME_UNKNOWN,
	GAME_MAFIA_ONE = 10, // Let's start at 10 (Since GTAC occupies the first lot to avoid confusion)
	GAME_MAFIA_TWO,
	GAME_MAFIA_THREE,
	GAME_MAFIA_ONE_DE,
	GAME_COUNT,
};
