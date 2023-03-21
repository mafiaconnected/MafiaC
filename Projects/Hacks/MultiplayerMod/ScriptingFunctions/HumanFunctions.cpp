#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"
#include <Utils/VectorTools.h>

#pragma region Functions

static bool FunctionHumanKill(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->DropWeapon();

	return true;
}

static bool FunctionHumanHolsterWeapon(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->HolsterWeapon();

	return true;
}

static bool FunctionHumanReloadWeapon(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->Reload();

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnNumber(pClientHuman->GetAnimationStateLocal());

	return true;
}

static bool FunctionHumanGetAnimationTimeLeft(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnNumber(pClientHuman->GetGameHuman()->GetInterface()->animTimeLeft);

	return true;
}

static bool FunctionHumanIsShooting(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnNumber(pClientHuman->IsShooting());

	return true;
}

static bool FunctionHumanGetHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

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
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnNumber(pClientHuman->GetHeading());

	return true;
}

static bool FunctionHumanSetHeading(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	float fHeading = 0;
	if (!pState->CheckNumber(0, fHeading))
		return false;

	pClientHuman->SetHeading(fHeading);

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

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	if (pClientHuman->SetRotation(rot))
		return true;

	return false;
}

static bool FunctionHumanShootAt(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;

	pClientHuman->GetGameHuman()->Do_Shoot(true, CVecTools::ConvertToMafiaVec(pos));

	return false;
}

static bool FunctionHumanAimAt(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;

	pClientHuman->GetGameHuman()->Do_Shoot(false, CVecTools::ConvertToMafiaVec(pos));

	return false;
}

static bool FunctionHumanGetSkin(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

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

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	if (pClientHuman->SetModel(mdl))
		return true;

	return false;
}

static bool FunctionHumanGetVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	CClientVehicle* pVehicle = pClientHuman->GetOccupiedVehicle();

	if (pVehicle) pState->ReturnObject((CBaseObject*)pClientHuman->GetOccupiedVehicle());
	else pState->ReturnNull();
	return true;
}

static bool FunctionHumanGetVehicleSeat(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnNumber(pClientHuman->GetVehicleSeat());
	return true;
}

static bool FunctionHumanGetAddress(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

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

	int8_t iSeat;
	if (!pState->CheckNumber(1, iSeat))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->WarpIntoVehicle(pClientVehicle, iSeat);
	return true;
}

static bool FunctionHumanRemoveFromVehicle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	CClientHuman* pClientHuman;
	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->RemoveFromVehicle();
	return true;
}

static bool FunctionHumanPlayAnimation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	const GChar* animName = pState->CheckString(0);
	if (!animName)
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->PlayAnim(animName);
	return false;
}

static bool FunctionHumanStopAnimation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	const GChar* animName = pState->CheckString(0);
	if (!animName)
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->StopAnim();
	return false;
}

static bool FunctionHumanGetCrouching(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnBoolean(pClientHuman->GetGameHuman()->GetInterface()->isDucking);
	return false;
}

static bool FunctionHumanGetReloading(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnBoolean(pClientHuman->GetGameHuman()->GetInterface()->isReloading);
	return false;
}

static bool FunctionHumanGetAiming(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnBoolean(pClientHuman->GetGameHuman()->GetInterface()->isAiming);
	return false;
}

static bool FunctionHumanGetPose(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pState->ReturnVector3D(CVecTools::ConvertFromMafiaVec(pClientHuman->GetGameHuman()->GetInterface()->pose));
	return false;
}

static bool FunctionHumanSetCrouching(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	bool bCrouched;
	if (!pState->CheckBoolean(0, bCrouched))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Do_Crouched(bCrouched);
	//pClientHuman->SetCrouched(bCrouched);
	return false;
}

static bool FunctionHumanMoveForward(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	bool bRun;
	if (!pState->CheckBoolean(0, bRun))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_Front(bRun);
	return false;
}

static bool FunctionHumanMoveBackward(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_Back();
	return false;
}

static bool FunctionHumanMoveRight(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	bool bRun;
	if (!pState->CheckBoolean(0, bRun))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_Right(bRun);
	return false;
}

static bool FunctionHumanMoveLeft(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	bool bRun;
	if (!pState->CheckBoolean(0, bRun))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_Left(bRun);
	return false;
}

static bool FunctionHumanMoveForwardLeft(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	bool bRun;
	if (!pState->CheckBoolean(0, bRun))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_FrontLeft(bRun);
	return false;
}

static bool FunctionHumanMoveBackwardLeft(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_BackLeft();
	return false;
}

static bool FunctionHumanMoveForwardRight(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	bool bRun;
	if (!pState->CheckBoolean(0, bRun))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_FrontRight(bRun);
	return false;
}

static bool FunctionHumanMoveBackwardRight(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Go_BackRight();
	
	return false;
}

static bool FunctionHumanIdle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->GetInterface()->animState = 1;
	pClientHuman->GetGameHuman()->GetInterface()->animStateLocal = 1;

	return false;
}

static bool FunctionHumanPunch(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	//pClientHuman->GetGameHuman()->Hit();

	return false;
}

static bool FunctionHumanJump(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Do_Jump();
	return false;
}

static bool FunctionHumanAim(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Do_Aimed();
	return false;
}

static bool FunctionHumanBreath(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Do_Breath();
	return false;
}

static bool FunctionHumanThrowGrenade(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	CVector3D vecPos;
	if (!pState->CheckVector3D(0, vecPos))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->GetGameHuman()->Do_ThrowGranade(CVecTools::ConvertToMafiaVec(vecPos));
	return false;
}

static bool FunctionHumanForceAI(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientHuman* pClientHuman;

	if (!pState->GetThis(pClientManager->m_pClientHumanClass, &pClientHuman))
		return false;

	uint32_t uiValue1;
	if (!pState->CheckNumber(0, uiValue1))
		return false;

	uint32_t uiValue2;
	if (!pState->CheckNumber(1, uiValue2))
		return false;

	uint32_t uiValue3;
	if (!pState->CheckNumber(2, uiValue3))
		return false;

	uint32_t uiValue4;
	if (!pState->CheckNumber(3, uiValue4))
		return false;

	if (pClientHuman->GetGameHuman() == nullptr)
		return pState->Error(_gstr("human not spawned"));

	pClientHuman->ForceAI(uiValue1, uiValue2, uiValue3, uiValue4);
	return true;
}

#pragma endregion

void CScriptingFunctions::RegisterHumanFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("kill"), _gstr("t"), FunctionHumanKill, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("respawn"), _gstr("tvf"), FunctionHumanRespawn, pClientManager);

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("heading"), ARGUMENT_FLOAT, FunctionHumanGetHeading, FunctionHumanSetHeading);

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("vehicle"), ARGUMENT_OBJECT, FunctionHumanGetVehicle);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("seat"), ARGUMENT_OBJECT, FunctionHumanGetVehicleSeat);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("skin"), ARGUMENT_STRING, FunctionHumanGetSkin, FunctionHumanSetSkin);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("health"), ARGUMENT_FLOAT, FunctionHumanGetHealth, FunctionHumanSetHealth);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("animationState"), ARGUMENT_INTEGER, FunctionHumanGetAnimationState);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("animationStateLocal"), ARGUMENT_INTEGER, FunctionHumanGetAnimationStateLocal); 
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("animationTimeLeft"), ARGUMENT_INTEGER, FunctionHumanGetAnimationTimeLeft);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("shooting"), ARGUMENT_BOOLEAN, FunctionHumanIsShooting);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("crouching"), ARGUMENT_BOOLEAN, FunctionHumanGetCrouching, FunctionHumanSetCrouching);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("reloading"), ARGUMENT_BOOLEAN, FunctionHumanGetReloading);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("aiming"), ARGUMENT_BOOLEAN, FunctionHumanGetAiming);
	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("pose"), ARGUMENT_VECTOR3D, FunctionHumanGetPose);

	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("clearWeapons"), _gstr("t"), FunctionHumanClearWeapons, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("giveWeapon"), _gstr("tiii"), FunctionHumanGiveWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("hasWeapon"), _gstr("ti"), FunctionHumanHasWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("takeWeapon"), _gstr("ti"), FunctionHumanTakeWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("dropWeapon"), _gstr("t"), FunctionHumanDropWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("holsterWeapon"), _gstr("t"), FunctionHumanHolsterWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("reloadWeapon"), _gstr("t"), FunctionHumanReloadWeapon, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("warpIntoVehicle"), _gstr("tvi"), FunctionHumanWarpIntoVehicle, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("removeFromVehicle"), _gstr("t"), FunctionHumanRemoveFromVehicle, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("addAnimation"), _gstr("ts"), FunctionHumanPlayAnimation, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("playAnimation"), _gstr("ts"), FunctionHumanPlayAnimation, pClientManager); // Alias
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("stopAnimation"), _gstr("ts"), FunctionHumanStopAnimation, pClientManager); // Alias
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("setBehavior"), _gstr("ti"), FunctionHumanSetBehavior, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("forceAI"), _gstr("tiiii"), FunctionHumanForceAI, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("shootAt"), _gstr("tv"), FunctionHumanShootAt, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("aimAt"), _gstr("tv"), FunctionHumanAimAt, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveForward"), _gstr("tb"), FunctionHumanMoveForward, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveBackward"), _gstr("t"), FunctionHumanMoveBackward, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveLeft"), _gstr("tb"), FunctionHumanMoveLeft, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveRight"), _gstr("tb"), FunctionHumanMoveRight, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveForwardLeft"), _gstr("tb"), FunctionHumanMoveForwardLeft, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveForwardRight"), _gstr("tb"), FunctionHumanMoveForwardRight, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveBackLeft"), _gstr("t"), FunctionHumanMoveBackwardLeft, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("moveBackRight"), _gstr("t"), FunctionHumanMoveBackwardRight, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("idle"), _gstr("tw"), FunctionHumanIdle, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("jump"), _gstr("t"), FunctionHumanJump, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("aim"), _gstr("t"), FunctionHumanAim, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("breath"), _gstr("t"), FunctionHumanBreath, pClientManager);
	pClientManager->m_pClientHumanClass->RegisterFunction(_gstr("throwGrenade"), _gstr("tv"), FunctionHumanThrowGrenade, pClientManager);

	pClientManager->m_pClientHumanClass->AddProperty(pClientManager, _gstr("address"), ARGUMENT_INTEGER, FunctionHumanGetAddress);
}
