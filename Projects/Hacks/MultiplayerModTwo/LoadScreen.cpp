#include "stdafx.h"
#include "LoadScreen.h"
#include "ClientGame.h"

Galactic3D::Texture* backgroundTex;

float CLoadScreen::loadProgress = 0;

void CLoadScreen::UpdateProgress(float progress)
{
	loadProgress = progress;
}

// To-do: Make the loadscreen actually draw itself

void CLoadScreen::LoadResources()
{
	//auto pStream = Strong<Stream>::New(g_pContext->GetFileSystem()->Open(_gstr("/GUI/Images/LoadScreen.png"), false));
	//if (pStream)
	//{
	//	backgroundTex = Texture::CreateFromPNG(pStream, false);
	//	if (backgroundTex)
	//	{
	//		backgroundTex->SetName(_gstr("LoadScreen.png"));
	//	}
	//}
}

void CLoadScreen::Draw(Galactic3D::C2D* p2D)
{
	//if (!p2D && !backgroundTex) return;

	//if (loadProgress > 0.0f && loadProgress < 1.0f)
	//{
	//	float scrWidth = (float)MafiaSDK::GetIGraph()->Scrn_sx();
	//	float scrHeight = (float)MafiaSDK::GetIGraph()->Scrn_sy();

	//	p2D->Draw2DImage(backgroundTex, { 0, 0 }, { scrWidth, scrHeight });
	//}
}