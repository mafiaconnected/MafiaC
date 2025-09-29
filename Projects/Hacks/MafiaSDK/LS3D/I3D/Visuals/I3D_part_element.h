#pragma once

#include <Visuals/I3D_part_element_base.h>

class I3D_part_element : public I3D_part_element_base {
  public:
    virtual void __stdcall Tick(int tickTime) override;
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_part_element();
};