#pragma once

#include <I3D/I3D_frame.h>

class I3D_light;
class I3D_sound;

class I3D_sector : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall SetName(const char* name) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_sector() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall AddLight(I3D_light* light);
    virtual LS3D_RESULT __stdcall DeleteLight(I3D_light* light);
    virtual void __stdcall AddSound(I3D_sound* sound);
    virtual LS3D_RESULT __stdcall DeleteSound(I3D_sound* sound);
    virtual LS3D_RESULT __stdcall SetSoundData(int, uint32_t);
    virtual uint32_t __stdcall GetSoundData(int);
    virtual bool __stdcall CheckPoint(const S_vector& v);
    virtual void __stdcall SetGroupBits(uint32_t);
};