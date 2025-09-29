#pragma once

#include <I3D/I3D_frame.h>

enum I3D_LIGHTTYPE : uint32_t { LIGHT_NULL, LIGHT_POINT, LIGHT_SPOT, LIGHT_DIRECTIONAL, LIGHT_AMBIENT, LIGHT_FOG, LIGHT_POINTAMBIENT, LIGHT_POINTFOG, LIGHT_LAYEREDFOG, LIGHT_MAXTYPES };

class I3D_sector;

class I3D_light : public I3D_frame {
  public:
    virtual void __stdcall SetOn(bool enable) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_light() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetLightType(I3D_LIGHTTYPE type);
    virtual void __stdcall SetColor(float r, float g, float b);
    virtual void __stdcall SetColor2(const S_vector& color);
    virtual void __stdcall SetPower(float power);
    virtual void __stdcall SetMode(uint32_t mode);
    virtual void __stdcall SetRange(float min, float max);
    virtual void __stdcall SetCone(float min, float max);

    inline I3D_LIGHTTYPE GetLightType() const { return *(I3D_LIGHTTYPE*)((int)this + 340); }
    inline S_vector GetColor() const { return *(S_vector*)((int)this + 348); }
    inline float GetPower() const { return *(float*)((int)this + 344); }
    inline uint32_t GetMode() const { return *(uint32_t*)((int)this + 336); }
    inline void GetRange(float& nearRange, float& farRange) const {
        int type = GetLightType();

        if(type == LIGHT_FOG || type == LIGHT_POINTAMBIENT || type == LIGHT_POINTFOG) {
            nearRange = *(float*)((int)this + 412) / 0.0099999998;
            farRange = *(float*)((int)this + 464) / 0.0099999998;
        } else {
            nearRange = *(float*)((int)this + 412) / *(float*)((int)this + 276);
            farRange = *(float*)((int)this + 464) / *(float*)((int)this + 276);
        }
        
    }
    inline void GetCone(float& nearAngle, float& farAngle) const {
        nearAngle = *(float*)((int)this + 368);
        farAngle = *(float*)((int)this + 372);
    }

    inline size_t NumLightSectors() const { return *(size_t*)((int)this + 328); }
    inline I3D_sector** GetLightSectors() const { return *(I3D_sector***)((int)this + 320); }
};