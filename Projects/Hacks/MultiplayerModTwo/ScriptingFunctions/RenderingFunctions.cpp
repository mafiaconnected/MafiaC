#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"

using namespace Galactic3D;

static bool FunctionLoadBMPII(IScriptState* pState, int argc, void* pUser)
{
	Stream* pStream = pState->CheckStream(0);
	if (!pStream)
		return false;
	auto pTexture = Strong<Texture>::New(Texture::CreateFromBMP(pStream,false));
	pTexture->m_pReflectedClass = g_pClientGame->m_pClientManager->m_pTextureClass;
	pState->ReturnObject(pTexture);
	return true;
}

static bool FunctionLoadPNGII(IScriptState* pState, int argc, void* pUser)
{
	Stream* pStream = pState->CheckStream(0);
	if (!pStream)
		return false;
	auto pTexture = Strong<Texture>::New(Texture::CreateFromPNG(pStream,false));
	pTexture->m_pReflectedClass = g_pClientGame->m_pClientManager->m_pTextureClass;
	pState->ReturnObject(pTexture);
	return true;
}

static bool FunctionDrawRectangleII(IScriptState* pState, int argc, void* pUser)
{
	Surface* pSurface;
	if (!pState->CheckClass(g_pClientGame->m_pClientManager->m_pSurfaceClass,0,true,&pSurface))
		return false;
	CVector2D vecPos;
	if (!pState->CheckVector2D(1,vecPos))
		return false;
	CVector2D vecSize;
	if (!pState->CheckVector2D(2,vecSize))
		return false;
	unsigned int uiColour1 = COLOUR::White;
	if (argc > 3 && !pState->CheckNumber(3,uiColour1))
		return false;
	unsigned int uiColour2 = COLOUR::White;
	if (argc > 4 && !pState->CheckNumber(4,uiColour2))
		return false;
	unsigned int uiColour3 = COLOUR::White;
	if (argc > 5 && !pState->CheckNumber(5,uiColour3))
		return false;
	unsigned int uiColour4 = COLOUR::White;
	if (argc > 6 && !pState->CheckNumber(6,uiColour4))
		return false;
	float fRotation = 0.0f;
	if (argc > 7 && !pState->CheckNumber(7,fRotation))
		return false;
	CVector2D vecCentre(0,0);
	if (argc > 8 && !pState->CheckVector2D(8,vecCentre))
		return false;
	CVector2D vecSourcePos(0,0);
	if (argc > 9 && !pState->CheckVector2D(9,vecSourcePos))
		return false;
	CVector2D vecSourceSize(1.0f,1.0f);
	if (argc > 10 && !pState->CheckVector2D(10,vecSourceSize))
		return false;
	CVector2D vecScale(1.0f,1.0f);
	if (argc > 11 && !pState->CheckVector2D(11,vecScale))
		return false;
	CBufferedRectangle Rectangle(vecPos, vecSize, uiColour1, uiColour2, uiColour3, uiColour4, fRotation, vecCentre, vecSourcePos, vecSourceSize, vecScale);
	auto pMaterial = Strong<Material>::New(new Material);
	pMaterial->SetCullMode(CULLMODE_NONE);
	pMaterial->SetTextureWrap(TEXTUREWRAP_CLAMP, TEXTUREWRAP_CLAMP);
	pMaterial->SetTexture(pSurface);
	g_pClientGame->m_pGalacticFunctions->m_p2D->DrawRectangle(pMaterial, Rectangle);
	return true;
}

static bool FunctionGetCursorPositionII(IScriptState* pState, int argc, void* pUser)
{
	if (g_pClientGame->IsCursorEnabled())
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		pState->ReturnVector2D(CVector2D((float)x, (float)y));
	}
	else
		pState->ReturnNull();
	return true;
}

static bool FunctionIsCursorEnabledII(IScriptState* pState, int argc, void* pUser)
{
	pState->ReturnBoolean(g_pClientGame->IsCursorEnabled());
	return true;
}

static bool FunctionShowCursorII(IScriptState* pState, int argc, void* pUser)
{
	bool bEnabled;
	if (!pState->CheckBoolean(0,bEnabled))
		return false;
	bool bToggleControls = true;
	if (argc > 1 && !pState->CheckBoolean(1,bToggleControls))
		return false;
	//pState->GetResource()->m_bCursorEnabled = bEnabled;
	g_pClientGame->SetCursorEnabled(bEnabled);
	//g_pClientGame->UpdateCursorEnabled();
	if (bToggleControls)
		g_pClientGame->LockControls(!bToggleControls);
	return true;
}

void CScriptingFunctionsII::RegisterRenderingFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame)
{
	auto pDrawing = pScripting->m_Global.AddNamespace(_gstr("graphics"));
	pDrawing->RegisterFunction(_gstr("loadBMP"), _gstr("x"), FunctionLoadBMPII, pClientGame);
	pDrawing->RegisterFunction(_gstr("loadPNG"), _gstr("x"), FunctionLoadPNGII, pClientGame);
	pDrawing->RegisterFunction(_gstr("drawRectangle"), _gstr("xzz|iiiifzzzz"), FunctionDrawRectangleII, pClientGame);
	pDrawing->SetAlias(_gstr("drawing"));

	auto pGUI = pScripting->m_Global.AddNamespace(_gstr("gui"));
	pGUI->AddProperty(pClientGame, _gstr("cursorPosition"), ARGUMENT_VECTOR2D, FunctionGetCursorPositionII);
	pGUI->AddProperty(pClientGame, _gstr("cursorEnabled"), ARGUMENT_BOOLEAN, FunctionIsCursorEnabledII);
	pGUI->RegisterFunction(_gstr("showCursor"), _gstr("b|b"), FunctionShowCursorII, pClientGame);
}
