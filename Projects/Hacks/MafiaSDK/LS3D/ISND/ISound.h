#pragma once
/*defines and enums*/
#define ISND_STREAMING 1
#define ISND_NOVOLUME  2
#define ISND_NOPANNING 4
#define ISND_PRELOAD   8     // load into sysmem buffer, use as needed
#define ISND_FREQUENCY 0x10  // frequency adjustable
#define ISND_CARDMEM   0x20  // alloc buffer in hardware
#define ISND_SYSMEM    0x40  // alloc buffer in system memory
#define ISND_LOOP      0x100 // loop playback - for all Play methods, also for Open method in case of streaming source
#define ISND_3DBUFFER  0x200 // alloc buffer with 3D hardware capabilities
#define ISND_NOREWIND  0x400 // when starting play, don't rewind buffer

enum ISND_INIT_FLAGS {
    ISND_NO_EAX                 = 0x10,
    ISND_FREE_3D_CHANNELS       = 0x20, 
    ISND_STEREO_HEADPHONES      = 0x400, 
    ISND_STEREO_SPEAKERS        = 0x800,
    ISND_QUADRAPHONIC_SPEAKERS  = 0x1000, 
    ISND_SUROUND_SOUND_SPEAKERS = 0x2000, 
    ISND_EAX_SUPPORT            = 0x100000,
    ISND_FULL_EAX_SUPPORT       = 0x200000
};

/* structs */
struct ISND_INIT {
    uint32_t uMixFreq;
    uint32_t uNumChan;
    uint32_t uBps;
    uint32_t uFlags;
    HWND hWindow;
};

struct S_wave_format {
    uint32_t uNumChannels;    // number of channels, 1=mono, 2=stereo, 4=quadro
    uint32_t uBytesPerSample; // 1, 2, 4, 8
    uint32_t uSamplesPerSec;  // sound playback frequency
    uint32_t uSize;           // size of data, in bytes
};

/* forward declares */
class ISND_source;
class ISND_sound;
class ISND_3Dsound;
class ISND_3Dlistener;