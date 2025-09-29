#pragma once
#include "I3D_math.h"
#include "I3D_frame.h"

typedef int I3D_ANIMOPTIONS;
typedef int SCN_CALLBACK_MESSAGE;
typedef int COLL_ENUMRET;
typedef int I3D_COLLISION;
typedef int WS_PARAM;

class I3D_camera;
class I3D_sector;
class I3D_interpolator;
class I3D_animation_set;
class I3D_dummy;
class I3D_visual;

class I3D_scene : public I3D_frame {
  public:
    virtual void __stdcall SetDir(const S_vector& dir, float roll) override;
    virtual void __stdcall Tick(int32_t itime) override;
    virtual void __stdcall Update() override;
    virtual LS3D_RESULT __stdcall LinkTo(I3D_frame* parent, uint32_t flags) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;

    virtual ~I3D_scene() {}
    virtual uint32_t __stdcall SetCallback(uint32_t (*const)(SCN_CALLBACK_MESSAGE, uint32_t, uint32_t), uint32_t);
    virtual void __stdcall Render();
    virtual I3D_frame* __stdcall FindFrame(char const* pSzFrameName, uint32_t uFlags);
    virtual void __stdcall AddFrame(I3D_frame* pFrame);
    virtual LS3D_RESULT __stdcall DeleteFrame(I3D_frame* pFrame);
    virtual void __stdcall SetGlobalColor(const S_vector& vColor);
    virtual float __stdcall GetLightness(S_vector const&, I3D_sector*, uint32_t, I3D_frame* const);
    virtual void __stdcall SetActiveCamera(I3D_camera* pCamera);
    virtual LS3D_RESULT __stdcall UnmapScreenPoint(int32_t iX, int32_t iY, S_vector& vPos, S_vector& vDir);
    virtual LS3D_RESULT __stdcall TransformPoints(S_vector const*, S_vector4*, uint32_t);
    virtual bool __stdcall TestVisibility(I3D_frame* pFrame, I3D_camera* pCamera);
    virtual LS3D_RESULT __stdcall SetAnimation(I3D_animation_set*, uint32_t, float, I3D_ANIMOPTIONS);
    virtual I3D_animation_set* __stdcall GetAnimationSet(uint32_t uIndex);
    virtual void __stdcall StopAnimation(uint32_t uIndex);
    virtual LS3D_RESULT __stdcall SetAnimBlend(uint32_t uIndex, float fRatio);
    virtual float __stdcall GetAnimBlend(uint32_t iIndex);
    virtual LS3D_RESULT __stdcall SetAnimTime(uint32_t uIndex, uint32_t uTime);
    virtual uint32_t __stdcall GetAnimTime(uint32_t uIndex);
    virtual bool __stdcall AnimIsOn(uint32_t uIndex);
    virtual uint32_t __stdcall NumInterpolators();
    virtual I3D_interpolator* __stdcall GetInterpolators();
    virtual void __stdcall AddTickFrame(const I3D_frame* pFrame);
    virtual LS3D_RESULT __stdcall DeleteTickFrame(const I3D_frame* pFrame);
    virtual LS3D_RESULT __stdcall CreateSector();
    virtual I3D_sector* __stdcall GetSector(S_vector const&, I3D_sector**, int);
    virtual I3D_sector* __stdcall GetSector2(S_vector const&, I3D_sector*);
    virtual void __stdcall SetBackdropRange(float fNear, float fFar);
    virtual LS3D_RESULT __stdcall SetFrameSectorPos(I3D_frame*, S_vector const&);
    virtual LS3D_RESULT __stdcall Open(char const*, uint32_t, void(__stdcall*)(LS3D_CALLBACK_MESSAGE, uint32_t, uint32_t, void*), void*);
    virtual void __stdcall Close();
    virtual I3D_frame* __stdcall TestColGeo1(S_vector const&,
                                             S_vector const&,
                                             uint32_t,
                                             S_vector*,
                                             S_vector*,
                                             COLL_ENUMRET (*)(S_vector const&, float, I3D_frame*, void*),
                                             void*,
                                             int (*)(S_vector const&, float, I3D_frame*, void*, void*),
                                             int);
    virtual I3D_frame* __stdcall TestColGeo(S_vector const&,
                                            S_vector const&,
                                            uint32_t,
                                            float*,
                                            S_vector*,
                                            COLL_ENUMRET (*)(S_vector const&, float, I3D_frame*, void*),
                                            void*,
                                            int (*)(S_vector const&, float, I3D_frame*, void*, void*),
                                            I3D_frame*,
                                            int);
    virtual I3D_frame* __stdcall TestColHierarchy(
        S_vector const&, S_vector const&, I3D_frame*, I3D_COLLISION&, uint32_t, I3D_frame*, int, COLL_ENUMRET (*)(S_vector const&, float, I3D_frame*, void*));
    virtual I3D_frame* __stdcall TestColSphere(
        S_vector const&, S_vector const&, float, uint32_t, S_vector*, I3D_frame*, COLL_ENUMRET (*)(S_vector const&, float, I3D_frame*, void*));
    virtual I3D_frame* __stdcall TestColSphereHierarchy(
        S_vector const&, S_vector const&, float, I3D_frame*, uint32_t, S_vector*, I3D_frame*, COLL_ENUMRET (*)(S_vector const&, float, I3D_frame*, void*));
    virtual I3D_frame* __stdcall TestColSphereVisual(S_vector const&, S_vector const&, float, I3D_visual*, uint32_t, S_vector*);
    virtual float __stdcall SphereToSphereD(float, S_vector const&, S_vector const&, float, S_vector const&, S_vector const&);
    virtual void __stdcall SetSpace(bool bSpace);
    virtual void __stdcall SetWeatherSystemParam(WS_PARAM, uint32_t);
    virtual uint32_t __stdcall GetWeatherSystemParam(WS_PARAM);
    virtual void __stdcall WeatherSystemReset();
    virtual void __stdcall WeatherSystemAddDummy(I3D_dummy*);
    virtual void __stdcall WeatherSystemDelDummy(I3D_dummy*);
    virtual uint32_t __stdcall WeatherSystemGetDummyCnt();
    virtual I3D_dummy* __stdcall WeatherSystemGetDumies();
    virtual LS3D_RESULT __stdcall SetViewport(uint32_t uX, uint32_t uY, uint32_t uWidth, uint32_t uHeight);

    inline S_vector GetClearColor() { return *(S_vector*)((int)this + 544); }
    inline void GetBackdropRange(float& nearRange, float& farRange) {
        nearRange = *(float*)((int)this + 588);
        farRange = *(float*)((int)this + 592);
    }

    inline I3D_sector* GetPrimarySector() { return *(I3D_sector**)((int)this + 528); }

    inline I3D_sector* GetBackdropSector() { return *(I3D_sector**)((int)this + 532); }
};