#ifndef __ENG_3DI_
#define __ENG_3DI_
#pragma once
#include "common.h"
#include "I3D/I3D_math.h"

#define LS3D_VERSION 395

#define I3DAPI __stdcall

class IGraph;
class I3D_frame;
class I3D_driver;
class ISND_driver;

static void* engine_malloc(size_t size) {
    HMODULE ls3df = GetModuleHandleA("LS3DF.dll");
    return ((void*(__cdecl*)(size_t))((int)ls3df + 0x8FBCE))(size);
}

static void engine_free(void* ptr) {
    HMODULE ls3df = GetModuleHandleA("LS3DF.dll");
    ((void(__cdecl*)(void*))((int)ls3df + 0x8F634))(ptr);
}

typedef long LS3D_RESULT;
enum {
    I3D_OK,

    I3DERR_NOFILE = 0x80000001,
    I3DERR_OUTOFMEM = 0x80000002,
    I3DERR_TEXTURESNOTSUPPORTED = 0x80000003,
    I3DERR_BADTEXTURE = 0x80000004,
    I3DERR_GENERIC = 0x80000005,
    I3DERR_OBJECTNOTFOUND = 0x80000006,
    I3DERR_INVALIDPARAM = 0x80000007,
    I3DERR_NOTLOADED = 0x80000008,
    I3DERR_NOTINITIALIZED = 0x80000009,
    I3DERR_FILECORRUPTED = 0x8000000A,
    I3DERR_NOADDITIONALFILE = 0x8000000B,
    I3DERR_CYCLICLINKOFHIERARCHY = 0x8000000C,
    I3DERR_ENUMERATIONCANCELLED = 0x8000000D,
    I3DERR_UNSUPPORTED = 0x8000000E,
    I3DERR_NO3DHARDWARE = 0x8000000F,
    I3DERR_SNDBUFFERLOST = 0x80000010,
    I3DERR_USERBREAK = 0x80000011,
    I3DERR_FILESYSTEMERR = 0x80000012,
    I3DERR_BADFILEVERSION = 0x80000013,
    I3DERR_BADFILETIMESTAMP = 0x80000014,
    I3DERR_LIMITOFENGINEEXCEEDED = 0x80000015
};

//----------------------------
// bounding box defined by 2 boundary points
struct I3D_bbox {
    S_vector min, max;
    I3D_bbox() {}
    I3D_bbox(const S_vector& n, const S_vector& x) {
        min = n;
        max = x;
    }
    inline const S_vector& operator[](int i) const { return (&min)[i]; }

    //----------------------------
    // Make bounding-box 'invalid' - expand min to positive infinity and max to negative infinity.
    // In this state the bounding-box is prepared for extens expansion.
    inline void Invalidate() {
        min = S_vector(1e+16f, 1e+16f, 1e+16f);
        max = S_vector(-1e+16f, -1e+16f, -1e+16f);
    }

    //----------------------------
    // Check if bounding-box is valid.
    inline bool IsValid() const { return (min.x <= max.x); }

    //----------------------------
    // Expand AA bounding-box, defined by 2 extreme points, into a bounding-box defined by
    // 8 corner points.
    void Expand(S_vector bbox_full[8]) const {
        // expand bound-box (2 corner points) to full bbox (8 points)
        bbox_full[0].x = min.x;
        bbox_full[0].y = min.y;
        bbox_full[0].z = min.z;
        bbox_full[1].x = max.x;
        bbox_full[1].y = min.y;
        bbox_full[1].z = min.z;
        bbox_full[2].x = min.x;
        bbox_full[2].y = max.y;
        bbox_full[2].z = min.z;
        bbox_full[3].x = max.x;
        bbox_full[3].y = max.y;
        bbox_full[3].z = min.z;
        bbox_full[4].x = min.x;
        bbox_full[4].y = min.y;
        bbox_full[4].z = max.z;
        bbox_full[5].x = max.x;
        bbox_full[5].y = min.y;
        bbox_full[5].z = max.z;
        bbox_full[6].x = min.x;
        bbox_full[6].y = max.y;
        bbox_full[6].z = max.z;
        bbox_full[7].x = max.x;
        bbox_full[7].y = max.y;
        bbox_full[7].z = max.z;
    }
};

typedef I3D_bbox* PI3D_bbox;
typedef const I3D_bbox* CPI3D_bbox;

//----------------------------
// boundnig sphere
struct I3D_bsphere {
    S_vector pos;
    float radius;
    I3D_bsphere() {}
    I3D_bsphere(const S_vector& p, float r) : pos(p), radius(r) {}
};

typedef I3D_bsphere* PI3D_bsphere;
typedef const I3D_bsphere* CPI3D_bsphere;

//----------------------------
// bounding volume - consisting of minimal box and minimal sphere
struct I3D_bound_volume {
    I3D_bbox bbox;
    I3D_bsphere bsphere;
};

typedef I3D_bound_volume* PI3D_bound_volume;
typedef const I3D_bound_volume* CPI3D_bound_volume;

//----------------------------

enum I3D_FRAME_TYPE {
    FRAME_NULL,
    FRAME_VISUAL,
    FRAME_LIGHT,
    FRAME_CAMERA,
    FRAME_SOUND,
    FRAME_SECTOR,
    FRAME_DUMMY,
    FRAME_TARGET,
    FRAME_USER,
    FRAME_MODEL,
    FRAME_JOINT,
    FRAME_VOLUME,
    FRAME_OCCLUDER,
    FRAME_SCENE,
    FRAME_AREA,
    FRAME_SHADOW,
    FRAME_LANDSACPE,
    FRAME_EMITOR,
    FRAME_LAST
};

enum I3D_VISUAL_TYPE {
    VISUAL_LIT_OBJECT = 0x50414D4C,
    VISUAL_PROJECTOR = 0x4A4F5250,
    VISUAL_SINGLE_MESH = 0x4D474E53,
    VISUAL_MORPH = 0x4850524D,
    VISUAL_PART_ELEMENT_BASE = 0x42545250,
    VISUAL_BILLBOARD = 0x44524242,
    VISUAL_PART_ELEMENT = 0x45545250,
    VISUAL_MIRROR = 0x5252494D,
    VISUAL_LAND_PATCH = 0x50444E4C,
    VISUAL_SINGLE_MORPH = 0x524D4D53,
    VISUAL_LENSFLARE = 0x534E454C,
    VISUAL_OBJECT = 0x5F4A424F
};

//----------------------------
// driver rendering state
// TODO(DavoSK): find out what is matching with LS3DF range of enum seems to be pretty same
// real mapping can be generated from I3D_driver::GetState

enum I3D_RENDERSTATE {
    RS_LINEARFILTER = 0, // linear texture filtering
    RS_MIPMAP = 2, // mipmap texture filtering
    RS_USEZB = 3, // use z-buffer during rendering
    RS_CLEAR = 4, // clear screen/zb before rendering
    RS_DITHER = 7, // dither primitives during rendering
    RS_WIREFRAME = 9, // use wire-rendering
    RS_DRAWBOUNDBOX = 8, // debug draw object bound boxes
    RS_DRAWHRBOUNDBOX = 9, // debug draw hierarchy bound boxes
    RS_DRAWLINKS = 10, // debug draw hierarchy links
    RS_LOADMIPMAP = 11, // build mipmap levels during texture loading
    RS_DRAWMIRRORS = 12, // render mirrors
    RS_USESHADOWS = 13, // render shadows
    RS_DEBUGDRAWSHADOWS = 14, // debug draw shadow textures and volumes
    RS_DRAWSECTORS = 15, // debug draw sectors
    RS_DRAWPORTALS = 16, // debug draw portals
    RS_TEXTUREDITHER = 17, // texture dither during loading
    RS_DRAWVISUALS = 18,
    RS_FOG = 19, // set fog on/off
    RS_LOD_QUALITY = 20, // float number
    RS_DEBUGDRAWSHDRECS = 21, // draw shadow receivers
    RS_TEXTURELOWDETAIL = 22, // scale all loaded textures to 1/4 of size
    RS_DRAWVOLUMES = 23, // debug draw collision volumes
    RS_DRAWLIGHTS = 24, // debug draw lights ranges and cones
    RS_DEBUG_DRAW_MATS = 25, // true to visualize materials
    RS_DRAWSOUNDS = 26, // draw sound ranges and cones
    RS_TEXTURECOMPRESS = 27, // use texture compression if available
    RS_USELMAPPING = 28, // use light-mapping
    RS_DRAWLMTEXTURES = 29, // debug draw light-map textures
    RS_DRAWCAMERAS = 30, // debug draw cameras
    RS_DRAWDUMMYS = 31, // debug draw dummy frames
    RS_DETAILMAPPING = 32, // bool (in bit 0) - use detail mapping (if bit 31 is set, vertex buffers are reset)
    RS_DRAWOCCLUDERS = 33, // debug draw occluders
    RS_DRAWTEXTURES = 34, // draw textures
    RS_LMTRUECOLOR = 35, // use true-color lightmaps
    RS_LMDITHER = 36, // dither highcolor textures
    RS_LOD_SCALE = 37, // float scale (1.0f = default, lower=better LOD, higher=worse LOD)
    RS_DEBUGDRAWBSP = 38, // draw bsp tree polygons
    RS_DRAWJOINTS = 39, // debug draw joints
    RS_DEBUGDRAWDYNAMIC = 40, // draw dynamic volume tree hiearchy
    RS_DEBUGDRAWSTATIC = 41, // draw static frames
    RS_ANISO_FILTERING = 42, // enable anisotropic filtering
    RS_DRAW_COL_TESTS = 43, // display all collision testing by lines
    RS_SOUND_VOLUME = 44, // float global sound volume (0.0 ... 1.0)
    RS_PROFILER_MODE = 45, // int m_Mode (0 = off, 1 = absolute, 2 = self)
    RS_LOD_INDEX = 46, // force automatic LOD, if -1, use default (computed)
    RS_USE_OCCLUSION = 47, // enable occlusion testing (default)
    RS_LM_AA_RATIO = 48, // LightMaps creation - antialias subpixel division (1 = no, 2 = 4x, 3 = 9x, ect)
    RS_ENVMAPPING = 49, // bool (in bit 0) - use environment mapping (if bit 31 is set, vertex buffers are reset)
    RS_DEBUG_INT0 = 50, // debugging purposes
    RS_DEBUG_INT1 = 51,
    RS_DEBUG_INT2 = 52,
    RS_DEBUG_INT3 = 53,
    RS_DEBUG_INT4 = 54,
    RS_DEBUG_INT5 = 55,
    RS_DEBUG_INT6 = 56,
    RS_DEBUG_INT7 = 57,
    RS_DEBUG_FLOAT0 = 58, // debugging purposes
    RS_DEBUG_FLOAT1 = 59,
    RS_DEBUG_FLOAT2 = 60,
    RS_DEBUG_FLOAT3 = 61,
    RS_DEBUG_FLOAT4 = 62,
    /*RS_DEBUG_FLOAT5   = 63,
    RS_DEBUG_FLOAT6        = 64,
    RS_DEBUG_FLOAT7        = 65,
    RS_DEBUG_SHOW_OVERDRAW = 66, // show overdraw (how many times each pixel is drawn), using separate colors*/
    RS_LAST
};

//NOTE: enum frames flags
//TODO: check if its valid in Mafia
#define ENUMF_VISUAL 1
#define ENUMF_LIGHT  2
#define ENUMF_CAMERA 4
#define ENUMF_SOUND  8
#define ENUMF_SECTOR 0x10
#define ENUMF_DUMMY  0x20

#define ENUMF_MODEL       0x80
#define ENUMF_USER        0x100
#define ENUMF_VOLUME      0x200
#define ENUMF_JOINT       0x400
#define ENUMF_OCCLUDER    0x800
#define ENUMF_ALL         0x0ffff
#define ENUMF_WILDMASK    0x10000
#define ENUMF_CASESENS    0x20000
#define ENUMF_NO_BACKDROP 0x40000

//NOTE: enum frames return value
enum I3DENUMRET {
    I3DENUMRET_OK,
    I3DENUMRET_SKIPCHILDREN,
    I3DENUMRET_CANCEL,
};

enum LS3D_CALLBACK_MESSAGE {
    I3DCB_MESSAGE = 2, // const char*, bool still
    I3DCB_DEBUG_MESSAGE = 3, // (const char*, int32_t type) - debug message, types: 0=one-time, 1=console, 2=run-time
    I3DCB_DEBUG_CODE = 4, // uint32_t code, void *data
    I3DCB_ANIM_FINISH = 5, // (0, uint32_t stage)
    I3DCB_ANIM_NOTE = 6, // (I3D_frame*, const I3D_note_callback*)
};

typedef I3DENUMRET I3DAPI I3D_ENUMPROC(I3D_frame*, uint32_t uContext);
typedef I3DENUMRET I3DAPI I3D_ENUMVISUALPROC(uint32_t uVisualType, const char* pSzFriendlyName);
typedef void I3DAPI I3D_CALLBACK(LS3D_CALLBACK_MESSAGE eCallbackMessage, uint32_t uPrm1, uint32_t uPrm2, void* pContext);

enum I3D_LOADMESSAGE {
    CBM_PROGRESS = 0, //(float progress, dword status); status: 0=beg, 1=in, 2=end
    CBM_ERROR = 1, //(const char *msg, int importance (0=max, 1=warning, ...))
    CBM_LOG = 2, //(const char *msg)
};

//----------------------------
// Loader - callback function.
// If the returned value for CBM_PROGRESS message is true, the loading is cancelled,
// and the loader method returns I3DERR_CANCELED.
typedef bool I3DAPI I3D_LOAD_CB_PROC(I3D_LOADMESSAGE eMsg, uint32_t uPrm1, uint32_t uPrm2, void* pContext);
typedef I3D_LOAD_CB_PROC* PI3D_LOAD_CB_PROC;

#ifndef I3D_SUCCESS
#define I3D_SUCCESS(n) ((LS3D_RESULT)n >= 0)
#endif
#ifndef I3D_FAIL
#define I3D_FAIL(n) ((LS3D_RESULT)n < 0)
#endif

#endif