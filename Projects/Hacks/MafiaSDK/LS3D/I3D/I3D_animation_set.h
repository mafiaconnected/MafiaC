#pragma once

#include <LS3D.h>
#include "I3D_math.h"

class I3D_animation_set {
  public:
    virtual int32_t __stdcall Release();
    virtual LS3D_RESULT __stdcall Open(const char* animFileName, uint32_t flags = 0, I3D_CALLBACK callback = NULL, void* context = NULL);
    virtual uint32_t __stdcall GetEndTime();
    virtual uint32_t __stdcall NumAnimations();
    virtual void* __stdcall GetAnimLink(int index);
    virtual int __stdcall GetAnimIndex(const char* animName);
    virtual uint32_t __stdcall NumPosKeys(uint32_t linkId);
    virtual LS3D_RESULT __stdcall GetPosKey(uint32_t linkId, uint32_t keyId, int& unk, S_vector& pos);
    virtual uint32_t __stdcall NumRotKeys(uint32_t linkId);
    virtual LS3D_RESULT __stdcall GetRotKey(uint32_t linkId, uint32_t keyId, int& unk, S_quat& rot);
    virtual uint32_t __stdcall NumSclKeys(uint32_t linkId);
    virtual LS3D_RESULT __stdcall GetSclKey(uint32_t linkId, uint32_t keyId, int& unk, S_vector& scale);
    virtual uint32_t __stdcall NumNoteKeys(uint32_t linkId);
    virtual LS3D_RESULT __stdcall GetNoteKey(uint32_t linkId, uint32_t keyId, int& unk, int16_t& noteKey);
    virtual LS3D_RESULT __stdcall GetNoteKeyStr(uint32_t linkId, uint32_t keyId, int& unk, const char*& str);
};