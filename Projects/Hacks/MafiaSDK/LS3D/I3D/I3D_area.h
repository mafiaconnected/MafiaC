#pragma once

#include <I3D/I3D_frame.h>

class I3D_sound;

class I3D_area : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall LinkTo(I3D_frame* parent, uint32_t flags) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_area() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual LS3D_RESULT __stdcall Build(const S_vector*, uint32_t, uint32_t);
    virtual LS3D_RESULT __stdcall Build(const S_vector*, uint32_t, const uint16_t*, uint32_t, uint32_t);
    virtual void __stdcall AddSound(I3D_sound* sound);
    virtual LS3D_RESULT __stdcall DeleteSound(I3D_sound* sound);
    virtual bool __stdcall CheckPoint(const S_vector& v);
    virtual void __stdcall SetSwitchFadeTime(uint32_t time);
    virtual uint32_t __stdcall GetSwitchFadeTime();
};