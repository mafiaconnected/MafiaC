#pragma once

#include <LS3D.h>
#include <IGraph/ITexture.h>

class I3D_material {
  public:
    virtual uint32_t __stdcall Release();
    virtual void __stdcall SetAmbient(const S_vector& ambient);
    virtual void __stdcall SetDiffuse(const S_vector& diffuse);
    virtual void __stdcall SetEmissive(const S_vector& emissive);
    virtual void __stdcall SetAlpha(float alpha);
    virtual void __stdcall SetTexture(ITexture* tex);
    virtual LS3D_RESULT __stdcall Duplicate(I3D_material* mat);
    virtual void __stdcall Set2Sided();
    virtual void __stdcall UpdateTexture();
    virtual void __stdcall UpdateEnvTexture();
    virtual void __stdcall SetEnvTexture(ITexture* tex, uint32_t type, uint32_t mode, float alpha);
    virtual void __stdcall SetEnvTexture(ITexture* tex);
    virtual bool __stdcall IsCkey();
    virtual bool __stdcall IsAnim();
    virtual LS3D_RESULT __stdcall SetEnvType(uint32_t type);
    virtual LS3D_RESULT __stdcall SetEnvMode(uint32_t mode);
    virtual LS3D_RESULT __stdcall SetEvnTile(uint32_t tile);
    virtual LS3D_RESULT __stdcall SetEnvAlpha(float alpha);
    virtual ~I3D_material(){};
};