#pragma once

#include <Visuals/I3D_object.h>

class I3D_joint;

class I3D_singlemesh : public I3D_object {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_singlemesh() {}
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetSpace(bool) override;
    virtual void _stdcall UpdateBoundProc() override;
    virtual void m_AddFGroups() override;
    virtual void __stdcall UpdateVertices(int) override;
    virtual void __stdcall UpdateVertices() override;
    virtual I3D_bbox* __stdcall GetJointBBox(int jointId);
    virtual POS_NORM* __stdcall GetVertices(int lodId);
    virtual I3D_joint* __stdcall GetVertexJoint(int lodId, int vertexId);
    virtual void UpdateLod(int lodId);
};