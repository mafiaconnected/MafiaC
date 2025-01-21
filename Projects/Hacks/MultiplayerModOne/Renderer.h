#pragma once

#include <Devices/Direct3D9/Internal.h>
#include <Devices/Direct3D9/Plugin.h>

#include <d3dx9.h>

class MafiaDirect3D9 : public Direct3D9
{
public:
	MafiaDirect3D9(Device* pDevice);
	virtual ~MafiaDirect3D9();

	virtual bool EnterFrame() override;
	virtual void EndFrame() override;

	virtual void SetViewport(CViewport* pViewport) override;
};

class CClientRenderer
{
private:
	//static int m_iBegun;
public:
	//static ID3DXSprite* m_pSprite;
	//static IDirect3DVertexBuffer8* m_pImageVB;
	static void OnDeviceLost();
	static void OnDeviceReset(IDirect3DDevice9* pD3DDevice);
	//static void DrawTexture(IDirect3DTexture9* pTexture, CVector2D* pvecPos, CVector2D* pvecSize, float fRotation, CVector2D* pvecCentre, D3DCOLOR Colour, CRect* pSourceRect, CVector2D* pvecScale = NULL, bool bKeepStencil = false);
	//static void Begin();
	//static void End();
};
