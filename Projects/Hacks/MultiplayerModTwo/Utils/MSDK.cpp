#include "stdafx.h"

using namespace M2;
extern SDL_Window* g_pWindow;

namespace Mafia2SDK
{
    bool IsWindowFocused()
    {
        Uint32 iWindowFlags = SDL_GetWindowFlags(g_pWindow);
        return (iWindowFlags & SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);
    }
}