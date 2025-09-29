#pragma once
#include "LS3D.h"
#include <I3D/I3D_math.h>
#include "C_list.h"
#include "Memory.h"

enum I3DSTATSINDEX;

class I3D_scene;
class I3D_visual;
class I3D_frame;
class I3D_material;
class I3D_anim_material;
class ISND_sound;

struct I3D_triface;
struct I3D_text_coor;

class I3D_face_group;
class I3D_mesh_level;
class I3D_mesh_object;
class I3D_animation_set;
class I3D_pose;
class I3D_pose_set;

class I3D_driver {
  public:
    /* public interface */
    virtual LS3D_RESULT __stdcall Init(uint32_t iUnk);
    virtual LS3D_RESULT __stdcall Close();
    virtual void __stdcall Tick(uint32_t uDeltaTime);
    virtual uint32_t __stdcall GetTickTime() const;
    virtual LS3D_RESULT __stdcall GetStats(I3DSTATSINDEX eStatIndex, void* pOutStats);
    virtual LS3D_RESULT __stdcall SetState(I3D_RENDERSTATE eRenderState, int iRenderState);
    virtual int __stdcall GetState(I3D_RENDERSTATE eRenderState) const;
    virtual LS3D_RESULT __stdcall Render(I3D_scene* const pScene);
    virtual void __stdcall DrawPoint(const S_vector& vPos, const S_vector& vColor, uint32_t uAlpha);
    virtual void __stdcall DrawPoint2D(int iX, int iY, const S_vector& vColor, uint32_t uAlpha);
    virtual void __stdcall DrawLine(const S_vector& vP1, const S_vector& vP2, const S_vector& vColor, uint32_t uAlpha);
    virtual void __stdcall DrawLines(
        const S_vector* pPoints, uint32_t uPointsCnt, uint16_t const* pIndices, uint32_t uIndicesCnt, struct S_vector const& vColor, uint32_t uUnk, uint32_t uAlpha);
    virtual LS3D_RESULT __stdcall DrawTriangles(void const* pVertices,
                                                uint32_t uVerticesCnt,
                                                uint16_t const* pIndices,
                                                uint32_t uIndicesCnt,
                                                S_vector const& vColor,
                                                uint32_t uAlpha,
                                                uint32_t uTextCoordCnt,
                                                I3D_text_coor* pTextCoords);
    virtual int __stdcall DrawUnk(const S_vector& vPos, float fUnk, const S_vector& vColor, int iUnk, uint8_t uAlpha);
    virtual void __stdcall DrawTextA(const S_vector& vPos, const char* pSzText, uint8_t uAlpha, float fTextSize);
    virtual void __stdcall DrawText2D(int iWidth, int iHeight, const char* pSzText, uint8_t uAlpha, float fTextSize);
    virtual void __stdcall DrawSprite(const S_vector& vPos, I3D_material* const pMaterial, uint8_t uAlpha, float fSize);
    virtual void __stdcall DrawBox(const I3D_bbox& bbox, const struct S_vector& vPos, const S_vector& vColor, uint8_t uAlpha);
    virtual void __stdcall DrawSphere(const S_matrix& matrix, const I3D_bsphere& sphere, const S_vector& vColor, uint8_t uAlpha);
    virtual I3D_visual* __stdcall CreateVisual(I3D_VISUAL_TYPE eVisualType);
    virtual I3D_frame* __stdcall CreateFrame(I3D_FRAME_TYPE eFrameType);
    virtual I3D_material* __stdcall CreateMaterial();
    virtual I3D_anim_material* __stdcall CreateAnimMaterial();
    virtual I3D_triface* __stdcall CreateTrifaceArray(int iCnt);
    virtual I3D_face_group* __stdcall CreateFaceGroup();
    virtual I3D_mesh_level* __stdcall CreateMeshLevel();
    virtual I3D_mesh_object* __stdcall CreateMesh();
    virtual I3D_animation_set* __stdcall CreateAnimationSet();
    virtual I3D_pose* __stdcall CreatePose();
    virtual I3D_pose_set* __stdcall CreatePoseSet();
    virtual const char* __stdcall FrameType2String(I3D_FRAME_TYPE eFrameType);
    virtual const char* __stdcall VisualType2String(I3D_VISUAL_TYPE eVisualType);
    virtual LS3D_RESULT __stdcall EnumVisualTypes(I3D_ENUMVISUALPROC* pCallback, bool bUnk) const;
    virtual I3D_frame* __stdcall FindFrameFast(I3D_scene* const pScene, const char* pSzFrameName) const;
    virtual void __stdcall OptimalizeFastFindData();
    virtual LS3D_RESULT __stdcall EnumFFTree(I3D_ENUMVISUALPROC* pCallback, uint32_t uUnk) const;
    virtual void __stdcall DeleteFromSceneTickList(I3D_frame* pFrame);
    virtual void __stdcall DeleteFromAllShadows(I3D_frame* pFrame);
    virtual void __stdcall DeleteFromAllTargets(I3D_frame* pFrame);
    virtual void __stdcall sub_100658F0(void);
    virtual void __stdcall nullsub_9(void);

  private:
    friend class ISND_driver;
    uint8_t pad0[10];
    C_list<ISND_sound*> m_CreatedSounds;
};

extern "C" {
extern __declspec(dllexport) I3D_driver* I3DGetDriver();
}