#pragma once
#include <LucasGUI/GUI/GUI.h>
#include <LucasGUI/LanguageLocalisation.h>
#include <LucasGUI/Fonts.h>
#include <LucasGUI/DumbUtil.h>
#include <LucasGUI/HtmlView.h>
#include <LucasFont/LucasFont.h>

class CLoadScreen
{
private:
	static float loadProgress;

public:
	static void LoadResources();

	static void UpdateProgress(float progress);

	static void Draw(Galactic3D::C2D* p2D);
};