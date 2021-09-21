
#include "stdafx.h"
#include "IDirect3DDevice8Hook.h"
#include <HackSupport.h>

extern IDirect3DDevice9* g_pD3D9Device;

IDirect3DDevice8Hook::IDirect3DDevice8Hook(IDirect3D8 * pD3D, IDirect3DDevice8 * pDevice)
{
	m_pD3D = pD3D;
	m_pDevice = pDevice;
	iRefs = 1;
	//pDevice->AddRef();
	//iRefs = pDevice->Release();
	//g_pClientGame->OnDeviceReset(this);
	{
		tHackEventDataD3D8 Data;
		Data.m_Size = sizeof(Data);
		Data.m_pD3DDevice = this;
		TriggerHackEvent(HACKEVENT_OND3D8DEVICESET, &Data);
	}
	{
		tHackEventDataD3D9 Data;
		Data.m_Size = sizeof(Data);
		Data.m_pD3DDevice = g_pD3D9Device;
		TriggerHackEvent(HACKEVENT_OND3D9DEVICESET, &Data);
	}
}

IDirect3DDevice8Hook::~IDirect3DDevice8Hook(void)
{

}

HRESULT IDirect3DDevice8Hook::QueryInterface(REFIID iid, void **ppvObject)
{
	return m_pDevice->QueryInterface(iid, ppvObject);
}

ULONG IDirect3DDevice8Hook::AddRef(void)
{
	iRefs++;
	return m_pDevice->AddRef();
}

ULONG IDirect3DDevice8Hook::Release(void)
{
	iRefs--;
	if (iRefs <= 0)
	{
		{
			tHackEventDataD3D9 Data;
			Data.m_Size = sizeof(Data);
			Data.m_pD3DDevice = g_pD3D9Device;
			TriggerHackEvent(HACKEVENT_OND3D9DEVICELOST, &Data);
		}

		{
			tHackEventDataD3D8 Data;
			Data.m_Size = sizeof(Data);
			Data.m_pD3DDevice = this;
			TriggerHackEvent(HACKEVENT_OND3D8DEVICELOST, &Data);
		}
	}
	ULONG ulResult = m_pDevice->Release();
	if (iRefs <= 0)
		delete this;
	return ulResult;
}

//extern DWORD g_dwResetDevice;

HRESULT IDirect3DDevice8Hook::TestCooperativeLevel(void)
{
	//if (g_dwResetDevice == 1)
	//{
	//	g_dwResetDevice = 2;
	//	return D3DERR_DEVICELOST;
	//}
	//else if (g_dwResetDevice == 2)
	//{
	//	g_dwResetDevice = 0;
	//	return D3DERR_DEVICENOTRESET;
	//}
	return m_pDevice->TestCooperativeLevel();
}

UINT IDirect3DDevice8Hook::GetAvailableTextureMem(void)
{
	return m_pDevice->GetAvailableTextureMem();
}

HRESULT IDirect3DDevice8Hook::ResourceManagerDiscardBytes(DWORD Bytes)
{
	return m_pDevice->ResourceManagerDiscardBytes(Bytes);
}

HRESULT IDirect3DDevice8Hook::GetDirect3D(IDirect3D8** ppD3D)
{
	return m_pDevice->GetDirect3D(ppD3D);
}

HRESULT IDirect3DDevice8Hook::GetDeviceCaps(D3DCAPS8* pCaps)
{
	return m_pDevice->GetDeviceCaps(pCaps);
}

HRESULT IDirect3DDevice8Hook::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
	return m_pDevice->GetDisplayMode(pMode);
}

HRESULT IDirect3DDevice8Hook::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
	return m_pDevice->GetCreationParameters(pParameters);
}

HRESULT IDirect3DDevice8Hook::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface8* pCursorBitmap)
{
	return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void IDirect3DDevice8Hook::SetCursorPosition(UINT X, UINT Y, DWORD Flags)
{
	m_pDevice->SetCursorPosition(X, Y, Flags);
}

BOOL IDirect3DDevice8Hook::ShowCursor(BOOL bShow)
{
	return m_pDevice->ShowCursor(bShow);
}

HRESULT IDirect3DDevice8Hook::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain8** ppSwapChain)
{
	return m_pDevice->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT IDirect3DDevice8Hook::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	{
		tHackEventDataD3D9 Data;
		Data.m_Size = sizeof(Data);
		Data.m_pD3DDevice = g_pD3D9Device;
		TriggerHackEvent(HACKEVENT_OND3D9DEVICELOST, &Data);
	}
	{
		tHackEventDataD3D8 Data;
		Data.m_Size = sizeof(Data);
		Data.m_pD3DDevice = this;
		TriggerHackEvent(HACKEVENT_OND3D8DEVICELOST, &Data);
	}
	HRESULT hr = m_pDevice->Reset(pPresentationParameters);
	if (SUCCEEDED(hr))
	{
		{
			tHackEventDataD3D8 Data;
			Data.m_Size = sizeof(Data);
			Data.m_pD3DDevice = this;
			TriggerHackEvent(HACKEVENT_OND3D8DEVICESET, &Data);
		}
		{
			tHackEventDataD3D9 Data;
			Data.m_Size = sizeof(Data);
			Data.m_pD3DDevice = g_pD3D9Device;
			TriggerHackEvent(HACKEVENT_OND3D9DEVICESET, &Data);
		}
	}
	return hr;
}

HRESULT IDirect3DDevice8Hook::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	return m_pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

HRESULT IDirect3DDevice8Hook::GetBackBuffer(UINT BackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface8** ppBackBuffer)
{
	return m_pDevice->GetBackBuffer(BackBuffer, Type, ppBackBuffer);
}

HRESULT IDirect3DDevice8Hook::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
	return m_pDevice->GetRasterStatus(pRasterStatus);
}

void IDirect3DDevice8Hook::SetGammaRamp(DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	m_pDevice->SetGammaRamp(Flags, pRamp);
}

void IDirect3DDevice8Hook::GetGammaRamp(D3DGAMMARAMP* pRamp)
{
	m_pDevice->GetGammaRamp(pRamp);
}

HRESULT IDirect3DDevice8Hook::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture8** ppTexture)
{
	return m_pDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture);
}

HRESULT IDirect3DDevice8Hook::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture8** ppVolumeTexture)
{
	return m_pDevice->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture);
}

HRESULT IDirect3DDevice8Hook::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture8** ppCubeTexture)
{
	return m_pDevice->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture);
}

HRESULT IDirect3DDevice8Hook::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer8** ppVertexBuffer)
{
	return m_pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer);
}

HRESULT IDirect3DDevice8Hook::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer8** ppIndexBuffer)
{
	return m_pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer);
}

HRESULT IDirect3DDevice8Hook::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, BOOL Lockable, IDirect3DSurface8** ppSurface)
{
	return m_pDevice->CreateRenderTarget(Width, Height, Format, MultiSample, Lockable, ppSurface);
}

HRESULT IDirect3DDevice8Hook::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface8** ppSurface)
{
	return m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, ppSurface);
}

HRESULT IDirect3DDevice8Hook::CreateImageSurface(UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface8** ppSurface)
{
	return m_pDevice->CreateImageSurface(Width, Height, Format, ppSurface);
}

HRESULT IDirect3DDevice8Hook::CopyRects(IDirect3DSurface8* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray)
{
	return m_pDevice->CopyRects(pSourceSurface, pSourceRectsArray, cRects, pDestinationSurface, pDestPointsArray);
}

HRESULT IDirect3DDevice8Hook::UpdateTexture(IDirect3DBaseTexture8* pSourceTexture, IDirect3DBaseTexture8* pDestinationTexture)
{
	return m_pDevice->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT IDirect3DDevice8Hook::GetFrontBuffer(IDirect3DSurface8* pDestSurface)
{
	return m_pDevice->GetFrontBuffer(pDestSurface);
}

HRESULT IDirect3DDevice8Hook::SetRenderTarget(IDirect3DSurface8* pRenderTarget, IDirect3DSurface8* pNewZStencil)
{
	return m_pDevice->SetRenderTarget(pRenderTarget, pNewZStencil);
}

HRESULT IDirect3DDevice8Hook::GetRenderTarget(IDirect3DSurface8** ppRenderTarget)
{
	return m_pDevice->GetRenderTarget(ppRenderTarget);
}

HRESULT IDirect3DDevice8Hook::GetDepthStencilSurface(IDirect3DSurface8** ppZStencilSurface)
{
	return m_pDevice->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT IDirect3DDevice8Hook::BeginScene(void)
{
	return m_pDevice->BeginScene();
}

HRESULT IDirect3DDevice8Hook::EndScene(void)
{	
	return m_pDevice->EndScene();
}

HRESULT IDirect3DDevice8Hook::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	return m_pDevice->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT IDirect3DDevice8Hook::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pDevice->SetTransform(State, pMatrix);
}

HRESULT IDirect3DDevice8Hook::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	return m_pDevice->GetTransform(State, pMatrix);
}

HRESULT IDirect3DDevice8Hook::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	return m_pDevice->MultiplyTransform(State, pMatrix);
}

HRESULT IDirect3DDevice8Hook::SetViewport(CONST D3DVIEWPORT8* pViewport)
{
	return m_pDevice->SetViewport(pViewport);
}

HRESULT IDirect3DDevice8Hook::GetViewport(D3DVIEWPORT8* pViewport )
{
	return m_pDevice->GetViewport(pViewport);
}

HRESULT IDirect3DDevice8Hook::SetMaterial(CONST D3DMATERIAL8* pMaterial)
{
	return m_pDevice->SetMaterial(pMaterial);
}

HRESULT IDirect3DDevice8Hook::GetMaterial(D3DMATERIAL8* pMaterial)
{
	return m_pDevice->GetMaterial(pMaterial);
}

HRESULT IDirect3DDevice8Hook::SetLight(DWORD Index, CONST D3DLIGHT8* pLight)
{
	return m_pDevice->SetLight(Index, pLight);
}

HRESULT IDirect3DDevice8Hook::GetLight(DWORD Index, D3DLIGHT8* pLight)
{
	return m_pDevice->GetLight(Index, pLight);
}

HRESULT IDirect3DDevice8Hook::LightEnable(DWORD LightIndex, BOOL bEnable)
{
	return m_pDevice->LightEnable(LightIndex, bEnable);
}

HRESULT IDirect3DDevice8Hook::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	return m_pDevice->GetLightEnable(Index, pEnable);
}

HRESULT IDirect3DDevice8Hook::SetClipPlane(DWORD Index, CONST float* pPlane )
{
	return m_pDevice->SetClipPlane(Index, pPlane);
}

HRESULT IDirect3DDevice8Hook::GetClipPlane(DWORD Index, float* pPlane)
{
	return m_pDevice->GetClipPlane(Index, pPlane);
}

HRESULT IDirect3DDevice8Hook::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	//CDeviceStateBackup::OnRenderStateSet(State, Value);
	return m_pDevice->SetRenderState(State, Value);
}

HRESULT IDirect3DDevice8Hook::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	return m_pDevice->GetRenderState(State, pValue);
}

HRESULT IDirect3DDevice8Hook::BeginStateBlock(void)
{
	return m_pDevice->BeginStateBlock();
}

HRESULT IDirect3DDevice8Hook::EndStateBlock(DWORD* pToken)
{
	return m_pDevice->EndStateBlock(pToken);
}

HRESULT IDirect3DDevice8Hook::ApplyStateBlock(DWORD Token)
{
	return m_pDevice->ApplyStateBlock(Token);
}

HRESULT IDirect3DDevice8Hook::CaptureStateBlock(DWORD Token)
{
	return m_pDevice->CaptureStateBlock(Token);
}

HRESULT IDirect3DDevice8Hook::DeleteStateBlock(DWORD Token)
{
	return m_pDevice->DeleteStateBlock(Token);
}

HRESULT IDirect3DDevice8Hook::CreateStateBlock(D3DSTATEBLOCKTYPE Type, DWORD* pToken)
{
	return m_pDevice->CreateStateBlock(Type, pToken);
}

HRESULT IDirect3DDevice8Hook::SetClipStatus(CONST D3DCLIPSTATUS8* pClipStatus)
{
	return m_pDevice->SetClipStatus(pClipStatus);
}

HRESULT IDirect3DDevice8Hook::GetClipStatus(D3DCLIPSTATUS8* pClipStatus)
{
	return m_pDevice->GetClipStatus(pClipStatus);
}

HRESULT IDirect3DDevice8Hook::GetTexture(DWORD Stage, IDirect3DBaseTexture8** ppTexture)
{
	return m_pDevice->GetTexture(Stage, ppTexture);
}

HRESULT IDirect3DDevice8Hook::SetTexture(DWORD Stage, IDirect3DBaseTexture8* pTexture)
{
	return m_pDevice->SetTexture(Stage, pTexture);
}

HRESULT IDirect3DDevice8Hook::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	return m_pDevice->GetTextureStageState(Stage, Type, pValue);
}

HRESULT IDirect3DDevice8Hook::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type,DWORD Value)
{
	//CDeviceStateBackup::OnTextureStageStateSet(Stage,Type,Value);
	return m_pDevice->SetTextureStageState(Stage, Type, Value);
}

HRESULT IDirect3DDevice8Hook::ValidateDevice(DWORD* pNumPasses)
{
	return m_pDevice->ValidateDevice(pNumPasses);
}

HRESULT IDirect3DDevice8Hook::GetInfo(DWORD DevInfoID, VOID* pDevInfoStruct, DWORD DevInfoStructSize)
{
	return m_pDevice->GetInfo(DevInfoID, pDevInfoStruct, DevInfoStructSize);
}

HRESULT IDirect3DDevice8Hook::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	return m_pDevice->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT IDirect3DDevice8Hook::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	return m_pDevice->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT IDirect3DDevice8Hook::SetCurrentTexturePalette(UINT PaletteNumber)
{
	return m_pDevice->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT IDirect3DDevice8Hook::GetCurrentTexturePalette(UINT* pPaletteNumber)
{
	return m_pDevice->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT IDirect3DDevice8Hook::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	return m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT IDirect3DDevice8Hook::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, UINT MinIndex, UINT NumVertices, UINT StartIndex, UINT PrimitiveCount)
{
	return m_pDevice->DrawIndexedPrimitive(Type, MinIndex, NumVertices, StartIndex, PrimitiveCount);
}

HRESULT IDirect3DDevice8Hook::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT IDirect3DDevice8Hook::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	return m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT IDirect3DDevice8Hook::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer8* pDestBuffer, DWORD Flags)
{
	return m_pDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, Flags);
}

HRESULT IDirect3DDevice8Hook::CreateVertexShader(CONST DWORD* pDeclaration, CONST DWORD* pFunction, DWORD* pHandle, DWORD Usage)
{
	return m_pDevice->CreateVertexShader(pDeclaration, pFunction, pHandle, Usage);
}

HRESULT IDirect3DDevice8Hook::SetVertexShader(DWORD Handle)
{
	return m_pDevice->SetVertexShader(Handle);
}

HRESULT IDirect3DDevice8Hook::GetVertexShader(DWORD* pHandle)
{
	return m_pDevice->GetVertexShader(pHandle);
}

HRESULT IDirect3DDevice8Hook::DeleteVertexShader(DWORD Handle)
{
	return m_pDevice->DeleteVertexShader(Handle);
}

HRESULT IDirect3DDevice8Hook::SetVertexShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->SetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IDirect3DDevice8Hook::GetVertexShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->GetVertexShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IDirect3DDevice8Hook::GetVertexShaderDeclaration(DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return m_pDevice->GetVertexShaderDeclaration(Handle, pData, pSizeOfData);
}

HRESULT IDirect3DDevice8Hook::GetVertexShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return m_pDevice->GetVertexShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT IDirect3DDevice8Hook::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8* pStreamData, UINT Stride)
{
	return m_pDevice->SetStreamSource(StreamNumber, pStreamData, Stride);
}

HRESULT IDirect3DDevice8Hook::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer8** ppStreamData, UINT* pStride)
{
	return m_pDevice->GetStreamSource(StreamNumber, ppStreamData, pStride);
}

HRESULT IDirect3DDevice8Hook::SetIndices(IDirect3DIndexBuffer8* pIndexData, UINT BaseVertexIndex)
{
	return m_pDevice->SetIndices(pIndexData, BaseVertexIndex);
}

HRESULT IDirect3DDevice8Hook::GetIndices(IDirect3DIndexBuffer8** ppIndexData, UINT* pBaseVertexIndex)
{
	return m_pDevice->GetIndices(ppIndexData, pBaseVertexIndex);
}

HRESULT IDirect3DDevice8Hook::CreatePixelShader(CONST DWORD* pFunction, DWORD* pHandle)
{
	return m_pDevice->CreatePixelShader(pFunction, pHandle);
}

HRESULT IDirect3DDevice8Hook::SetPixelShader(DWORD Handle)
{
	return m_pDevice->SetPixelShader(Handle);
}

HRESULT IDirect3DDevice8Hook::GetPixelShader(DWORD* pHandle)
{
	return m_pDevice->GetPixelShader(pHandle);
}

HRESULT IDirect3DDevice8Hook::DeletePixelShader(DWORD Handle)
{
	return m_pDevice->DeletePixelShader(Handle);
}

HRESULT IDirect3DDevice8Hook::SetPixelShaderConstant(DWORD Register, CONST void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->SetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IDirect3DDevice8Hook::GetPixelShaderConstant(DWORD Register, void* pConstantData, DWORD ConstantCount)
{
	return m_pDevice->GetPixelShaderConstant(Register, pConstantData, ConstantCount);
}

HRESULT IDirect3DDevice8Hook::GetPixelShaderFunction(DWORD Handle, void* pData, DWORD* pSizeOfData)
{
	return m_pDevice->GetPixelShaderFunction(Handle, pData, pSizeOfData);
}

HRESULT IDirect3DDevice8Hook::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	return m_pDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT IDirect3DDevice8Hook::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	return m_pDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT IDirect3DDevice8Hook::DeletePatch(UINT Handle)
{
	return m_pDevice->DeletePatch(Handle);
}
