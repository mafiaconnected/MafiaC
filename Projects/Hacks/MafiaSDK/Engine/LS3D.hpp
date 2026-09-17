/*
    LS3D engine (LS3DF.dll) result codes, enums and callback typedefs.

    Several MafiaSDK headers (I3D_Frame.hpp, I3D_Driver.hpp, IGraph.hpp,
    I3D_Mesh_Object.hpp, I3D_Mesh_Level.hpp, I3D_Model.hpp) declare virtual
    functions returning/taking `enum LS3D_RESULT`/`enum I3DENUMRET` etc. as bare
    elaborated-type-specifiers. MSVC accepts that without a prior declaration
    (non-standard extension) and none of those enumerators are ever compared
    against by name elsewhere in the SDK, so it happened to compile without
    this file - but the actual values were never available to use. Ported
    from reMafia's LS3D.h (same author, MafiaOrbitCam/Vendors/reMafia).
*/

#ifndef _LS3D_H_
#define _LS3D_H_

namespace MafiaSDK
{
    class I3D_Frame;

    // Bounding box defined by 2 boundary points
    struct I3D_bbox
    {
        S_vector min, max;
        I3D_bbox() { }
        I3D_bbox(const S_vector& n, const S_vector& x) { min = n; max = x; }
        inline const S_vector& operator[](int i) const { return (&min)[i]; }
    };

    // Bounding sphere
    struct I3D_bsphere
    {
        S_vector pos;
        float radius;
        I3D_bsphere() { }
        I3D_bsphere(const S_vector& p, float r) : pos(p), radius(r) { }
    };

    // Bounding volume - consisting of a minimal box and a minimal sphere
    struct I3D_bound_volume
    {
        I3D_bbox bbox;
        I3D_bsphere bsphere;
    };

    typedef long LS3D_RESULT;
    enum
    {
        I3D_OK,

        I3DERR_NOFILE                = 0x80000001,
        I3DERR_OUTOFMEM              = 0x80000002,
        I3DERR_TEXTURESNOTSUPPORTED  = 0x80000003,
        I3DERR_BADTEXTURE            = 0x80000004,
        I3DERR_GENERIC               = 0x80000005,
        I3DERR_OBJECTNOTFOUND        = 0x80000006,
        I3DERR_INVALIDPARAM          = 0x80000007,
        I3DERR_NOTLOADED             = 0x80000008,
        I3DERR_NOTINITIALIZED        = 0x80000009,
        I3DERR_FILECORRUPTED         = 0x8000000A,
        I3DERR_NOADDITIONALFILE      = 0x8000000B,
        I3DERR_CYCLICLINKOFHIERARCHY = 0x8000000C,
        I3DERR_ENUMERATIONCANCELLED  = 0x8000000D,
        I3DERR_UNSUPPORTED           = 0x8000000E,
        I3DERR_NO3DHARDWARE          = 0x8000000F,
        I3DERR_SNDBUFFERLOST         = 0x80000010,
        I3DERR_USERBREAK             = 0x80000011,
        I3DERR_FILESYSTEMERR         = 0x80000012,
        I3DERR_BADFILEVERSION        = 0x80000013,
        I3DERR_BADFILETIMESTAMP      = 0x80000014,
        I3DERR_LIMITOFENGINEEXCEEDED = 0x80000015
    };

#ifndef I3D_SUCCESS
#define I3D_SUCCESS(n) ((MafiaSDK::LS3D_RESULT)(n) >= 0)
#endif
#ifndef I3D_FAIL
#define I3D_FAIL(n) ((MafiaSDK::LS3D_RESULT)(n) < 0)
#endif

    enum I3D_FRAME_TYPE
    {
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

    enum I3D_VISUAL_TYPE
    {
        VISUAL_LIT_OBJECT        = 0x50414D4C,
        VISUAL_PROJECTOR         = 0x4A4F5250,
        VISUAL_SINGLE_MESH       = 0x4D474E53,
        VISUAL_MORPH             = 0x4850524D,
        VISUAL_PART_ELEMENT_BASE = 0x42545250,
        VISUAL_BILLBOARD         = 0x44524242,
        VISUAL_PART_ELEMENT      = 0x45545250,
        VISUAL_MIRROR            = 0x5252494D,
        VISUAL_LAND_PATCH        = 0x50444E4C,
        VISUAL_SINGLE_MORPH      = 0x524D4D53,
        VISUAL_LENSFLARE         = 0x534E454C,
        VISUAL_OBJECT            = 0x5F4A424F
    };

    // NOTE(DavoSK, via reMafia): find out what is matching with LS3DF range of enum, seems pretty same;
    // real mapping can be generated from I3D_driver::GetState
    enum I3D_RENDERSTATE
    {
        RS_LINEARFILTER        = 0,
        RS_MIPMAP              = 1,
        RS_USE_EMBM            = 2,
        RS_USEZB               = 3,
        RS_CLEAR               = 4,
        RS_DITHER              = 5,
        RS_POINTFOG            = 6,
        RS_WIREFRAME           = 7,
        RS_DRAWBOUNDBOX        = 8,
        RS_DRAWHRBOUNDBOX      = 9,
        RS_DRAWLINKS           = 10,
        RS_LOADMIPMAP          = 11,
        RS_DRAWMIRRORS         = 12,
        RS_USESHADOWS          = 13,
        RS_DEBUGDRAWSHADOWS    = 14,
        RS_DRAWSECTORS         = 15,
        RS_DRAWPORTALS         = 16,
        RS_TEXTUREDITHER       = 17,
        RS_DRAWVISUALS         = 18,
        RS_FOG                 = 19,
        RS_LOD_QUALITY         = 20,
        RS_DEBUGDRAWSHDRECS    = 21,
        RS_TEXTURELOWDETAIL    = 22,
        RS_DRAWVOLUMES         = 23,
        RS_DRAWLIGHTS          = 24,
        RS_DEBUG_DRAW_MATS     = 25,
        RS_DRAWSOUNDS          = 26,
        RS_TEXTURECOMPRESS     = 27,
        RS_USELMAPPING         = 28,
        RS_DRAWLMTEXTURES      = 29,
        RS_DRAWCAMERAS         = 30,
        RS_DRAWDUMMYS          = 31,
        RS_DETAILMAPPING       = 32,
        RS_DRAWOCCLUDERS       = 33,
        RS_DRAWTEXTURES        = 34,
        RS_LMTRUECOLOR         = 35,
        RS_LMDITHER            = 36,
        RS_LOD_SCALE           = 37,
        RS_DEBUGDRAWBSP        = 38,
        RS_DRAWJOINTS          = 39,
        RS_DEBUGDRAWDYNAMIC    = 40,
        RS_DEBUGDRAWSTATIC     = 41,
        RS_ANISO_FILTERING     = 42,
        RS_DRAW_COL_TESTS      = 43,
        RS_SOUND_VOLUME        = 44,
        RS_PROFILER_MODE       = 45,
        RS_LOD_INDEX           = 46,
        RS_USE_OCCLUSION       = 47,
        RS_LM_AA_RATIO         = 48,
        RS_ENVMAPPING          = 49,
        RS_LAST
    };

    // Enum-frames flags
    enum
    {
        ENUMF_VISUAL       = 1,
        ENUMF_LIGHT        = 2,
        ENUMF_CAMERA       = 4,
        ENUMF_SOUND        = 8,
        ENUMF_SECTOR       = 0x10,
        ENUMF_DUMMY        = 0x20,
        ENUMF_MODEL        = 0x80,
        ENUMF_USER         = 0x100,
        ENUMF_VOLUME       = 0x200,
        ENUMF_JOINT        = 0x400,
        ENUMF_OCCLUDER     = 0x800,
        ENUMF_ALL          = 0x0ffff,
        ENUMF_WILDMASK     = 0x10000,
        ENUMF_CASESENS     = 0x20000,
        ENUMF_NO_BACKDROP  = 0x40000
    };

    // Enum-frames return value
    enum I3DENUMRET
    {
        I3DENUMRET_OK,
        I3DENUMRET_SKIPCHILDREN,
        I3DENUMRET_CANCEL,
    };

    enum LS3D_CALLBACK_MESSAGE
    {
        I3DCB_MESSAGE       = 2, // const char*, bool still
        I3DCB_DEBUG_MESSAGE = 3, // (const char*, int32_t type) - debug message, types: 0=one-time, 1=console, 2=run-time
        I3DCB_DEBUG_CODE    = 4, // uint32_t code, void *data
        I3DCB_ANIM_FINISH   = 5, // (0, uint32_t stage)
        I3DCB_ANIM_NOTE     = 6, // (I3D_Frame*, const I3D_note_callback*)
    };

    typedef I3DENUMRET(__stdcall I3D_ENUMPROC)(I3D_Frame*, unsigned int uContext);
    typedef I3DENUMRET(__stdcall I3D_ENUMVISUALPROC)(unsigned int uVisualType, const char* pSzFriendlyName);
    typedef void (__stdcall I3D_CALLBACK)(LS3D_CALLBACK_MESSAGE eCallbackMessage, unsigned int uPrm1, unsigned int uPrm2, void* pContext);

    enum I3D_LOADMESSAGE
    {
        CBM_PROGRESS = 0, // (float progress, dword status); status: 0=beg, 1=in, 2=end
        CBM_ERROR    = 1, // (const char *msg, int importance (0=max, 1=warning, ...))
        CBM_LOG      = 2, // (const char *msg)
    };

    // If the returned value for CBM_PROGRESS message is true, the loading is cancelled,
    // and the loader method returns I3DERR_CANCELED.
    typedef bool(__stdcall I3D_LOAD_CB_PROC)(I3D_LOADMESSAGE eMsg, unsigned int uPrm1, unsigned int uPrm2, void* pContext);
};

#endif
