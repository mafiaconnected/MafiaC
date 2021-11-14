
#include "pch.h"
#include "resource.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

tHack* g_pHack = nullptr;

static WNDPROC DefaultWndProc;
static WNDPROC SDLWndProc;

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// These window messages are messing up the window within SDL's WndProc so filter them out
static LRESULT CALLBACK SDLHackWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message) 
	{
		//case WM_SHOWWINDOW:
		//case WM_NCACTIVATE:
	case WM_ACTIVATE://SDL handling this breaks bully
	//case WM_GETMINMAXINFO:
	//case WM_WINDOWPOSCHANGING:
	//case WM_WINDOWPOSCHANGED:
	//case WM_SIZE:
	//case WM_PAINT:
	//case WM_ERASEBKGND:
	//case WM_DROPFILES:
	//case WM_NCCALCSIZE:
	//case WM_NCHITTEST:
		return WndProc(hWnd, message, wParam, lParam);
	}
	return CallWindowProc(SDLWndProc, hWnd, message, wParam, lParam);
}

static SDL_Window* g_pWindow;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	tHackEventDataWindowProcedure Event;
	Event.m_Size = sizeof(Event);
	Event.m_hWnd = hWnd;
	Event.m_Msg = message;
	Event.m_wParam = wParam;
	Event.m_lParam = lParam;
	Event.m_Result = 0;
	switch (message) {
	case WM_NCCREATE:
	{
		GHWND = hWnd;
		SDL_InitSubSystem(SDL_INIT_VIDEO);

		// Make an SDL window
		SDL_Window* pWindow = SDL_CreateWindowFrom(hWnd);
		g_pWindow = pWindow;

		SDLWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)SDLHackWndProc);

		SDL_ShowCursor(SDL_DISABLE);
	}
	break;
	case WM_CREATE:
	{
		GChar szTitle[128];
		_gsnprintf(szTitle, ARRAY_COUNT(szTitle), _gstr("%s %s"), CHackSupport::m_pInstance->m_InjectedData.m_InjectData.m_LauncherData.m_Launcher.m_szTitle, CHackSupport::m_pInstance->m_InjectedData.m_InjectData.m_LauncherData.m_Launcher.m_Version.m_szDisplayVersion);

		// Set title
		UTF16String Title2(false, szTitle);
		SetWindowTextW(hWnd, Title2);
	}
	break;
	case WM_NCDESTROY:
	{
		if (GHWND == hWnd)
			GHWND = NULL;

		// Destroy SDL window
		if (g_pWindow != nullptr)
			SDL_DestroyWindow(g_pWindow);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		g_pWindow = nullptr;
	}
	break;
	case WM_SETCURSOR:
		if (LOWORD(lParam) == HTCLIENT) // HACK: SDL doesn't tell us the return value from their WndProc...
			return 1;
		return DefWindowProcA(hWnd, message, wParam, lParam);
		break;
	default:
		break;
	}
	HACKEVENTRESULT Result = TriggerHackEvent(HACKEVENT_WNDPROC, &Event);
	if ((Result & HACKEVENTRESULT_PREVENTDEFAULT) != HACKEVENTRESULT_PREVENTDEFAULT) 
	{
		LRESULT Result2 = DefaultWndProc(hWnd, message, wParam, lParam);
		if ((Result & HACKEVENTRESULT_RESULTSET) != HACKEVENTRESULT_RESULTSET)
			Event.m_Result = Result2;
	}
	TriggerHackEvent(HACKEVENT_POSTWNDPROC, &Event);
	return Event.m_Result;
}

static HICON GetIcon(HMODULE hModule, bool bSmall) 
{
	return (HICON)LoadImage(hModule, MAKEINTRESOURCE(IDI_LAUNCHER), IMAGE_ICON, GetSystemMetrics(bSmall ? SM_CXSMICON : SM_CXICON), GetSystemMetrics(bSmall ? SM_CYSMICON : SM_CYICON), LR_DEFAULTCOLOR);
}

static void GetIcons(HICON& hLargeIcon, HICON& hSmallIcon) {
	auto hModule = LoadLibraryEx(CHackSupport::m_pInstance->m_InjectedData.m_InjectData.m_LauncherExePath.c_str(), nullptr, LOAD_LIBRARY_AS_DATAFILE);
	if (hModule != nullptr) {
		hLargeIcon = GetIcon(hModule, false);
		hSmallIcon = GetIcon(hModule, true);
		FreeModule(hModule);
	}
}

ATOM WINAPI HookRegisterClassExA(const WNDCLASSEXA* lpWndClass) 
{
	WNDCLASSEXA WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = lpWndClass->style;
	DefaultWndProc = lpWndClass->lpfnWndProc;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = lpWndClass->cbClsExtra;
	WndClass.cbWndExtra = lpWndClass->cbWndExtra;
	WndClass.hInstance = lpWndClass->hInstance;
	WndClass.hIcon = lpWndClass->hIcon;
	WndClass.hCursor = lpWndClass->hCursor;
	WndClass.hbrBackground = lpWndClass->hbrBackground;
	WndClass.lpszMenuName = lpWndClass->lpszMenuName;
	WndClass.lpszClassName = lpWndClass->lpszClassName;
	WndClass.hIconSm = lpWndClass->hIcon;
	GetIcons(WndClass.hIcon, WndClass.hIconSm);
	ATOM Result = 0;
	
	if (g_pWindow != nullptr)
		Result = RegisterClassExA(lpWndClass);
	else
		Result = RegisterClassExA(&WndClass);
	
	if (Result == 0)
		Alert(MESSAGEBOXICON_ERROR, _gstr("Failed to register the main window class."), CHackSupport::m_pInstance->m_pContext->GetGameName());
	
	return Result;
}

/*ATOM WINAPI HookRegisterClassExW(const WNDCLASSEXW* lpWndClass) {
	WNDCLASSEXW WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = lpWndClass->style;
	DefaultWndProc = lpWndClass->lpfnWndProc;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = lpWndClass->cbClsExtra;
	WndClass.cbWndExtra = lpWndClass->cbWndExtra;
	WndClass.hInstance = lpWndClass->hInstance;
	WndClass.hIcon = lpWndClass->hIcon;
	WndClass.hCursor = lpWndClass->hCursor;
	WndClass.hbrBackground = lpWndClass->hbrBackground;
	WndClass.lpszMenuName = lpWndClass->lpszMenuName;
	WndClass.lpszClassName = lpWndClass->lpszClassName;
	WndClass.hIconSm = lpWndClass->hIcon;
	GetIcons(WndClass.hIcon, WndClass.hIconSm);
	ATOM Result = RegisterClassExW(&WndClass);
	if (Result == 0)
		Alert(MESSAGEBOXICON_ERROR, _gstr("Failed to register the main window class."), CHackSupport::m_pInstance->m_pContext->GetGameName());
	return Result;
}

ATOM WINAPI HookRegisterClassW(const WNDCLASSW* lpWndClass) {
	WNDCLASSEXW WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = lpWndClass->style;
	DefaultWndProc = lpWndClass->lpfnWndProc;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = lpWndClass->cbClsExtra;
	WndClass.cbWndExtra = lpWndClass->cbWndExtra;
	WndClass.hInstance = lpWndClass->hInstance;
	WndClass.hIcon = lpWndClass->hIcon;
	WndClass.hCursor = lpWndClass->hCursor;
	WndClass.hbrBackground = lpWndClass->hbrBackground;
	WndClass.lpszMenuName = lpWndClass->lpszMenuName;
	WndClass.lpszClassName = lpWndClass->lpszClassName;
	WndClass.hIconSm = lpWndClass->hIcon;
	GetIcons(WndClass.hIcon, WndClass.hIconSm);
	ATOM Result = RegisterClassExW(&WndClass);
	if (Result == 0)
		Alert(MESSAGEBOXICON_ERROR, _gstr("Failed to register the main window class."), CHackSupport::m_pInstance->m_pContext->GetGameName());
	return Result;
}

ATOM WINAPI HookRegisterClassA(const WNDCLASSA* lpWndClass) {
	WNDCLASSEXA WndClass;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = lpWndClass->style;
	DefaultWndProc = lpWndClass->lpfnWndProc;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = lpWndClass->cbClsExtra;
	WndClass.cbWndExtra = lpWndClass->cbWndExtra;
	WndClass.hInstance = lpWndClass->hInstance;
	WndClass.hIcon = lpWndClass->hIcon;
	WndClass.hCursor = lpWndClass->hCursor;
	WndClass.hbrBackground = lpWndClass->hbrBackground;
	WndClass.lpszMenuName = lpWndClass->lpszMenuName;
	WndClass.lpszClassName = lpWndClass->lpszClassName;
	WndClass.hIconSm = lpWndClass->hIcon;
	GetIcons(WndClass.hIcon, WndClass.hIconSm);
	ATOM Result = RegisterClassExA(&WndClass);
	if (Result == 0)
		Alert(MESSAGEBOXICON_ERROR, _gstr("Failed to register the main window class."), CHackSupport::m_pInstance->m_pContext->GetGameName());
	return Result;
}*/

HWND WINAPI HookCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) 
{
	if ((dwStyle & WS_CAPTION) != 0)
	{
		dwStyle |= WS_SYSMENU;
		dwStyle |= WS_MINIMIZEBOX;
	}
	return CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

/*HWND WINAPI HookCreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
	if ((dwStyle & WS_CAPTION) != 0) {
		dwStyle |= WS_SYSMENU;
		dwStyle |= WS_MINIMIZEBOX;
	}
	return CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}*/

int WINAPI HookShowCursor(BOOL bShow) 
{
	return 0;
}

HCURSOR WINAPI HookSetCursor(HCURSOR hCursor) 
{
	return nullptr;
}

static void InitialiseWindowHacks() 
{
	HMODULE hEngineModule = GetModuleHandleA("LS3DF.dll");
	CHacks::GetImportLookups(hEngineModule, "user32.dll", [&](uint16_t Ordinal, const char* pszName, void** ppFunction) {
		if (pszName != nullptr) {
			/*if (strcmp(pszName, "RegisterClassA") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookRegisterClassA);
			else if (strcmp(pszName, "RegisterClassW") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookRegisterClassW);*/
			if (strcmp(pszName, "RegisterClassExA") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookRegisterClassExA);
			/*else if (strcmp(pszName, "RegisterClassExW") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookRegisterClassExW);*/
			else if (strcmp(pszName, "CreateWindowExA") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookCreateWindowExA);
			/*else if (strcmp(pszName, "CreateWindowExW") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookCreateWindowExW);*/
			else if (strcmp(pszName, "ShowCursor") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookShowCursor);
			else if (strcmp(pszName, "SetCursor") == 0)
				new CHackValueHack(g_pHack, ppFunction, sizeof(void*), &HookSetCursor);
		}
		return true;
	});
}

bool g_bHackEventTriggered_Load = false;
HACKEVENTRESULT HackMain(uint32_t Event, tHackEventData* pData) 
{
	switch (Event) {
	case HACKEVENT_LOAD:
	{
		if (g_bHackEventTriggered_Load)
			return HACKEVENTRESULT_NORMAL;
		g_bHackEventTriggered_Load = true;
		tHackEventDataLoad* pData2 = static_cast<tHackEventDataLoad*>(pData);
		g_pHack = pData2->m_pHack;
		InitialiseWindowHacks();
		return HACKEVENTRESULT_NORMAL;
	}
	break;
	case HACKEVENT_UNLOAD:
		return HACKEVENTRESULT_NORMAL;
		break;
	default:
		break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}
