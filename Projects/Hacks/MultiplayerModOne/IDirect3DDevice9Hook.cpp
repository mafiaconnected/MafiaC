
#include "stdafx.h"
#include "IDirect3D9Hook.h"
#include "CHookedDirect3D9Util.h"

IDirect3DDevice9* g_pD3DDevice;

IDirect3DDevice9Hook::IDirect3DDevice9Hook(IDirect3D9* pDirect3D9, IDirect3DDevice9* pDirect3DDevice9)
{
	g_pD3DDevice = pDirect3DDevice9;
	m_pDirect3D9 = pDirect3D9;
	m_pDirect3DDevice9 = pDirect3DDevice9;
}

HRESULT IDirect3DDevice9Hook::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pDirect3DDevice9->QueryInterface(riid,ppvObj);
}

ULONG IDirect3DDevice9Hook::AddRef()
{
	//m_pDirect3DDevice9->AddRef();
	m_RefCount++;
	return m_RefCount;
}

ULONG IDirect3DDevice9Hook::Release()
{
	if (m_RefCount <= 1)
	{
		m_pDirect3DDevice9->Release();
		delete this;
		return 0;
	}
	m_RefCount--;
	return m_RefCount;
}

HRESULT IDirect3DDevice9Hook::TestCooperativeLevel()
{
	return m_pDirect3DDevice9->TestCooperativeLevel();
}

UINT IDirect3DDevice9Hook::GetAvailableTextureMem()
{
	return m_pDirect3DDevice9->GetAvailableTextureMem();
}

HRESULT IDirect3DDevice9Hook::EvictManagedResources()
{
	return m_pDirect3DDevice9->EvictManagedResources();
}

HRESULT IDirect3DDevice9Hook::GetDirect3D(IDirect3D9** ppD3D9)
{
	return m_pDirect3DDevice9->GetDirect3D(ppD3D9);
}

HRESULT IDirect3DDevice9Hook::GetDeviceCaps(D3DCAPS9* pCaps)
{
	return m_pDirect3DDevice9->GetDeviceCaps(pCaps);
}

HRESULT IDirect3DDevice9Hook::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	return m_pDirect3DDevice9->GetDisplayMode(iSwapChain, pMode);
}

HRESULT IDirect3DDevice9Hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	return m_pDirect3DDevice9->GetCreationParameters(pParameters);
}

HRESULT IDirect3DDevice9Hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
	return m_pDirect3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void IDirect3DDevice9Hook::SetCursorPosition(int X, int Y, DWORD Flags)
{
	m_pDirect3DDevice9->SetCursorPosition(X, Y, Flags);
}

BOOL IDirect3DDevice9Hook::ShowCursor(BOOL bShow)
{
	return m_pDirect3DDevice9->ShowCursor(bShow);
}

HRESULT IDirect3DDevice9Hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	return m_pDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT IDirect3DDevice9Hook::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	return m_pDirect3DDevice9->GetSwapChain(iSwapChain, pSwapChain);
}

UINT IDirect3DDevice9Hook::GetNumberOfSwapChains()
{
	return m_pDirect3DDevice9->GetNumberOfSwapChains();
}

HRESULT IDirect3DDevice9Hook::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	CHookedDirect3D9Util::OnPreLostDevice(m_pDirect3DDevice9, m_pDirect3D9, false);
	CHookedDirect3D9Util::OnPostLostDevice(m_pDirect3DDevice9, m_pDirect3D9, false);
	CHookedDirect3D9Util::OnPreResetDevice(m_pDirect3DDevice9, m_pDirect3D9, false, pPresentationParameters);
	HRESULT hrResult = m_pDirect3DDevice9->Reset((D3DPRESENT_PARAMETERS*)pPresentationParameters);
	if (SUCCEEDED(hrResult))
	{
		CHookedDirect3D9Util::OnPostResetDevice(m_pDirect3DDevice9, m_pDirect3D9, false, pPresentationParameters);
		CHookedDirect3D9Util::m_bSimulateDeviceNotReset = false;
	}
	return hrResult;
	//return m_pDirect3DDevice9->Reset(pPresentationParameters);
}

HRESULT IDirect3DDevice9Hook::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return m_pDirect3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT IDirect3DDevice9Hook::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	return m_pDirect3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT IDirect3DDevice9Hook::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	return m_pDirect3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT IDirect3DDevice9Hook::SetDialogBoxMode(BOOL bEnableDialogs)
{
	return m_pDirect3DDevice9->SetDialogBoxMode(bEnableDialogs);
}

void IDirect3DDevice9Hook::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	m_pDirect3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void IDirect3DDevice9Hook::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	m_pDirect3DDevice9->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT IDirect3DDevice9Hook::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
	return m_pDirect3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT IDirect3DDevice9Hook::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	return m_pDirect3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT IDirect3DDevice9Hook::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
	return m_pDirect3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT IDirect3DDevice9Hook::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	return m_pDirect3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT IDirect3DDevice9Hook::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	return m_pDirect3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT IDirect3DDevice9Hook::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	return m_pDirect3DDevice9->ColorFill(pSurface, pRect, color);
}

HRESULT IDirect3DDevice9Hook::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	return m_pDirect3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT IDirect3DDevice9Hook::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	return m_pDirect3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT IDirect3DDevice9Hook::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{
	return m_pDirect3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT IDirect3DDevice9Hook::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	return m_pDirect3DDevice9->SetDepthStencilSurface(pNewZStencil);
}

HRESULT IDirect3DDevice9Hook::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	return m_pDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT IDirect3DDevice9Hook::BeginScene()
{
	return m_pDirect3DDevice9->BeginScene();
}

HRESULT IDirect3DDevice9Hook::EndScene()
{
	return m_pDirect3DDevice9->EndScene();
}

HRESULT IDirect3DDevice9Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return m_pDirect3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT IDirect3DDevice9Hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pDirect3DDevice9->SetTransform(State, pMatrix);
}

HRESULT IDirect3DDevice9Hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return m_pDirect3DDevice9->GetTransform(State, pMatrix);
}

HRESULT IDirect3DDevice9Hook::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pDirect3DDevice9->MultiplyTransform(State, pMatrix);
}

HRESULT IDirect3DDevice9Hook::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	return m_pDirect3DDevice9->SetViewport(pViewport);
}

HRESULT IDirect3DDevice9Hook::GetViewport(D3DVIEWPORT9* pViewport)
{
	return m_pDirect3DDevice9->GetViewport(pViewport);
}

HRESULT IDirect3DDevice9Hook::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	return m_pDirect3DDevice9->SetMaterial(pMaterial);
}

HRESULT IDirect3DDevice9Hook::GetMaterial(D3DMATERIAL9* pMaterial)
{
	return m_pDirect3DDevice9->GetMaterial(pMaterial);
}

HRESULT IDirect3DDevice9Hook::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
	return m_pDirect3DDevice9->SetLight(Index, pLight);
}

HRESULT IDirect3DDevice9Hook::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
	return m_pDirect3DDevice9->GetLight(Index, pLight);
}

HRESULT IDirect3DDevice9Hook::LightEnable(DWORD Index, BOOL Enable)
{
	return m_pDirect3DDevice9->LightEnable(Index, Enable);
}

HRESULT IDirect3DDevice9Hook::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return m_pDirect3DDevice9->GetLightEnable(Index, pEnable);
}

HRESULT IDirect3DDevice9Hook::SetClipPlane(DWORD Index, CONST float* pPlane)
{
	return m_pDirect3DDevice9->SetClipPlane(Index, pPlane);
}

HRESULT IDirect3DDevice9Hook::GetClipPlane(DWORD Index, float* pPlane)
{
	return m_pDirect3DDevice9->GetClipPlane(Index, pPlane);
}

HRESULT IDirect3DDevice9Hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	return m_pDirect3DDevice9->SetRenderState(State, Value);
}

HRESULT IDirect3DDevice9Hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return m_pDirect3DDevice9->GetRenderState(State, pValue);
}

HRESULT IDirect3DDevice9Hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	return m_pDirect3DDevice9->CreateStateBlock(Type, ppSB);
}

HRESULT IDirect3DDevice9Hook::BeginStateBlock()
{
	return m_pDirect3DDevice9->BeginStateBlock();
}

HRESULT IDirect3DDevice9Hook::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	return m_pDirect3DDevice9->EndStateBlock(ppSB);
}

HRESULT IDirect3DDevice9Hook::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDirect3DDevice9->SetClipStatus(pClipStatus);
}

HRESULT IDirect3DDevice9Hook::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	return m_pDirect3DDevice9->GetClipStatus(pClipStatus);
}

HRESULT IDirect3DDevice9Hook::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	return m_pDirect3DDevice9->GetTexture(Stage, ppTexture);
}

HRESULT IDirect3DDevice9Hook::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
	return m_pDirect3DDevice9->SetTexture(Stage, pTexture);
}

HRESULT IDirect3DDevice9Hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return m_pDirect3DDevice9->GetTextureStageState(Stage, Type, pValue);
}

HRESULT IDirect3DDevice9Hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	return m_pDirect3DDevice9->SetTextureStageState(Stage, Type, Value);
}

HRESULT IDirect3DDevice9Hook::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	return m_pDirect3DDevice9->GetSamplerState(Sampler, Type, pValue);
}

HRESULT IDirect3DDevice9Hook::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	return m_pDirect3DDevice9->SetSamplerState(Sampler, Type, Value);
}

HRESULT IDirect3DDevice9Hook::ValidateDevice(DWORD* pNumPasses)
{
	return m_pDirect3DDevice9->ValidateDevice(pNumPasses);
}

HRESULT IDirect3DDevice9Hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return m_pDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT IDirect3DDevice9Hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return m_pDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT IDirect3DDevice9Hook::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT IDirect3DDevice9Hook::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	return m_pDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT IDirect3DDevice9Hook::SetScissorRect(CONST RECT* pRect)
{
	return m_pDirect3DDevice9->SetScissorRect(pRect);
}

HRESULT IDirect3DDevice9Hook::GetScissorRect(RECT* pRect)
{
	return m_pDirect3DDevice9->GetScissorRect(pRect);
}

HRESULT IDirect3DDevice9Hook::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	return m_pDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware);
}

BOOL IDirect3DDevice9Hook::GetSoftwareVertexProcessing()
{
	return m_pDirect3DDevice9->GetSoftwareVertexProcessing();
}

HRESULT IDirect3DDevice9Hook::SetNPatchMode(float nSegments)
{
	return m_pDirect3DDevice9->SetNPatchMode(nSegments);
}

float IDirect3DDevice9Hook::GetNPatchMode()
{
	return m_pDirect3DDevice9->GetNPatchMode();
}

HRESULT IDirect3DDevice9Hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return m_pDirect3DDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT IDirect3DDevice9Hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	return m_pDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT IDirect3DDevice9Hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return m_pDirect3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT IDirect3DDevice9Hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return m_pDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT IDirect3DDevice9Hook::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	return m_pDirect3DDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT IDirect3DDevice9Hook::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDirect3DDevice9->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT IDirect3DDevice9Hook::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	return m_pDirect3DDevice9->SetVertexDeclaration(pDecl);
}

HRESULT IDirect3DDevice9Hook::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	return m_pDirect3DDevice9->GetVertexDeclaration(ppDecl);
}

HRESULT IDirect3DDevice9Hook::SetFVF(DWORD FVF)
{
	return m_pDirect3DDevice9->SetFVF(FVF);
}

HRESULT IDirect3DDevice9Hook::GetFVF(DWORD* pFVF)
{
	return m_pDirect3DDevice9->GetFVF(pFVF);
}

HRESULT IDirect3DDevice9Hook::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	return m_pDirect3DDevice9->CreateVertexShader(pFunction, ppShader);
}

HRESULT IDirect3DDevice9Hook::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	return m_pDirect3DDevice9->SetVertexShader(pShader);
}

HRESULT IDirect3DDevice9Hook::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	return m_pDirect3DDevice9->GetVertexShader(ppShader);
}

HRESULT IDirect3DDevice9Hook::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return m_pDirect3DDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT IDirect3DDevice9Hook::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return m_pDirect3DDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT IDirect3DDevice9Hook::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return m_pDirect3DDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT IDirect3DDevice9Hook::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return m_pDirect3DDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT IDirect3DDevice9Hook::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	return m_pDirect3DDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT IDirect3DDevice9Hook::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return m_pDirect3DDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT IDirect3DDevice9Hook::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	return m_pDirect3DDevice9->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT IDirect3DDevice9Hook::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
	return m_pDirect3DDevice9->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT IDirect3DDevice9Hook::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
{
	return m_pDirect3DDevice9->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT IDirect3DDevice9Hook::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
{
	return m_pDirect3DDevice9->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT IDirect3DDevice9Hook::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	return m_pDirect3DDevice9->SetIndices(pIndexData);
}

HRESULT IDirect3DDevice9Hook::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	return m_pDirect3DDevice9->GetIndices(ppIndexData);
}

HRESULT IDirect3DDevice9Hook::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	return m_pDirect3DDevice9->CreatePixelShader(pFunction, ppShader);
}

HRESULT IDirect3DDevice9Hook::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	return m_pDirect3DDevice9->SetPixelShader(pShader);
}

HRESULT IDirect3DDevice9Hook::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	return m_pDirect3DDevice9->GetPixelShader(ppShader);
}

HRESULT IDirect3DDevice9Hook::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	return m_pDirect3DDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT IDirect3DDevice9Hook::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	return m_pDirect3DDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT IDirect3DDevice9Hook::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	return m_pDirect3DDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT IDirect3DDevice9Hook::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	return m_pDirect3DDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT IDirect3DDevice9Hook::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	return m_pDirect3DDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT IDirect3DDevice9Hook::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	return m_pDirect3DDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT IDirect3DDevice9Hook::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pDirect3DDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT IDirect3DDevice9Hook::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT IDirect3DDevice9Hook::DeletePatch(UINT Handle)
{
	return m_pDirect3DDevice9->DeletePatch(Handle);
}

HRESULT IDirect3DDevice9Hook::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	return m_pDirect3DDevice9->CreateQuery(Type, ppQuery);
}
