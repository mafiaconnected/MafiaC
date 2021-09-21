#pragma once

enum eGameVersion
{
	GAMEVERSION_ONE_12,
	GAMEVERSION_ONE_12_GOG,
	GAMEVERSION_ONE_12_STEAM,
	GAMEVERSION_TWO,
	GAMEVERSION_THREE_STEAM,
	GAMEVERSION_ONE_DE_STEAM,
	GAMEVERSION_UNKNOWN,
};

#define GAMEVERSION_IS_ONE(x) ((x) == GAMEVERSION_ONE_12)
