#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"

#pragma region Functions

static bool FunctionHumanKill(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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


static bool FunctionHumanDespawn(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanSpawn(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanRespawn(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

	InventoryItem *items = pClientHuman->GetInventory();

	for (int i = 0; i < 8; i++)
	{
		auto item = items[i];

		if (item.weapId != 0)
			pClientHuman->GetGameHuman()->G_Inventory_AddItem({ item.weapId, item.ammo1, item.ammo2 });
	}

	return true;
}

static bool FunctionHumanClearWeapons(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanGiveWeapon(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanHasWeapon(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanTakeWeapon(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanDropWeapon(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
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

static bool FunctionHumanSetBehavior(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	unsigned int uiBehavior = 0;
	if (!pState->CheckNumber(0, uiBehavior))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pClientHuman->SetBehavior(uiBehavior);
	return true;
}

//static bool FunctionHumanWarpIntoVehicle(IScriptState* pState, int argc, void* pUser)
//{
//	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
//
//	CClientHuman* pClientHuman;
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
static bool FunctionHumanGetAnimationState(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientHuman->GetAnimationState());

	return true;
}

static bool FunctionHumanGetAnimationStateLocal(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientHuman->GetAnimationStateLocal());

	return true;
}

static bool FunctionHumanGetHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanSetHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanGetHeading(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanSetHeading(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanGetPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanSetPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanGetRotation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanSetRotation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

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

static bool FunctionHumanGetSkin(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));

		return false;
	}

	const GChar* model = pClientHuman->GetModel();

	pState->ReturnString(model);

	return true;
}

static bool FunctionHumanSetSkin(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	const GChar* mdl = pState->CheckString(0);
	if (!mdl)
		return false;

	if (pClientHuman->SetModel(mdl))
		return true;

	pState->Error(_gstr("human not spawned"));
	return false;
}

static bool FunctionHumanGetVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));

		return false;
	}

	CClientVehicle* pVehicle = pClientHuman->GetOccupiedVehicle();

	if (pVehicle) pState->ReturnObject((CBaseObject*)pClientHuman->GetOccupiedVehicle());
	else pState->ReturnNull();
	return true;
}

static bool FunctionHumanGetAddress(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
	{
		pState->Error(_gstr("human not spawned"));
		return false;
	}

	pState->ReturnNumber((uint32_t)pClientHuman->GetGameHuman()->GetInterface());

	return true;
}

static bool FunctionHumanWarpIntoVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CClientVehicle* pClientVehicle;
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

static bool FunctionHumanRemoveFromVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CClientVehicle* pClientVehicle;
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
#pragma endregion

void CScriptingFunctions::RegisterHumanFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("kill"), _gstr("t"), FunctionHumanKill, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("respawn"), _gstr("tvf"), FunctionHumanRespawn, pClientManager);

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("vehicle"), ARGUMENT_OBJECT, FunctionHumanGetVehicle);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("skin"), ARGUMENT_STRING, FunctionHumanGetSkin, FunctionHumanSetSkin);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("health"), ARGUMENT_FLOAT, FunctionHumanGetHealth, FunctionHumanSetHealth);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("animationState"), ARGUMENT_INTEGER, FunctionHumanGetAnimationState);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("animationStateLocal"), ARGUMENT_INTEGER, FunctionHumanGetAnimationStateLocal);

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("clearWeapons"), _gstr("t"), FunctionHumanClearWeapons, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("giveWeapon"), _gstr("tiii"), FunctionHumanGiveWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("hasWeapon"), _gstr("ti"), FunctionHumanHasWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("takeWeapon"), _gstr("ti"), FunctionHumanTakeWeapon, pClientManager);
	//pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("dropWeapon"), _gstr("t"), FunctionHumanDropWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("warpIntoVehicle"), _gstr("tvi"), FunctionHumanWarpIntoVehicle, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("removeFromVehicle"), _gstr("tvi"), FunctionHumanRemoveFromVehicle, pClientManager);
	//pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("playAnimation"), _gstr("ts"), FunctionHumanPlayAnim, pClientManager);

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("setBehavior"), _gstr("ti"), FunctionHumanSetBehavior, pClientManager);

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("address"), ARGUMENT_INTEGER, FunctionHumanGetAddress);
}