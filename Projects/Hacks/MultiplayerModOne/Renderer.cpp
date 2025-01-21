
#include "stdafx.h"
#include "Renderer.h"

//ID3DXSprite* CClientRenderer::m_pSprite;
//int CClientRenderer::m_iBegun = 0;

IDirect3DTexture9* pD3DTexture = nullptr;
Direct3D9* g_pD3D9 = nullptr;

void CClientRenderer::OnDeviceLost()
{
	if (g_pD3D9)
		g_pD3D9->OnDeviceLost();
	//if (m_iBegun > 0)
	//{
	//	m_pSprite->End();
	//	m_iBegun = 0;
	//}
	//SAFE_RELEASE(m_pSprite);
}

void CClientRenderer::OnDeviceReset(IDirect3DDevice9* pD3DDevice)
{
	//pD3DDevice->CreateTexture((UINT)1, (UINT)1, (UINT)1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pD3DTexture, NULL);

	//D3DLOCKED_RECT lr;
	//pD3DTexture->LockRect((UINT)0, &lr, NULL, 0);
	//COLOUR Colour = CARGB::White;
	//memcpy((unsigned char*)lr.pBits + 0 * lr.Pitch, (unsigned char*)&Colour, 1 * sizeof(COLOUR));
	//pD3DTexture->UnlockRect((UINT)0);

	//D3DXCreateSprite(pD3DDevice, &m_pSprite);
}

#if 0
void CClientRenderer::DrawTexture(IDirect3DTexture9* pTexture, CVector2D* pvecPos, CVector2D* pvecSize, float fRotation, CVector2D* pvecCentre, D3DCOLOR Colour, CRect* pSourceRect, CVector2D* pvecScale, bool bKeepStencil)
{
	if (pTexture == nullptr)
	{
		//pTexture = pD3DTexture;
		return;
	}
	float fImageWidth = 1.0f;
	float fImageHeight = 1.0f;
	if (pTexture != nullptr)
	{
		D3DSURFACE_DESC TextureDesc;
		pTexture->GetLevelDesc(0, &TextureDesc);
		fImageWidth = (float)TextureDesc.Width;
		fImageHeight = (float)TextureDesc.Height;
	}
	float fSourceWidth = pSourceRect == NULL ? fImageWidth : (((float)(pSourceRect->m_fRight - pSourceRect->m_fLeft))*(fImageWidth));
	float fSourceHeight = pSourceRect == NULL ? fImageHeight : (((float)(pSourceRect->m_fBottom - pSourceRect->m_fTop))*(fImageHeight));
	CVector2D vecSizeScale = CVector2D(pvecSize->x / fSourceWidth, pvecSize->y / fSourceHeight);
	CVector2D vecPos = *pvecPos;
	Begin();
	D3DXMATRIX matRot;
	D3DXMATRIX matTranslation;
	D3DXMATRIX matCentre;
	D3DXMATRIX matSizeScale;
	D3DXMatrixRotationZ(&matRot, fRotation);
	D3DXMatrixScaling(&matSizeScale, vecSizeScale.x, vecSizeScale.y, 0.0f);
	D3DXMatrixTranslation(&matTranslation, vecPos.x, vecPos.y, 0.0f);
	if (pvecCentre != NULL)
		D3DXMatrixTranslation(&matCentre, -pvecCentre->x*fSourceWidth*vecSizeScale.x, -pvecCentre->y*fSourceHeight*vecSizeScale.y, 0.0f);
	else
		D3DXMatrixTranslation(&matCentre, 0.0f, 0.0f, 0.0f);
	D3DXMATRIX matScreenScaling;
	if (pvecScale == NULL)
		D3DXMatrixScaling(&matScreenScaling, 1.0f, 1.0f, 0.0f);
	else
		D3DXMatrixScaling(&matScreenScaling, pvecScale->x, pvecScale->y, 0.0f);
	D3DXMATRIX mat = matSizeScale * matCentre*matRot*matTranslation*matScreenScaling;
	if (pSourceRect != NULL)
	{
		RECT SourceRect;
		SourceRect.right = (LONG)(pSourceRect->m_fRight*(fImageWidth + 1.0f));
		SourceRect.bottom = (LONG)(pSourceRect->m_fBottom*(fImageHeight + 1.0f));
		SourceRect.left = (LONG)(pSourceRect->m_fLeft*(fImageWidth + 1.0f));
		SourceRect.top = (LONG)(pSourceRect->m_fTop*(fImageHeight + 1.0f));
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(pTexture, &SourceRect, NULL, NULL, Colour);
	}
	else
	{
		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(pTexture, NULL, NULL, NULL, Colour);
	}
	End();
}

void CClientRenderer::Begin()
{
	m_iBegun++;
	if (m_iBegun == 1)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
}

void CClientRenderer::End()
{
	m_iBegun--;
	if (m_iBegun == 0)
	{
		m_pSprite->End();
	}
}
#endif

MafiaDirect3D9::MafiaDirect3D9(Device* pDevice) : Direct3D9(pDevice)
{
}

MafiaDirect3D9::~MafiaDirect3D9()
{
}

bool MafiaDirect3D9::EnterFrame()
{
	return true;
}

void MafiaDirect3D9::EndFrame()
{
}

void MafiaDirect3D9::SetViewport(CViewport* pViewport)
{
	Renderer::SetViewport(pViewport);
}
