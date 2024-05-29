
#include "stdafx.h"
#include <HackSupport.h>
#include "ScriptingFunctions.h"
#include "ClientGame.h"
#include "resource.h"
#include <Engine/GalacticLexer.h>

using namespace Galactic3D;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

void CScriptingFunctionsII::RegisterDefines(Galactic3D::CDefineHandlers* pDefineHandlers)
{
	RegisterGameDefines(pDefineHandlers);
	RegisterUtilDefines(pDefineHandlers);
}

void CScriptingFunctionsII::RegisterFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame)
{
	auto pMafia = pScripting->m_Global.AddNamespace(_gstr("mafia"));
	pMafia->SetAlias(_gstr("game"));

	RegisterGameFunctions(pScripting,pClientGame);

	// Class functions
	RegisterEntityFunctions(pScripting, pClientGame);
	RegisterHumanFunctions(pScripting, pClientGame);
	//RegisterPlayerFunctions(pScripting, pClientGame);
	RegisterVehicleFunctions(pScripting, pClientGame);
	RegisterUtilFunctions(pScripting, pClientGame);
	RegisterRenderingFunctions(pScripting, pClientGame);

#if !MAFIA_SCRIPTING_ALWAYS_INITIALISED
	auto pScriptingInterface = Strong<Interfaces::IScripting>::New(pScripting->GetInterface());
	tHackEventDataRegisterFunctions Data;
	Data.m_Size = sizeof(Data);
	Data.m_pScripting = pScriptingInterface;
	TriggerHackEvent(HACKEVENT_REGISTERFUNCTIONS, &Data);
#endif
}
