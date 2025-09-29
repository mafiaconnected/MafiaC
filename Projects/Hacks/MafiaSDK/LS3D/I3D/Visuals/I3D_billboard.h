#pragma once

#include <Visuals/I3D_object.h>

class I3D_billboard : public I3D_object {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_billboard() {}
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetSpace(bool) override;
    virtual void _stdcall UpdateBoundProc() override;
    virtual void m_AddFGroups() override;
    virtual void m_RenderFGroup(void* fgStruct) override;
    virtual void __stdcall SetMesh(const I3D_mesh_object*, bool);
};