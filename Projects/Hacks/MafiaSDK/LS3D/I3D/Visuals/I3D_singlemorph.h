#pragma once

#include <Visuals/I3D_singlemesh.h>

class I3D_singlemorph : public I3D_singlemesh {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_singlemorph() {}
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void _stdcall UpdateBoundProc() override;
    virtual void __stdcall SetMesh(const I3D_mesh_object*, bool) override;
    virtual void UpdateLod(int lodId) override;

    virtual int __stdcall NumRegions() override;
    virtual int __stdcall SetWeight(int, int, float) override;
    virtual float __stdcall GetWeight(int, int) override;
};