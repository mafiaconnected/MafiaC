#pragma once

#include <I3D/I3D_frame.h>

struct I3D_mesh_object;

class I3D_emitor : public I3D_frame {
  public:
    virtual void __stdcall Tick(int32_t time) override;
    virtual void __stdcall SetOn(bool enable) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_emitor() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetMesh(const I3D_mesh_object* m_pMesh);
    virtual LS3D_RESULT __stdcall SetParam(int param, uint32_t value);
    virtual uint32_t __stdcall GetParam(int param);
    virtual void __stdcall Reset();
    virtual void __stdcall KillElements();
    virtual void __stdcall TickElements(int tickTime);
    virtual void __stdcall SetOnElements(bool enable);
};