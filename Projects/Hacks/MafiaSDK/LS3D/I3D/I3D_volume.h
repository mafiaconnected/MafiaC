#pragma once

#include <I3D/I3D_frame.h>

class I3D_volume : public I3D_frame {
  public:
    virtual ~I3D_volume() {}
    virtual void UpdateV() override;
    virtual int GetVolumeType();
};