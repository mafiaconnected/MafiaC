#pragma once

#include <Visuals/I3D_object.h>

class I3D_sector;

class I3D_part_element_base : public I3D_object {
  public:
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_frame* frame) override;
    virtual ~I3D_part_element_base() {}
    virtual void __stdcall SetSpace(bool) override;
    virtual void m_AddFGroups() override;
    virtual void m_RenderFGroup(void* fgStruct) override;
    virtual void __stdcall SetSector(I3D_sector* sector);
    virtual I3D_sector* __stdcall GetSector();
};