#pragma once
#include "common.h"

struct IDirect3DVertexBuffer8;
class C_VBuffer;

struct COMPLETE_VB_STRUCT {
    C_VBuffer* m_pBuffer;
    IDirect3DVertexBuffer8* m_pD3DBuffer;
    uint32_t m_uBufferId;
};

enum VERTEX_FORMAT {
    NONE
};

class C_VBuffer {
public:
    C_VBuffer() {};
    ~C_VBuffer() {};

    char pad[7];
    IDirect3DVertexBuffer8* m_pBuffer;
};

class C_VBManager {
public:
    C_VBManager();
    ~C_VBManager();
    bool m_Alloc(uint32_t uUnk1, uint32_t uUnk2, uint32_t uUnk3, uint32_t uUnk4, COMPLETE_VB_STRUCT* pVBStructOout, VERTEX_FORMAT eFormat, bool bUnk5);
    bool m_Free(COMPLETE_VB_STRUCT* pVBStr);
    void m_DebugStatus2LOG(); //NOTE(DavoSK): used in debug engine, we can backport it back when we do implementation
    void m_FreeAllUnusedVBuffers(void* pUnk);
    void m_FreeAllVBuffers();
    void m_FreeAllVRAMVBuffers();
    void m_UnlockAllVBuffers();

    friend class IGraph;
};