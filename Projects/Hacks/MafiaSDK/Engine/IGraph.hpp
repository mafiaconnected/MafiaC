/*
	Copyright 2017 Dávid Svitana

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef _IGraph_H_
#define _IGraph_H_

namespace MafiaSDK
{
    constexpr unsigned long IGraph_Constance = 0x00647EE0;

    enum PRIMITIVE_TYPE
    {
        POINTLIST = 0,
        LINELIST = 1,
        LINESTRIP = 2,
        TRIANGLELIST = 3,
        TRIANGLEFAN = 4,
        TRIANGLESTRIP = 5,
    };

    enum LS3D_STREAM_TYPE
    {
        BORDER = 0,
        FILLED = 1
    };

    class IGraph
    {
        public:
        virtual class IShow * __stdcall CreateIShow(void);								// 0000  0000
		virtual enum  LS3D_RESULT __stdcall CreateITexture(char *,char *,unsigned int,class ITexture * *);								// 0001  0004
		virtual class ITexture * __stdcall CreateITexture(void);								// 0002  0008
		virtual bool __stdcall SetTexture(ITexture *);								// 0003  000C
		virtual HWND __stdcall GetMainHWND(void);								// 0004  0010
		virtual HWND __stdcall GetChildHWND(void);								// 0005  0014
		virtual void __stdcall SetAppName(char const *);								// 0006  0018
		virtual void __cdecl SetAppNameDebug(char const *,...);								// 0007  001C
		virtual char const * __stdcall GetAppName(void);								// 0008  0020
		virtual bool __stdcall AddDlgHWND(void *);								// 0009  0024
		virtual bool __stdcall DeleteDlgHWND(void *);								// 0010  0028
		virtual void __stdcall ProcessWinMessages(void);								// 0011  002C
		virtual void __stdcall Clear(unsigned int,float,unsigned int);								// 0012  0030
		virtual void __stdcall BeginScene(void);								// 0013  0034
		virtual void __stdcall EndScene(void);								// 0014  0038
		virtual void __stdcall Present(void);								// 0015  003C
		virtual void __stdcall ResetRenderProps(void);								// 0016  0040
		virtual void __stdcall SetState(enum  DX_STATE,unsigned int);								// 0017  0044
		virtual unsigned int __stdcall GetState(enum  DX_STATE);								// 0018  0048
		virtual enum  LS3D_RESULT __stdcall SetViewPort(struct LS3D_VIEWPORT *);								// 0019  004C
		virtual struct LS3D_VIEWPORT * __stdcall GetViewPort(void);								// 0020  0050
		virtual void __stdcall SetWorldMatrix(struct S_matrix const &);								// 0021  0054
		virtual enum  LS3D_RESULT __stdcall DrawPrimitiveList(PRIMITIVE_TYPE,unsigned int,void *, LS3D_STREAM_TYPE);								// 0022  0058
		virtual enum  LS3D_RESULT __stdcall DrawIndexedPrimitiveList(PRIMITIVE_TYPE,unsigned int,void *,unsigned int,void *, LS3D_STREAM_TYPE);								// 0023  005C
		virtual enum  LS3D_RESULT __stdcall Init(struct IGRAPH_INIT_DESC *);								// 0024  0060
		virtual void sub_10070190(void);								// 0025  0064
		virtual bool __stdcall IsInit(void);								// 0026  0068
		virtual void __stdcall Close(void);								// 0027  006C
		virtual int __stdcall Scrn_sx(void);								// 0028  0070
		virtual int __stdcall Scrn_sy(void);								// 0029  0074
		virtual int __stdcall BPP(void);								// 0030  0078
		virtual enum  _D3DFORMAT __stdcall GetD3DFormat(void);								// 0031  007C
		virtual char const * __stdcall GetFormatName(enum  _D3DFORMAT)const;								// 0032  0080
		virtual enum  LS3D_RESULT __stdcall GetImageInfo(char const *,unsigned short &,unsigned short &,unsigned char &,unsigned int,int *);								// 0033  0084
		virtual bool __stdcall SaveShot(char const *,struct tagRECT *,unsigned int);								// 0034  0088
		virtual enum  LS3D_RESULT __stdcall KeyboardInit(unsigned int);								// 0035  008C
		virtual bool __stdcall IsKeyboardInit(void);								// 0036  0090
		virtual void __stdcall KeyboardClose(void);								// 0037  0094
		virtual enum  LS3D_RESULT __stdcall UpdateKeyboardData(void);								// 0038  0098
		virtual unsigned char __stdcall ReadKey(void);								// 0039  009C
        virtual void sub_10071A10(void); // 0040  00A0
        virtual void sub_10071910(void); // 0041  00A4
        virtual void sub_10071940(void); // 0042  00A8
		virtual bool __stdcall TestKey(unsigned char);								// 0043  00AC
		virtual enum  LS3D_RESULT __stdcall DefineKeys(char const *,unsigned int);								// 0044  00B0
		virtual __int64 __stdcall ReadKeys(void);								// 0045  00B4
		virtual bool __stdcall KeyShift(void);								// 0046  00B8
		virtual bool __stdcall KeyCtrl(void);								// 0047  00BC
		virtual bool __stdcall KeyAlt(void);								// 0048  00C0
		virtual unsigned int __stdcall GetShiftKeys(bool);								// 0049  00C4
		virtual unsigned char __stdcall TranslateKey(unsigned char,unsigned int);								// 0050  00C8
        virtual void sub_10071C40(void); // 0051  00CC
        virtual void sub_10071CA0(void); // 0052  00D0
		virtual bool __stdcall IsMouseInit(void);								// 0053  00D4
		virtual void __stdcall MouseClose(void);								// 0054  00D8
		virtual enum  LS3D_RESULT __stdcall UpdateMouseData(void);								// 0055  00DC
		virtual int __stdcall Mouse_rx(void);								// 0056  00E0
		virtual int __stdcall Mouse_ry(void);								// 0057  00E4
		virtual int __stdcall Mouse_rz(void);								// 0058  00E8
		virtual void __stdcall SetMousePos(int,int);								// 0059  00EC
		virtual int __stdcall Mouse_x(void);								// 0060  00F0
		virtual int __stdcall Mouse_y(void);								// 0061  00F4
		virtual unsigned int __stdcall GetMouseButtons(void);								// 0062  00F8
		virtual void __stdcall NullTimer(void);								// 0063  00FC
		virtual unsigned int __stdcall GetTimer(unsigned int,unsigned int);								// 0064  0100
		virtual enum  LS3D_RESULT __stdcall AddMapsDir(char const *,bool);								// 0065  0104
		virtual enum  LS3D_RESULT __stdcall DeleteMapsDir(char const *);								// 0066  0108
		virtual void __stdcall ClearMapDirs(void);								// 0067  010C
		virtual class ITexture * * __stdcall GetCachedTextures(void);								// 0068  0110
		virtual unsigned int __stdcall GetCachedTextureCnt(void);								// 0069  0114
		virtual class ITexture * * __stdcall GetUncachedTextures(void);								// 0070  0118
		virtual unsigned int __stdcall GetUncachedTextureCnt(void);								// 0071  011C
		virtual bool __stdcall SetGammaRamp(struct LS3DGAMMARAMP *,unsigned int);								// 0072  0120
		virtual bool __stdcall GetGammaRamp(struct LS3DGAMMARAMP *);								// 0073  0124
		virtual bool __stdcall ResizeImage(int,int,unsigned int,unsigned char *,int,int,unsigned int,unsigned char * *,unsigned int,int);								// 0074  0128
    };

	extern IGraph* GetIGraph();
	extern bool IsWindowFocused();

    // TODO: Move it to I3D_Camera
    static MafiaSDK::I3D_Frame* GetCurrentCamera() {
        __asm {
            mov eax, dword ptr ds : [63788Ch]
            mov ecx, dword ptr ds : [eax + 10h]
            mov eax, dword ptr ds : [ecx + 17Ch]
        }
    }
}

#endif
