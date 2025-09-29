#pragma once

#include <I3D/I3D_visual.h>
#include <IGraph/ITexture.h>

class I3D_material;

class I3D_projector : public I3D_visual {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_projector() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void m_AddFGroups() override;
    virtual void m_RenderFGroup(void* fgStruct) override;
    virtual void __stdcall SetTexture(const ITexture* texture);
    virtual void __stdcall SetMaterial(const I3D_material* material);
    virtual void __stdcall SetOrthogonal(bool enable);
    virtual void __stdcall SetAlpha(float alpha);
    virtual void __stdcall SetReverseFactor(float factor);
    virtual void __stdcall SetColor(const S_vector& color);
    virtual void __stdcall SetMode(int mode);
    virtual void __stdcall SetProjectorLock(int lockMode);
    virtual void __stdcall MakeProjection(uint32_t flags, I3D_frame* frame);
};