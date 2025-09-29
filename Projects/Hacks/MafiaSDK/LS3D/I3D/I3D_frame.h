#pragma once
#include "C_str.hpp"
#include "LS3D.h"

enum I3D_FRAME_FAGS { FRMFLAGS_ON = 0x1, FRMFLAGS_VALID_WMATRIX = 0x20, FRMFLAGS_IN_FFTREE = 0x20000000, FRMFLAGS_UNK1 = 0x80000000 };

class I3D_frame {
  public:
    virtual int32_t __stdcall Release();
    virtual void __stdcall SetWorldPos(const S_vector& vPos);
    virtual void __stdcall SetWorldPosDir(const S_vector& vPos, const S_vector& vDir, float fRoll);
    virtual void __stdcall SetDir(const S_vector& vAxis, float fRoll);
    virtual void __stdcall SetWorldDir(const S_vector& vAxis, float fRoll);
    virtual void __stdcall Tick(int32_t iTime);
    virtual void __stdcall Update();
    virtual void __stdcall SetCallback(I3D_CALLBACK pCallback, uint32_t uUnk);
    virtual LS3D_RESULT __stdcall SetProperty(const char* pSzProperty);
    virtual void __stdcall SetOn(bool bOn);
    virtual LS3D_RESULT __stdcall SetName(const char* pSzName);
    virtual LS3D_RESULT __stdcall LinkTo(I3D_frame* pParent, uint32_t uFlags = 0);
    virtual I3D_frame* __stdcall GetChild(uint32_t uIndex);
    virtual LS3D_RESULT __stdcall EnumFrames(I3D_ENUMPROC* pProc, uint32_t uUser, uint32_t uFlags = ENUMF_ALL, const char* pSzMask = NULL);
    virtual I3D_frame* __stdcall FindChildFrame(const char* pSzFrameName, uint32_t uFlags = ENUMF_ALL);
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* pFrame);

    // TODO(DavoSK): find out when compiler is generating scalar deleting destructor
    // in original game destructor is generated here after 'Duplicate' and as scalar deleting constructor
    // here MSVC generates vector deleting consturctor this can be a problem
    virtual ~I3D_frame();

    // NOTE(DavoSK): theese 3 following functions are __thiscall calling convention (weird)
    virtual void UpdateV();
    virtual void SaveImage();
    virtual void LoadImageA();

    inline void AddRef() { m_iRefCount++; }

    inline I3D_frame* GetParent() const { return m_pPrev; }

    inline I3D_FRAME_TYPE GetType() const { return m_eFrameType; }

    inline const char* GetName() const { return m_pSzName; }

    inline const S_matrix& GetWorldMat() {
        if(!(m_uFlags & FRMFLAGS_VALID_WMATRIX)) { UpdateWMatrixProc(); }

        return m_mWorldMat;
    }

    inline void SetPos(const S_vector& vPos) {
        if(!(m_uFlags & 0x80u)) {
            m_mLocalMat.e[12] = vPos.x;
            m_mLocalMat.e[13] = vPos.y;
            m_mLocalMat.e[14] = vPos.z;
            m_uFlags = m_uFlags & 0xBFFFFEDF | 0x40000000;
        }
    }

    inline const S_vector& GetPos() { return *(S_vector*)&m_mLocalMat.e[12]; }

    inline const S_vector GetWorldPos() {
        S_matrix mWorldMat = GetWorldMat();
        return *(S_vector*)&mWorldMat.e[12];
    }

    inline const S_vector& GetDir() { return *(S_vector*)&m_mLocalMat.m_31; }

    inline void SetRot(const S_quat& aRot) {
        m_aRot = aRot;
        m_aRot.Normalize();
        m_uFlags = m_uFlags & 0xfffffecb | 0x40000008;
    }

    const S_quat& GetRot() const { return m_aRot; }

    inline void SetScale(const S_vector& vScale) {
        m_aScale = vScale;

        if(!(m_uFlags & 8)) { m_aRot.Make(m_mLocalMat); }

        m_uFlags = m_uFlags & 0xBFFFFEC3 | 0x40000008;
    }

    const S_vector& GetScale() const { return m_aScale; }

    inline uint32_t NumChildren() const { return m_uChildrenCount; }

    inline bool IsOn() const { return m_uFlags & 1; }

  private:
    __declspec(dllexport) void __stdcall UpdateHRBoundVol();
    __declspec(dllexport) void __stdcall UpdateHRBoundVolW();
    __declspec(dllexport) void __thiscall UpdateWMatrixProc();

  public:
    // protected: // Note(Sev3n): WiP
    int32_t m_iRefCount;
    uint32_t _pad0;
    char* m_pSzProperties;
    S_matrix m_mWorldMat;
    S_matrix m_mLocalMat;
    S_vector m_aScale;
    S_quat m_aRot;
    uint32_t m_uFlags;
    I3D_bound_volume m_sLocalBVol;
    I3D_bound_volume m_sWorldBVol;
    char* m_pSzName;
    char* m_pSzModelName;
    I3D_CALLBACK* m_pCallback;
    I3D_frame* m_pOwner;
    I3D_FRAME_TYPE m_eFrameType;
    uint32_t _pad1;
    uint32_t _pad2;
    uint32_t m_uChildrenCount;
    I3D_frame* m_pPrev;
    I3D_frame* m_pNext;
    I3D_frame* m_pPrevPrev;
    I3D_frame* m_pNextNext;
    char _pad3[16];
    I3D_bbox m_uDummyBBOX;
    char _pad4[42];
};

static const char* g_pSzFrameNames[] = {"NULL",
                                        "VISUAL",
                                        "LIGHT",
                                        "CAMERA",
                                        "SOUND",
                                        "SECTOR",
                                        "DUMMY",
                                        "TARGET",
                                        "USER",
                                        "MODEL",
                                        "JOINT",
                                        "VOLUME",
                                        "OCCLUDER",
                                        "SCENE",
                                        "AREA",
                                        "SHADOW",
                                        "LANDSACPE",
                                        "EMITOR",
                                        "LAST"};