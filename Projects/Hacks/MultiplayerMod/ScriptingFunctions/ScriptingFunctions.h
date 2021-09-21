#pragma once

#include <Scripting/Scripting.h>

class CClientGame;

class CScriptingFunctions
{
public:
    static void RegisterGameDefines(Galactic3D::CDefineHandlers* pDefineHandlers);

	static void RegisterEntityFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
	static void RegisterGameFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
	static void RegisterHumanFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
	//static void RegisterPlayerFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
	static void RegisterVehicleFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
	static void RegisterUtilFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
	static void RegisterRenderingFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);

	static void RegisterDefines(Galactic3D::CDefineHandlers* pDefineHandlers);
	static void RegisterFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame);
};
