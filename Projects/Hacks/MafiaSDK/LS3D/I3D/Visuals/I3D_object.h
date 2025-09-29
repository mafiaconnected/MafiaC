#pragma once

#include <I3D/I3D_visual.h>
#include <I3D/I3D_mesh_object.h>

class I3D_object : public I3D_visual {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_object() {}
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetSpace(bool) override;
    virtual void m_AddFGroups() override;
    virtual void m_RenderFGroup(void* fgStruct) override;
    virtual void __stdcall SetMesh(const I3D_mesh_object*, bool);
    virtual LS3D_RESULT __stdcall DetachMesh();
    virtual void __stdcall UpdateVertices(int);
    virtual void __stdcall UpdateVertices2();
    virtual void __stdcall SetLightness(float lightness);
    virtual void __stdcall SetTransparency(float transparency);

  protected:
    uint8_t pad[88];
    I3D_mesh_object* m_pMesh;
};