#pragma once
class I3D_model;

class C_I3D_model_cache {
public:
    LS3D_RESULT Create(
        I3D_model* pModel, 
        const char* pSzModelName, 
        uint32_t uFlags = NULL, 
        void (*pCallback)(const char*, void*) = NULL, 
        void* a6 = NULL, 
        void* a7 = NULL) {
        return ((LS3D_RESULT(__thiscall*)(C_I3D_model_cache*, I3D_model*, const char*, uint32_t, void*, void*, void*))0x4087E0)(
            this, 
            pModel, 
            pSzModelName, 
            uFlags, 
            pCallback, 
            a6, 
            a7
        );
    }
};

C_I3D_model_cache* pI3DModelCache = (C_I3D_model_cache*)0x647DD0;