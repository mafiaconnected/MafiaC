#pragma once

#include <I3D/I3D_visual.h>

class I3D_land_patch : public I3D_visual {
  public:
    virtual ~I3D_land_patch() {}
    virtual void m_RenderFGroup(void* fgStruct) override;
    virtual void m_Init(int, int, int, int, uint8_t*);
    virtual void m_Reset();
    virtual void m_Tessellate();
    virtual void m_Render();
    virtual void m_ComputeVariance();
    virtual void m_Split(void*);
    virtual void m_RecursTessellate(void*, int, int, int, int, int, int, int);
    virtual void m_RecursRender(void*, int, int, int, int, int, int);
    virtual void m_RecursComputeVariance(int, int, int, int, int, int, int, int, int, int);
};