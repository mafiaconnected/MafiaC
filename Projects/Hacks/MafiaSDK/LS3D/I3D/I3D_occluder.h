#pragma once

#include <I3D/I3D_frame.h>

class I3D_occluder : public I3D_frame {
  public:
    virtual LS3D_RESULT __stdcall LinkTo(I3D_frame* parent, uint32_t flags);
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_occluder() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual LS3D_RESULT __stdcall Build(const S_vector* vertices, uint32_t numVertices, uint32_t flags);
    virtual LS3D_RESULT __stdcall Build(const S_vector* vertices, uint32_t numVertices, const uint16_t* indices, uint32_t numIndices, uint32_t flags);
};