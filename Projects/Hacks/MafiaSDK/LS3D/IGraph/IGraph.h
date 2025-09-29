#pragma once
#include "I3D/I3D_math.h"
#include "LS3D.h"
#include <C_list.h>
#include <C_str.hpp>

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <d3d8.h>

#include "IBManager.h"
#include "VBManager.h"

#define MAX_DEFINED_KEYS    64
#define KEYBOARD_BUFFER_LEN 256
#define FVF_2D_PRIMITIVE    (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define CLEAR_TARGET        0x00000001l /* Clear target surface */
#define CLEAR_ZBUFFER       0x00000002l

#pragma pack(push, 1)
struct S_vertex_2d {
	float x, y, z, w;
	DWORD diffuse;
	DWORD specular;
	float u, v;
};

struct S_vertex_border {
	float x, y, z, w;
	DWORD diffuse;
	DWORD specular;
};

#pragma pack(pop)


struct LS3DGAMMARAMP {
	WORD                red[256];
	WORD                green[256];
	WORD                blue[256];
};
struct LS3D_VIEWPORT {
	DWORD       X;
	DWORD       Y;            /* Viewport Top left */
	DWORD       Width;
	DWORD       Height;       /* Viewport Dimensions */
	float       MinZ;         /* Min/max of clip Volume */
	float       MaxZ;
};

enum MOUSE_BUTTONS_FLAGS { MOUSE_BUTTON_LEFT = (1 << 0), MOUSE_BUTTON_RIGHT = (1 << 1), MOUSE_BUTTON_MIDDLE = (1 << 2) };

// NOTE (Sev3n): Temporarily put here, just to reverse IGraph::Init method
struct I3D_stats_volume {
	uint8_t buf[32];
};

struct IGRAPH_INIT_DESC {
	HINSTANCE Instance;
	uint32_t CurrentAdapter;
	uint32_t X;
	uint32_t Y;
	uint32_t Width;
	uint32_t Height;
	RECT Rc;
	uint8_t BPP;
	uint8_t RefreshRate;
	uint8_t Unk2;
	uint8_t Unk3;
	uint32_t Flags;
	HMENU Menu;
};

#pragma pack(push, 1)
typedef struct _tgaheader {
	uint8_t IDLength; /* 00h  Size of Image ID field */
	uint8_t ColorMapType; /* 01h  Color map type */
	uint8_t ImageType; /* 02h  Image type code */
	uint8_t CMapStart[2]; /* 03h  Color map origin */
	uint8_t CMapLength[2]; /* 05h  Color map length */
	uint8_t CMapDepth; /* 07h  Depth of color map entries */
	uint16_t XOffset; /* 08h  X origin of image */
	uint16_t YOffset; /* 0Ah  Y origin of image */
	uint16_t Width; /* 0Ch  Width of image */
	uint16_t Height; /* 0Eh  Height of image */
	uint8_t PixelDepth; /* 10h  Image pixel size */
	uint8_t ImageDescriptor; /* 11h  Image descriptor byte */
} TGAHEADER;
#pragma pack(pop)

// TODO(DavoSK): meybe later on unix / linux ?
//
// typedef struct _tagBITMAPFILEHEADER {
//     uint16_t bfType;
//     uint32_t bfSize;
//     uint16_t bfReserved1;
//     uint16_t bfReserved2;
//     uint32_t bfOffBits;
// } BITMAPFILEHEADER;
//
// typedef struct _tagBITMAPINFOHEADER {
//     uint32_t biSize;
//     uint32_t biWidth;
//     uint32_t biHeight;
//     uint16_t biPlanes;
//     uint16_t biBitCount;
//     uint32_t biCompression;
//     uint32_t biSizeImage;
//     uint32_t biXPelsPerMeter;
//     uint32_t biYPelsPerMeter;
//     uint32_t biClrUsed;
//     uint32_t biClrImportant;
// } BITMAPINFOHEADER;

enum DX_STATE {
	ALPHA = 0,
	BLEND,
	CLIP,
	CULL,
	COLOR_OP,
	COLOR_OP1,
	COLOR_OP2,
	ALPHA_OP,
	ZBIAS,
	ADDRESS_U,
	ADDRESS_V,
	ALPHA_REF,
	ZCOMPARE,
	ZWRITABLE,
	ZENABLE,
	CLAMP_TEXTURE,
	FOG = 16
};

enum PRIMITIVE_TYPE { PT_POINTLIST = 0, PT_LINELIST = 1, PT_LINESTRIP = 2, PT_TRIANGLELIST = 3, PT_TRIANGLEFAN = 4, PT_TRIANGLESTRIP = 5, PT_UNKNOWN = 6 };

enum IGRAPH_INIT_FLAGS {
	INITFLAG_FULLSCREEN = 0x2,
	INITFLAG_TRIPLEBUFFERING = 0x4,
	INITFLAG_WINDOWED = 0x8,
	INITFLAG_HWTRANSANDLIGHT = 0x10,
	INITFLAG_DEBUGLOG = 0x40,
	INITFLAG_DONTCREATECHILDWINDOW = 0x80,
	INITFLAG_VSYNC = 0x100,
	INITFLAG_LOCKABLESTENCILFORMAT = 0x400,
	INITFLAG_SLOWREFRASTERIZER = 0x800,
	INITFLAG_2XMSAA = 0x1000,
	INITFLAG_ENABLEMSAA = 0x3000,
	INITFLAG_MULTIPASSRENDERING = 0x4000,
	INITFLAG_DEBUGMODE = 0x10000,
	INITFLAG_USETEXMANAGER = 0x100000,
};

enum IGRAPH_HWCAPS {
	HWCAPS_HARDWARE_TRANSFORM = 0x1,
	HWCAPS_SINGLEPASS_RENDERING = 0x80,
};

enum LS3D_STREAM_TYPE { ST_BORDER = 0, ST_FILLED = 1 };

class IShow;
class ITexture;

class IGraph {
public:
	virtual IShow* __stdcall CreateIShow();
	virtual LS3D_RESULT __stdcall CreateITexture(const char* pSzDiffuse, const char* pSzAlpha, uint32_t uFlags, ITexture** pTextureOut);
	virtual ITexture* __stdcall CreateITexture2();
	virtual bool __stdcall SetTexture(ITexture* pITexture);
	virtual HWND __stdcall GetMainHWND();
	virtual HWND __stdcall GetChildHWND();
	virtual void __stdcall SetAppName(const char* pSzName);
	virtual void __stdcall SetAppNameDebug(const char* pSzFormat, ...);
	virtual const char* __stdcall GetAppName();
	virtual bool __stdcall AddDlgHWND(HWND);
	virtual bool __stdcall DeleteDlgHWND(HWND);
	virtual void __stdcall ProcessWinMessages();
	virtual void __stdcall Clear(uint32_t color, float depth, uint32_t flags);
	virtual void __stdcall BeginScene();
	virtual void __stdcall EndScene();
	virtual void __stdcall Present();
	virtual void __stdcall ResetRenderProps();
	virtual int32_t __stdcall SetState(DX_STATE, uint32_t);
	virtual int32_t __stdcall GetState(DX_STATE);
	virtual LS3D_RESULT __stdcall SetViewPort(LS3D_VIEWPORT*);
	virtual LS3D_VIEWPORT* __stdcall GetViewPort();
	virtual void __stdcall SetWorldMatrix(const S_matrix& matrix);
	virtual LS3D_RESULT __stdcall DrawPrimitiveList(PRIMITIVE_TYPE primitiveType, uint32_t primitiveCount, void* buffer, LS3D_STREAM_TYPE streamType);
	virtual LS3D_RESULT __stdcall DrawIndexedPrimitiveList(
		PRIMITIVE_TYPE primitiveType, uint32_t primitiveCount, void* buffer, uint32_t indicesCount, void* indicesBuffer, LS3D_STREAM_TYPE streamType);
	virtual LS3D_RESULT __stdcall Init(IGRAPH_INIT_DESC*);
	virtual IGRAPH_INIT_DESC* __stdcall GetResInitDesc();
	virtual bool __stdcall IsInit();
	virtual void __stdcall Close();
	virtual int32_t __stdcall Scrn_sx();
	virtual int32_t __stdcall Scrn_sy();
	virtual int32_t __stdcall BPP();
	virtual _D3DFORMAT __stdcall GetD3DFormat();
	virtual const char* __stdcall GetFormatName(_D3DFORMAT);
	virtual LS3D_RESULT __stdcall GetImageInfo(const char*, uint16_t*, uint16_t*, uint8_t*, uint32_t, int32_t*);
	virtual bool __stdcall SaveShot(const char* pSzFileName, RECT* pRect, uint32_t uFlags, void* pUnk);
	virtual LS3D_RESULT __stdcall KeyboardInit(uint32_t);
	virtual bool __stdcall IsKeyboardInit();
	virtual void __stdcall KeyboardClose();
	virtual LS3D_RESULT __stdcall UpdateKeyboardData();
	virtual uint32_t __stdcall ReadKey();
	virtual uint32_t __stdcall ReadChar();
	virtual char* __stdcall ReadCharBuffer();
	virtual void __stdcall ClearCharBuffer();
	virtual bool __stdcall TestKey(uint8_t);
	virtual LS3D_RESULT __stdcall DefineKeys(const char*, uint32_t);
	virtual int64_t __stdcall ReadKeys();
	virtual bool __stdcall KeyShift();
	virtual bool __stdcall KeyCtrl();
	virtual bool __stdcall KeyAlt();
	virtual uint32_t __stdcall GetShiftKeys(bool);
	virtual char __stdcall TranslateKey(uint8_t, uint32_t);
	virtual const wchar_t* __stdcall GetKeyName(int32_t);
	virtual LS3D_RESULT __stdcall MouseInit(uint32_t);
	virtual bool __stdcall IsMouseInit();
	virtual void __stdcall MouseClose();
	virtual LS3D_RESULT __stdcall UpdateMouseData();
	virtual int32_t __stdcall Mouse_rx();
	virtual int32_t __stdcall Mouse_ry();
	virtual int32_t __stdcall Mouse_rz();
	virtual void __stdcall SetMousePos(int32_t, int32_t);
	virtual int32_t __stdcall Mouse_x();
	virtual int32_t __stdcall Mouse_y();
	virtual uint32_t __stdcall GetMouseButtons();
	virtual void __stdcall NullTimer();
	virtual uint32_t __stdcall GetTimer(uint32_t, uint32_t);
	virtual LS3D_RESULT __stdcall AddMapsDir(const char*, bool);
	virtual LS3D_RESULT __stdcall DeleteMapsDir(const char*);
	virtual void __stdcall ClearMapDirs();
	virtual ITexture** __stdcall GetCachedTextures();
	virtual uint32_t __stdcall GetCachedTextureCnt();
	virtual ITexture** __stdcall GetUncachedTextures();
	virtual uint32_t __stdcall GetUncachedTextureCnt();
	virtual bool __stdcall SetGammaRamp(LS3DGAMMARAMP*, uint32_t);
	virtual bool __stdcall GetGammaRamp(LS3DGAMMARAMP*);
	virtual bool __stdcall ResizeImage(int32_t, int32_t, uint32_t, uint8_t*, int32_t, int32_t, uint32_t, uint8_t**, uint32_t, int32_t);
};

extern "C" {
	extern __declspec(dllexport) IGraph* GetIGraph();
}