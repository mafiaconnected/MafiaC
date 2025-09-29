#pragma once

#include <I3D/I3D_frame.h>

class I3D_animation_set;
class I3D_pose_set;
class I3D_pose;
class I3D_interpolator;

class I3D_model : public I3D_frame {
  public:
    virtual ~I3D_model() {}
    virtual LS3D_RESULT __stdcall Open(const char* modelName, uint32_t flags = 0, I3D_CALLBACK callback = NULL, void* context = NULL);
    virtual void __stdcall Close();
    virtual void __stdcall AddFrame(I3D_frame* frame);
    virtual LS3D_RESULT __stdcall DeleteFrame(I3D_frame* frame);
    virtual LS3D_RESULT __stdcall SetAnimation(I3D_animation_set* animSet, uint32_t animId, float animBlend, int animOptions);
    virtual I3D_animation_set* __stdcall GetAnimationSet(uint32_t animId);
    virtual void __stdcall StopAnimation(uint32_t animId);
    virtual void __stdcall StartAnimation(uint32_t animId);
    virtual LS3D_RESULT __stdcall SetAnimBlend(uint32_t animId, float blend);
    virtual LS3D_RESULT __stdcall GetAnimBlend(uint32_t animId);
    virtual LS3D_RESULT __stdcall SetAnimTime(uint32_t animId, int time);
    virtual int __stdcall GetAnimTime(uint32_t animId);
    virtual LS3D_RESULT __stdcall SetAnimOptions(uint32_t animId, int options);
    virtual int __stdcall GetAnimOptions(uint32_t animId);
    virtual bool __stdcall AnimIsOn(uint32_t animId);
    virtual void __stdcall Tick2(const int* const);
    virtual LS3D_RESULT __stdcall SetPose(const I3D_pose_set* poseSet);
    virtual LS3D_RESULT __stdcall SetPose(const char* name, I3D_pose* pose);
    virtual LS3D_RESULT __stdcall SetPose(const I3D_frame* frame, I3D_pose* pose);
    virtual LS3D_RESULT __stdcall SetPoseBlend(float blend, const I3D_pose_set* poseSet);
    virtual LS3D_RESULT __stdcall SetPoseBlend(float blend);
    virtual float __stdcall GetPoseBlend();
    virtual void __stdcall ResetAllPoses();
    virtual int __stdcall NumInterpolators();
    virtual I3D_interpolator** __stdcall GetInterpolators();
    virtual LS3D_RESULT __stdcall DuplicateHierarchy(const I3D_model* model);
};
