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

#ifndef _I3D_Driver_H_
#define _I3D_Driver_H_

namespace MafiaSDK
{
    constexpr unsigned long I3D_Driver_Const = 0x00647ED8;
	namespace I3D_Driver_Enum
	{
		enum FunctionsAddresses
		{
			Construct = 0x100201F0,
			Destruct = 0x10020760,

		};
	
        enum FrameType
        {
            LIGHT = 2,
            CAMERA, 
            SOUND,
            SECTOR,
            DUMMY,
            TARGET,
            USER,
            MODEL,
            JOINT,
            VOLUME,
            OCCLUDER,
            SCENE,
            AREA,
            SHADOW,
            LANDSACPE,
            EMITOR
        };
    };

    struct I3D_bsphere 
    {
        float offsetX;
        float offsetY;
        float offsetZ;
        float radius;
    };

    struct I3D_bbox
    {
        float unk1;
        float unk2;
        float unk3;
        float unk4;
    };

	class I3D_Driver
	{
	public:
        virtual enum  LS3D_RESULT __stdcall Init(unsigned int);								                    // 0000  0000
        virtual enum  LS3D_RESULT __stdcall Close(void);								                        // 0001  0004
        virtual void __stdcall Tick(int);								                                        // 0002  0008
        virtual int __stdcall GetTickTime(void)const;								// 0003  000C
        virtual enum  LS3D_RESULT __stdcall GetStats(enum  I3DSTATSINDEX, void *);								// 0004  0010
        virtual enum  LS3D_RESULT __stdcall SetState(enum  I3D_RENDERSTATE, int);								// 0005  0014
        virtual int __stdcall GetState(enum  I3D_RENDERSTATE)const;								// 0006  0018
        virtual enum  LS3D_RESULT __stdcall Render(class I3D_scene * const);								// 0007  001C
        virtual void __stdcall DrawPoint(struct S_vector const &, struct S_vector const &, unsigned int);								// 0008  0020
        virtual void __stdcall DrawPoint2D(int, int, struct S_vector const &, unsigned int);								// 0009  0024
        virtual void __stdcall DrawLine(const struct S_vector& a2, const struct S_vector& a3, const struct S_vector& a4, unsigned int a5);  // 0010  0028
        virtual void sub_10016B20(void); // 0011  002C
        virtual void __stdcall DrawLines(struct S_vector const *, unsigned int, unsigned short const *, unsigned int, struct S_vector const &, unsigned int, unsigned int);								// 0012  0030
        virtual enum  LS3D_RESULT __stdcall DrawTriangles(void const *, unsigned int, unsigned short const *, unsigned int, struct S_vector const &, unsigned int, unsigned int, struct I3D_text_coor *);								// 0013  0034
        virtual void __stdcall DrawTextA(struct S_vector const &, char const *, unsigned char, float);								// 0014  0038
        virtual void __stdcall DrawText2D(int, int, char const *, unsigned char, float);								// 0015  003C
        virtual void __stdcall DrawSprite(struct S_vector const &, class I3D_Material * const, unsigned char, float);								// 0016  0040
        virtual void __stdcall DrawBox(I3D_bbox const &, struct S_vector& const, struct S_vector& const, unsigned short);								// 0017  0044
        virtual void __stdcall DrawSphere(struct S_matrix const &,I3D_bsphere const &, struct S_vector const &, unsigned short);								// 0018  0048
        virtual class I3D_visual * __stdcall CreateVisual(enum  I3D_VISUAL_TYPE);								// 0019  004C
        virtual class I3D_Frame * __stdcall CreateFrame(I3D_Driver_Enum::FrameType);								// 0020  0050
        virtual class I3D_Material * __stdcall CreateMaterial(void);								// 0021  0054
        virtual class I3D_anim_material * __stdcall CreateAnimMaterial(void);								// 0022  0058
        virtual struct I3D_triface * __stdcall CreateTrifaceArray(int);								// 0023  005C
        virtual class I3D_face_group * __stdcall CreateFaceGroup(void);								// 0024  0060
        virtual class I3D_mesh_level * __stdcall CreateMeshLevel(void);								// 0025  0064
        virtual class I3D_mesh_object * __stdcall CreateMesh(void);								// 0026  0068
        virtual class I3D_animation_set * __stdcall CreateAnimationSet(void);								// 0027  006C
        virtual class I3D_pose * __stdcall CreatePose(void);								// 0028  0070
        virtual class I3D_pose_set * __stdcall CreatePoseSet(void);								// 0029  0074
        virtual char const * __stdcall FrameType2String(enum  I3D_FRAME_TYPE)const;								// 0030  0078
        virtual char const * __stdcall VisualType2String(enum  I3D_VISUAL_TYPE)const;								// 0031  007C
        virtual enum  LS3D_RESULT __stdcall EnumVisualTypes(void(__stdcall *const)(unsigned int, char const *), bool)const;								// 0032  0080
        virtual class I3D_frame * __stdcall FindFrameFast(class I3D_scene * const, char const *)const;								// 0033  0084
        virtual void __stdcall OptimalizeFastFindData(void);								// 0034  0088
        virtual enum  LS3D_RESULT __stdcall EnumFFTree(enum  I3DENUMRET(__stdcall *)(class I3D_frame *, unsigned int), unsigned int)const;								// 0035  008C
        virtual void __stdcall DeleteFromSceneTickList(class I3D_frame *);								// 0036  0090
        virtual void __stdcall DeleteFromAllShadows(class I3D_frame *);								// 0037  0094
        virtual void __stdcall DeleteFromAllTargets(class I3D_frame *);								// 0038  0098
        virtual void sub_100658F0(void);								// 0039  009C
        virtual void nullsub_9(void);								// 0040  00A0
	};

    I3D_Material* m_GetSysIconFromRes(int res_id)
    {   
        /*unsigned long funcAddress = 0x10016600;
        __asm 
        {
            push res_id
            mov ecx, this
            call funcAddress
        }*/
        return 0;
    }

    extern I3D_Driver* I3DGetDriver(); 
};

#endif
