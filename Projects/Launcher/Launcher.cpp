
#include "pch.h"
#include "Launcher.h"
#include <Devices/Direct3D9/Plugin.h>
#include <Devices/Direct3D11/Plugin.h>
#include <SDLApp/SDLApp.h>
#if !PUBLIC_RELEASE
#include <ModLauncher/LauncherTest.h>
#endif
#include <crtdbg.h>
#include <LauncherVersion.h>
#include <AdditionalPackets.h>
#include <GameVersion.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <ModLauncher/ProgressWindow.h>

CMafiaCModLauncher::CMafiaCModLauncher(HINSTANCE hInstance) : CModLauncher(hInstance)
{
#if PUBLIC_RELEASE
	m_bCodeSignatureCheck = true;
	m_bAllowMap = false;
	m_bAllowLargeIcons = false;
	m_bAllowLicenses = false;
#else
	m_bCodeSignatureCheck = false;
	m_bAllowMap = true;
	m_bAllowLargeIcons = true;
	m_bAllowLicenses = true;
#endif

	m_Launcher.m_Version.m_uiNetVersion = NETGAME_CURRENT_VERSION;

	_gstrlcpy(m_szServerListing, _gstr("serverlisting.mafiaconnected.com"), ARRAY_COUNT(m_szServerListing));
	_gstrlcpy(m_szHacksFolder, _gstr(""), ARRAY_COUNT(m_szHacksFolder));

	_gstrcpy_s(m_Launcher.m_szName, ARRAY_COUNT(m_Launcher.m_szName), _gstr("MafiaC"));
	_gstrcpy_s(m_Launcher.m_szTitle, ARRAY_COUNT(m_Launcher.m_szTitle), _gstr("Mafia Connected"));
	_gstrcpy_s(m_Launcher.m_szWebsite, ARRAY_COUNT(m_Launcher.m_szWebsite), _gstr("https://mafiaconnected.com/"));
	_gstrcpy_s(m_Launcher.m_szPatreon, ARRAY_COUNT(m_Launcher.m_szPatreon), _gstr("jack9267"));
	_gstrcpy_s(m_Launcher.m_szProtocol, ARRAY_COUNT(m_Launcher.m_szProtocol), _gstr("mafiac"));

#ifndef _DEBUG
#if PUBLIC_RELEASE
	_gstrcpy_s(m_Launcher.m_szUpdateURL, ARRAY_COUNT(m_Launcher.m_szUpdateURL), _gstr("https://ota.mafiaconnected.com/Launcher-Default.xml"));
#else
	_gstrcpy_s(m_Launcher.m_szUpdateURL, ARRAY_COUNT(m_Launcher.m_szUpdateURL), _gstr("https://ota.mafiaconnected.com/Launcher-Beta.xml"));
#endif
#endif

	m_NetGamesList.Register(_gstr("mafia:one"), GAME_MAFIA_ONE);
	m_NetGamesList.Register(_gstr("mafia:two"), GAME_MAFIA_TWO);
	m_NetGamesList.Register(_gstr("mafia:three"), GAME_MAFIA_THREE);
	m_NetGamesList.Register(_gstr("mafia:one_de"), GAME_MAFIA_ONE_DE);
}

int CMafiaCModLauncher::WinMain()
{
	try
	{
		Context Context(m_hInstance);

		try
		{
			WinUtil::AttachParentConsole();
		}
		catch (std::exception& e)
		{
			XTHROW_RESUMEABLE(e);
		}

		RegisterDirect3D11DeviceFactory(&Context);
		RegisterDirect3D9DeviceFactory(&Context);

		if (!Initialise(&Context))
			return EXIT_FAILURE;

		int iResult = EXIT_SUCCESS;

		if (m_bNewLauncher)
		{
#if !PUBLIC_RELEASE
			IApp* pApp = CreateSDLApp(&Context);
			IApp::StartupInfo si = { 0 };
			si.m_Size = sizeof(si);
			si.m_nFlags = IApp::FLAGS_RESIZABLE | IApp::FLAGS_ALLOWFULLSCREEN | IApp::FLAGS_AUTOQUIT | IApp::FLAGS_MOUSE | IApp::FLAGS_CENTERED | IApp::FLAGS_SIZE;
			si.m_nWidth = 750;
			si.m_nHeight = 550;
			CLauncherTest Subscriber(this);
			pApp->Main(&Subscriber, &si);
			delete pApp;
#endif
		}
		else
		{
			iResult = Main();
		}

		ShutDown();
		return iResult;
	}
	catch (std::exception& e)
	{
		Context::ShowExceptionMessage(e, false);
		return EXIT_FAILURE;
	}
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	{
		_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_WNDW);
	}

	CMafiaCModLauncher* pModLauncher = new CMafiaCModLauncher(hInstance);

	int iResult = pModLauncher->WinMain();

	delete pModLauncher;

	return iResult;
}
