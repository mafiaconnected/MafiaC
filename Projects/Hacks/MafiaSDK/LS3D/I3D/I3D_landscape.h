#pragma once

#include <I3D/I3D_frame.h>

class I3D_landscape : public I3D_frame {
  public:
    virtual ~I3D_landscape() {}
    virtual void __stdcall Open(const char* fileName, uint32_t flags);
    virtual float __stdcall GetHeight(int, int);
};