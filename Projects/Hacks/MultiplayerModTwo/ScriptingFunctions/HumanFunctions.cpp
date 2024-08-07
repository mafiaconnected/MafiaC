#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"

#pragma region Functions

static bool FunctionHumanKillII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->Kill();

	return true;
}

static bool FunctionHumanDespawnII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->Despawn();

	return true;
}

static bool FunctionHumanSpawnII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;

	float angle = 0;

	if (!pState->CheckNumber(1, angle))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->Spawn(pos, angle, pClientHuman == pClientManager->m_pLocalPlayer);

	return true;
}

static bool FunctionHumanClearWeaponsII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->ClearInventory();

	return true;
}

static bool FunctionHumanGiveWeaponII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	unsigned short usWeaponId = 0;
	if (!pState->CheckNumber(0, usWeaponId))
		return false;

	unsigned short ammo1 = 0;
	if (!pState->CheckNumber(1, ammo1))
		return false;

	unsigned short ammo2 = 0;
	if (!pState->CheckNumber(2, ammo2))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->GiveWeapon(usWeaponId, ammo1, ammo2);

	return true;
}

static bool FunctionHumanHasWeaponII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	unsigned short usWeaponId = 0;
	if (!pState->CheckNumber(0, usWeaponId))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pState->ReturnBoolean(pClientHuman->HasWeapon(usWeaponId));

	return true;
}

static bool FunctionHumanTakeWeaponII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	unsigned short usWeaponId = 0;
	if (!pState->CheckNumber(0, usWeaponId))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->TakeWeapon(usWeaponId);

	return true;
}

static bool FunctionHumanDropWeaponII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->DropWeapon();

	return true;
}

//static bool FunctionHumanWarpIntoVehicle(IScriptState* pState, int argc, void* pUser)
//{
//	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
//
//	CClientHumanII* pClientHuman;
//	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
//		return false;
//
//	CClientVehicle* pClientVehicle;
//	if (!pState->CheckClass(pClientManager->m_pClientVehicleClass, 0, false, &pClientVehicle))
//		return false;
//
//	uint8_t nSeat = 0;
//	if (!pState->CheckNumber(1, nSeat))
//		return false;
//
//	if (pClientHuman->WarpIntoVehicle(pVehicle, nSeat))
//		return true;
//
//	pState->Error(_gstr("human not spawned"));
//	return false;
//}
#pragma endregion

#pragma region Properties
static bool FunctionHumanGetHealthII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	float health = pClientHuman->GetHealth();

	pState->ReturnNumber(health);

	return true;
}

static bool FunctionHumanSetHealthII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	float health = 0;

	if (!pState->CheckNumber(0, health))
		return false;

	pClientHuman->SetHealth(health);

	return true;
}

static bool FunctionHumanGetHeadingII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	float heading = pClientHuman->GetHeading();

	pState->ReturnNumber(heading);

	return true;
}

static bool FunctionHumanSetHeadingII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	float heading = 0;

	if (!pState->CheckNumber(0, heading))
		return false;

	pClientHuman->SetHeading(heading);

	return true;
}

static bool FunctionHumanGetPositionII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D pos = { 0, 0, 0 };

	if (pClientHuman->GetPosition(pos))
	{
		pState->ReturnVector3D(pos);

		return true;
	}

	pState->Error(_gstr("human not spawned"));
	return false;
}

static bool FunctionHumanSetPositionII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;

	if (pClientHuman->SetPosition(pos))
		return true;

	pState->Error(_gstr("human not spawned"));
	return false;
}

static bool FunctionHumanGetRotationII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D rot = { 0, 0, 0 };

	if (pClientHuman->GetRotation(rot))
	{
		pState->ReturnVector3D(rot);

		return true;
	}

	pState->Error(_gstr("human not spawned"));
	return false;
}

static bool FunctionHumanSetRotationII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D rot;
	if (!pState->CheckVector3D(0, rot))
		return false;

	if (pClientHuman->SetRotation(rot))
		return true;

	pState->Error(_gstr("human not spawned"));
	return false;
}

static bool FunctionHumanGetSkinII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));

		return false;
	}

	uint32_t model = pClientHuman->GetModel();
	pState->ReturnNumber(model);

	return true;
}

static bool FunctionHumanSetSkinII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	uint32_t model;
	if (!pState->CheckNumber(0, model))
		return false;

	if (pClientHuman->SetModel(model))
		return true;

	pState->Error(_gstr("human not spawned"));
	return false;
}

static bool FunctionHumanGetVehicleII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));

		return false;
	}

	CClientVehicleII* pVehicle = pClientHuman->GetOccupiedVehicle();

	if (pVehicle) pState->ReturnObject((CBaseObject*)pClientHuman->GetOccupiedVehicle());
	else pState->ReturnNull();
	return true;
}

static bool FunctionHumanWarpIntoVehicleII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CClientVehicleII* pClientVehicle;
	if (!pState->CheckClass(pClientManager->m_pClientVehicleClass, 0, false, &pClientVehicle))
		return false;

	unsigned char ucSeat;
	if (!pState->CheckNumber(1, ucSeat))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->WarpIntoVehicle(pClientVehicle, ucSeat);
	return true;
}

static bool FunctionHumanRemoveFromVehicleII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
	CClientHumanII* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CClientVehicleII* pClientVehicle;
	if (!pState->CheckClass(pClientManager->m_pClientVehicleClass, 0, false, &pClientVehicle))
		return false;

	unsigned char ucSeat;
	if (!pState->CheckNumber(1, ucSeat))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->RemoveFromVehicle();
	return true;
}

static bool FunctionHumanPlayAnimII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientHumanII* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	const GChar* animName = pState->CheckString(0);
	if (!animName)
		return false;

	pClientHuman->PlayAnim((const char*)animName);

	pState->Error(_gstr("human not spawned"));
	return false;
}
#pragma endregion

void CScriptingFunctionsII::RegisterHumanFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("heading"), ARGUMENT_FLOAT, FunctionHumanGetHeadingII, FunctionHumanSetHeadingII);

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("kill"), _gstr("t"), FunctionHumanKillII, pClientManager);

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("vehicle"), ARGUMENT_OBJECT, FunctionHumanGetVehicleII);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("skin"), ARGUMENT_INTEGER, FunctionHumanGetSkinII, FunctionHumanSetSkinII);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("health"), ARGUMENT_FLOAT, FunctionHumanGetHealthII, FunctionHumanSetHealthII);

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("clearWeapons"), _gstr("t"), FunctionHumanClearWeaponsII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("giveWeapon"), _gstr("tiii"), FunctionHumanGiveWeaponII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("hasWeapon"), _gstr("ti"), FunctionHumanHasWeaponII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("takeWeapon"), _gstr("ti"), FunctionHumanTakeWeaponII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("dropWeapon"), _gstr("t"), FunctionHumanDropWeaponII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("warpIntoVehicle"), _gstr("tvi"), FunctionHumanWarpIntoVehicleII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("removeFromVehicle"), _gstr("tvi"), FunctionHumanRemoveFromVehicleII, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("addAnimation"), _gstr("ts"), FunctionHumanPlayAnimII, pClientManager);
}