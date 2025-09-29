#pragma once

#include <I3D/I3D_frame.h>

class I3D_shadow : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall LinkTo(I3D_frame* parent, uint32_t flags) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_shadow() {}
    virtual void UpdateV() override;
    virtual LS3D_RESULT __stdcall Open(const char* modelName, uint32_t flags = 0, I3D_LOAD_CB_PROC* callback = NULL, void* context = NULL);
    virtual void __stdcall Close();
    virtual LS3D_RESULT __stdcall SetTextureSize(int x, int y);
    virtual void __stdcall SetRange(float range);
    virtual LS3D_RESULT __stdcall SetMaxTextureDistance(float distance);
    virtual void __stdcall SetShadowLock(int lock);
    virtual void __stdcall SetReverseFactor(float factor);
    virtual void __stdcall SetColor(const S_vector& color);
    virtual void __stdcall SetOrthogonal(bool enable);
};