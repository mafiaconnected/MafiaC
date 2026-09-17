/*
    The game's own CRT heap (operator new/delete, malloc/free), as opposed to this process'
    own CRT heap (GAlloc/GFree). Anything allocated with the wrong heap and freed/reallocated
    with the other one will corrupt memory, so use these - not `new`/`malloc` - for any buffer
    the game itself will later free (or that was allocated by the game and this code frees).
    Ported from reMafia's Memory.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
*/

#ifndef _GAMEMEMORY_H_
#define _GAMEMEMORY_H_

namespace MafiaSDK
{
    namespace GameMemory_Enum
    {
        enum FunctionAddresses
        {
            New = 0x006106FC,
            Delete = 0x0060FCF6,
            Malloc = 0x00612D80,
            Free = 0x006124F3
        };
    };

    inline void* GameNew(size_t size)
    {
        return ((void* (__cdecl*)(size_t))(GameMemory_Enum::New))(size);
    }

    inline void GameDelete(void* p)
    {
        ((void(__cdecl*)(void*))(GameMemory_Enum::Delete))(p);
    }

    inline void* GameMalloc(size_t size)
    {
        return ((void* (__cdecl*)(size_t))(GameMemory_Enum::Malloc))(size);
    }

    inline void GameFree(void* ptr)
    {
        ((void(__cdecl*)(void*))(GameMemory_Enum::Free))(ptr);
    }
};

#endif
