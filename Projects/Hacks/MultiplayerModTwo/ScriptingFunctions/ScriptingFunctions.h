#pragma once

#include <Scripting/Scripting.h>

class CClientGameII;

class CScriptingFunctionsII
{
public:
    static void RegisterGameDefines(Galactic3D::CDefineHandlers* pDefineHandlers);
	static void RegisterUtilDefines(Galactic3D::CDefineHandlers* pDefineHandlers);
	
	static void RegisterEntityFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
	static void RegisterGameFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
	static void RegisterHumanFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
	//static void RegisterPlayerFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
	static void RegisterVehicleFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
	static void RegisterUtilFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
	static void RegisterRenderingFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);

	static void RegisterDefines(Galactic3D::CDefineHandlers* pDefineHandlers);
	static void RegisterFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame);
};
