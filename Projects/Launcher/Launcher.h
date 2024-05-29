#pragma once

#include <ModLauncher/Launcher.h>

class CMafiaCModLauncher : public CModLauncher
{
public:
	CMafiaCModLauncher(HINSTANCE hInstance);

	int WinMain();

	virtual HANDLE BeginStartingGame(HMODULE hHackSupport, const wchar_t* pszGamePath, const wchar_t* pszGameEXEPath, const wchar_t* pszGameCommandLine, CInjectedData::tLauncherData* pLauncherData, bool& bError) override;

	virtual bool Initialise(Context* pContext) override;
	virtual void ShutDown() override;
};
