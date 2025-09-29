#pragma once

#include <I3D/I3D_frame.h>

struct POS_NORM {
    S_vector pos;
    S_vector normal;
};

class I3D_void_interface {
    virtual int __stdcall NumRegions();
    virtual int __stdcall SetWeight(int, int, float);
    virtual float __stdcall GetWeight(int, int);
};

class I3D_visual : public I3D_frame, public I3D_void_interface {
  public:
    virtual ~I3D_visual() {}
    virtual void UpdateV() override;
    virtual void __stdcall SetSpace(bool);
    virtual void __stdcall SetShadowMode(int mode);
    virtual int __stdcall GetShadowMode();
    virtual void __stdcall SetProjectorMode(int mode);
    virtual int __stdcall GetProjectorMode();
    virtual void _stdcall UpdateBoundProc();
    virtual void m_AddFGroups();
    virtual void m_RenderFGroup(void* fgStruct);

    inline I3D_VISUAL_TYPE GetVisualType() const {
        return *(I3D_VISUAL_TYPE*)((int)this + 0x1D4);
    }
  private:
    __declspec(dllexport) void __stdcall UpdateWBoundProc();
};