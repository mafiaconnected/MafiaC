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

	virtual bool VerifyGameEXE(tGame& Game, const IMAGE_DOS_HEADER& DOSHeader, const IMAGE_NT_HEADERS32& NTHeader, Stream* pStream, bool& bShowError) override;
};
