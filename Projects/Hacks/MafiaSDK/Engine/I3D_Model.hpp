/*
Copyright 2018 Dávid Svitana

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _I3D_Model_H_
#define _I3D_Model_H_

namespace MafiaSDK
{
    class I3D_Model : public I3D_Frame
    {
    public:
        ~I3D_Model(void);
        virtual void nullsub_7(void);
        virtual void nullsub_8(void);
        virtual void __stdcall LoadImageA(void);
        virtual enum  LS3D_RESULT __stdcall Open(char const *, unsigned int, void(__stdcall *)(enum  LS3D_CALLBACK_MESSAGE, unsigned int, unsigned int, void *), void *);
        virtual void __stdcall Close(void);
        virtual void __stdcall AddFrame(class I3D_Frame *);
        virtual enum  LS3D_RESULT __stdcall DeleteFrame(class I3D_Frame *);
        virtual enum  LS3D_RESULT __stdcall SetAnimation(class I3D_animation_set *, unsigned int, float, enum  I3D_ANIMOPTIONS);
        virtual class I3D_animation_set * __stdcall GetAnimationSet(unsigned int)const;
        virtual void __stdcall StopAnimation(unsigned int);
        virtual void __stdcall StartAnimation(unsigned int);
        virtual enum  LS3D_RESULT __stdcall SetAnimBlend(unsigned int, float);
        virtual float __stdcall GetAnimBlend(unsigned int)const;
        virtual enum  LS3D_RESULT __stdcall SetAnimTime(unsigned int, int);
        virtual int __stdcall GetAnimTime(unsigned int)const;
        virtual enum  LS3D_RESULT __stdcall SetAnimOptions(unsigned int, enum  I3D_ANIMOPTIONS);
        virtual enum  I3D_ANIMOPTIONS __stdcall GetAnimOptions(unsigned int)const;
        virtual bool __stdcall AnimIsOn(unsigned int)const;
        virtual void __stdcall Tick(int const * const);
        virtual enum  LS3D_RESULT __stdcall SetPose(class I3D_pose_set * const);
        virtual enum  LS3D_RESULT __stdcall SetPose(char const *, class I3D_pose *);
        virtual enum  LS3D_RESULT __stdcall SetPose(class I3D_Frame * const, class I3D_pose *);
        virtual enum  LS3D_RESULT __stdcall SetPoseBlend(float, class I3D_pose_set * const);
        virtual enum  LS3D_RESULT __stdcall SetPoseBlend(float);
        virtual float __stdcall GetPoseBlend(void)const;
        virtual void __stdcall ResetAllPoses(void);
        virtual int __stdcall NumInterpolators(void)const;
        virtual class I3D_interpolator * const * __stdcall GetInterpolators(void)const;
        virtual enum  LS3D_RESULT __stdcall DuplicateHierarchy(class I3D_model * const);
    };
}

#endif
