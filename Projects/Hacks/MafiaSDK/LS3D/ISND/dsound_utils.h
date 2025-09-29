#pragma once
#include <dsound.h>
#include <stdio.h>

inline const char* GetDirectSoundErrorString(HRESULT hr) {
    switch (hr) {
    case DS_OK: return "The function completed successfully.";
    case DS_NO_VIRTUALIZATION: return "The call succeeded, but no virtualization effect was obtained.";
    case DSERR_ACCESSDENIED: return "The call failed because the user did not have the required priority level.";
    case DSERR_ALLOCATED: return "The request failed because resources, such as a priority level, were already in use by another caller.";
    case DSERR_ALREADYINITIALIZED: return "The object is already initialized.";
    case DSERR_BADFORMAT: return "The specified wave format is not supported.";
    case DSERR_BUFFERLOST: return "The buffer memory has been lost and must be restored.";
    case DSERR_BUFFERTOOSMALL: return "The buffer size is not big enough to hold the requested data.";
    case DSERR_CONTROLUNAVAIL: return "The control (volume, pan, and so on) requested by the caller is not available.";
    case DSERR_DS8_REQUIRED: return "A DirectSound object of class CLSID_DirectSound8 or later is required for the requested functionality.";
    case DSERR_FXUNAVAILABLE: return "The requested effect type is not available.";
    case DSERR_GENERIC: return "An undetermined error occurred inside the DirectSound subsystem.";
    case DSERR_INVALIDCALL: return "This function is not valid for the current state of this object.";
    case DSERR_INVALIDPARAM: return "An invalid parameter was passed to the returning function.";
    case DSERR_NOAGGREGATION: return "The object does not support aggregation.";
    case DSERR_NODRIVER: return "No sound driver is available for use, or the given GUID is not a valid DirectSound device ID.";
    case DSERR_NOINTERFACE: return "The requested COM interface is not available.";
    case DSERR_OBJECTNOTFOUND: return "The requested object was not found.";
    case DSERR_OTHERAPPHASPRIO: return "Another application has a higher priority level, preventing this call from succeeding.";
    case DSERR_OUTOFMEMORY: return "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
    case DSERR_PRIOLEVELNEEDED: return "A cooperative level of DSSCL_PRIORITY or higher is required.";
    case DSERR_SENDLOOP: return "A circular loop of send effects was detected.";
    case DSERR_UNINITIALIZED: return "The IDirectSound8::Initialize method has not been called, or the IDirectSound8::Initialize method failed.";
    case DSERR_UNSUPPORTED: return "The function called is not supported at this time.";
    default:
        static char szBuffer[128] = { 0 };
        sprintf(szBuffer, "Unknown DirectSound error code: 0x%08X", hr);
        return szBuffer;
    }
}

inline int32_t Lin2Log(float val) {
    return (int32_t)(10000.0f / (16 * .69314718) * log(float(val)));
}
