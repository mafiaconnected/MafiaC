// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <Engine/GalacticPlatform.h>
#include <Engine/GalacticWin32.h>
#include <Engine/GalacticDefines.h>
#include <Engine/Engine.h>

using namespace Galactic3D;

#include <WindowsX.h>

#include <Multiplayer/Multiplayer.h>
#include <Scripting/Scripting.h>

#include <MafiaHackSupport.h>

#include <LauncherVersion.h>
#include "AdditionalPackets.h"

#ifndef MAFIA2_SDK_IMPLEMENTATION
#define MAFIA2_SDK_IMPLEMENTATION
#endif
#include <m2sdk.h>

//#pragma comment(lib, "m2sdk.lib")

extern tHack* g_pHack;
extern Context* g_pContext;