#pragma once

#include <Visuals/I3D_object.h>

class I3D_morph : public I3D_object {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_morph() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetSpace(bool) override;
    virtual void _stdcall UpdateBoundProc() override;
    virtual void m_AddFGroups() override;
    virtual void __stdcall SetMesh(const I3D_mesh_object*, bool) override;

    virtual int __stdcall NumRegions() override;
    virtual int __stdcall SetWeight(int, int, float) override;
    virtual float __stdcall GetWeight(int, int) override;
};