#pragma once

#include <I3D/I3D_visual.h>

class I3D_mirror : public I3D_visual {
    virtual void __stdcall Tick(int tickTime) override;
    virtual LS3D_RESULT __stdcall LinkTo(I3D_frame* frame, uint32_t flags) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_mirror() {}
    virtual void UpdateV() override;
    virtual void SaveImage() override;
    virtual void LoadImageA() override;
    virtual void __stdcall SetViewBox(const I3D_bbox& bbox, const S_matrix& matrix);
    virtual void __stdcall SetColor(const S_vector& color);
};