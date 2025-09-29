#pragma once

#include <I3D/I3D_frame.h>
#include <ISND/ISND_source.h>

enum I3D_SOUNDTYPE : uint32_t { SOUND_NULL, SOUND_POINT, SOUND_SPOT, SOUND_AMBIENT, SOUND_VOLUME, SOUND_POINTAMBIENT, SOUND_MAXTYPES };

class I3D_sound : public I3D_frame {
  public:
    virtual void __stdcall SetOn(bool enable) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_sound() {}
    virtual void LoadImageA() override;
    virtual LS3D_RESULT __stdcall Open(const char* fileName, uint32_t flags, void* unk1, void* unk2);
    virtual bool __stdcall IsPlaying();
    virtual void __stdcall SetSoundType(I3D_SOUNDTYPE soundType);
    virtual void __stdcall SetRange(float innerRadius, float outerRadius, float innerFalloff, float outerFalloff);
    virtual void __stdcall SetCone(float min, float max);
    virtual void __stdcall SetOutVol(float vol);
    virtual void __stdcall SetVolume(float vol);
    virtual void __stdcall SetLoop(bool enable);
    virtual void __stdcall SetCurrTime(uint32_t time);
    virtual void __stdcall SetFrequency(float freq);
    virtual void __stdcall GetRange(float& innerRadius, float& outerRadius, float& innerFalloff, float& outerFalloff);
    virtual void __stdcall GetCone(float& min, float& max);
    virtual uint32_t __stdcall GetCurrTime();
    virtual uint32_t __stdcall GetPlayTime();
    virtual void __stdcall SetOn2(bool, bool);
    virtual void __stdcall SetVelocity(S_vector velocity);

    inline I3D_SOUNDTYPE GetSoundType() const { return *(I3D_SOUNDTYPE*)((int)this + 412); }
    inline float GetVolume() const { return *(float*)((int)this + 396); }
    inline float GetOutVol() const { return *(float*)((int)this + 392); }
    inline bool IsLoop() const { return *(bool*)((int)this + 356); }

    inline size_t NumSoundSectors() const { return *(size_t*)((int)this + 468); }
    inline I3D_sector** GetSoundSectors() const { return *(I3D_sector***)((int)this + 460); }

    //inline ISND_source* GetSoundSource() const { return *(ISND_source**)((int)this + 352); }
};