#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientHuman.h"
#include "../Elements/ClientVehicle.h"

#pragma region Functions
static bool FunctionVehicleExplode(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle->Explode())
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleRepair(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle->Repair())
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleDespawn(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle->GetGameVehicle() == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pClientVehicle->Delete();

	return true;
}
#pragma endregion

#pragma region Properties
static bool FunctionVehicleGetPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D pos = { 0, 0, 0 };

	if (pClientVehicle->GetPosition(pos))
	{
		pState->ReturnVector3D(pos);

		return true;
	}

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleSetPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;

	if (pClientVehicle->SetPosition(pos))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetRotation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D rot = { 0, 0, 0 };

	if (pClientVehicle->GetRotation(rot))
	{
		pState->ReturnVector3D(rot);

		return true;
	}

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleSetRotation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D rot;
	if (!pState->CheckVector3D(0, rot))
		return false;

	if (pClientVehicle->SetRotation(rot))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetRotVelocity(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D vel = { 0, 0, 0 };

	if (pClientVehicle->GetRotationVelocity(vel))
	{
		pState->ReturnVector3D(vel);

		return true;
	}

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleSetRotVelocity(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D vel;
	if (!pState->CheckVector3D(0, vel))
		return false;

	if (pClientVehicle->SetRotationVelocity(vel))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetVelocity(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D vel = { 0, 0, 0 };

	if (pClientVehicle->GetVelocity(vel))
	{
		pState->ReturnVector3D(vel);

		return true;
	}

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleSetVelocity(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	CVector3D vel;
	if (!pState->CheckVector3D(0, vel))
		return false;

	if (pClientVehicle->SetVelocity(vel))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetSiren(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnBoolean(pClientVehicle->GetSiren());

	return true;
}

static bool FunctionVehicleSetSiren(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	bool state;
	if (!pState->CheckBoolean(0, state))
		return false;

	if (pClientVehicle->SetSiren(state))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetLocked(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnBoolean(pClientVehicle->GetLocked());

	return true;
}

static bool FunctionVehicleSetLocked(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	bool state;
	if (!pState->CheckBoolean(0, state))
		return false;

	if (pClientVehicle->SetLocked(state))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetLights(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnBoolean(pClientVehicle->GetLights());

	return true;
}

static bool FunctionVehicleSetLights(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	bool state;
	if (!pState->CheckBoolean(0, state))
		return false;

	if (pClientVehicle->SetLights(state))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetEngine(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnBoolean(pClientVehicle->GetEngine());

	return true;
}

static bool FunctionVehicleSetEngine(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	bool state;
	if (!pState->CheckBoolean(0, state))
		return false;

	if (pClientVehicle->SetEngine(state))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetRoof(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnBoolean(pClientVehicle->GetRoof());

	return true;
}

static bool FunctionVehicleSetRoof(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	bool roof;
	if (!pState->CheckBoolean(0, roof))
		return false;

	if (pClientVehicle->SetRoof(roof))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetFuel(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetFuel());

	return true;
}

static bool FunctionVehicleSetFuel(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float fuel;
	if (!pState->CheckNumber(0, fuel))
		return false;

	if (pClientVehicle->SetFuel(fuel))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleSetWheelAngle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float wheelAngle;
	if (!pState->CheckNumber(0, wheelAngle))
		return false;

	if (pClientVehicle->SetWheelAngle(wheelAngle))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetWheelAngle(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetWheelAngle());

	return true;
}

static bool FunctionVehicleGetSpeedLimit(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetSpeedLimit());

	return true;
}

static bool FunctionVehicleSetSpeedLimit(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float speedLimit;
	if (!pState->CheckNumber(0, speedLimit))
		return false;

	if (pClientVehicle->SetSpeedLimit(speedLimit))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetGear(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetGear());

	return true;
}

static bool FunctionVehicleSetGear(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float gear;
	if (!pState->CheckNumber(0, gear))
		return false;

	if (pClientVehicle->SetGear(gear))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetOdometer(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetOdometer());

	return true;
}

static bool FunctionVehicleSetOdometer(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float odometer;
	if (!pState->CheckNumber(0, odometer))
		return false;

	if (pClientVehicle->SetOdometer(odometer))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetSpeed(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetSpeed());

	return true;
}

static bool FunctionVehicleSetSpeed(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float speed;
	if (!pState->CheckNumber(0, speed))
		return false;

	if (pClientVehicle->SetSpeed(speed))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetEngineHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetEngineHealth());

	return true;
}

static bool FunctionVehicleSetEngineHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float engineHealth;
	if (!pState->CheckNumber(0, engineHealth))
		return false;

	if (pClientVehicle->SetEngineHealth(engineHealth))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetHealth());

	return true;
}

static bool FunctionVehicleSetHealth(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float health;
	if (!pState->CheckNumber(0, health))
		return false;

	if (pClientVehicle->SetHealth(health))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetEngineRPM(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber(pClientVehicle->GetEngineRPM());

	return true;
}

static bool FunctionVehicleSetEngineRPM(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	float engineRPM;
	if (!pState->CheckNumber(0, engineRPM))
		return false;

	if (pClientVehicle->SetEngineRPM(engineRPM))
		return true;

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleSetActState(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	uint8_t state;
	if (!pState->CheckNumber(0, state))
		return false;

	if (pClientVehicle->GetGameVehicle() != nullptr)
		pClientVehicle->GetGameVehicle()->SetActState(state);

	pState->Error(_gstr("vehicle not spawned"));
	return false;
}

static bool FunctionVehicleGetAddress(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientVehicle* pClientVehicle;

	if (!pState->GetThis(pClientManager->m_pClientVehicleClass, &pClientVehicle))
		return false;

	if (pClientVehicle == nullptr)
	{
		pState->Error(_gstr("vehicle not spawned"));
		return false;
	}

	pState->ReturnNumber((uint32_t)pClientVehicle->GetGameVehicle());
	return true;
}
#pragma endregion

void CScriptingFunctions::RegisterVehicleFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pClientManager->m_pClientVehicleClass->RegisterFunction(_gstr("explode"), _gstr("t"), FunctionVehicleExplode, pClientManager);
	pClientManager->m_pClientVehicleClass->RegisterFunction(_gstr("repair"), _gstr("t"), FunctionVehicleRepair, pClientManager);

	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("siren"), ARGUMENT_BOOLEAN, FunctionVehicleGetSiren, FunctionVehicleSetSiren);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("lights"), ARGUMENT_BOOLEAN, FunctionVehicleGetLights, FunctionVehicleSetLights);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("engine"), ARGUMENT_BOOLEAN, FunctionVehicleGetEngine, FunctionVehicleSetEngine);
	//pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("odometer"), ARGUMENT_BOOLEAN, FunctionVehicleGetOdometer, FunctionVehicleSetOdometer);
	//pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("roof"), ARGUMENT_BOOLEAN, FunctionVehicleGetRoof, FunctionVehicleSetRoof);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("speedLimit"), ARGUMENT_FLOAT, FunctionVehicleGetSpeedLimit, FunctionVehicleSetSpeedLimit);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("gear"), ARGUMENT_FLOAT, FunctionVehicleGetGear, FunctionVehicleSetGear);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("wheelAngle"), ARGUMENT_FLOAT, FunctionVehicleGetWheelAngle, FunctionVehicleSetWheelAngle);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("fuel"), ARGUMENT_FLOAT, FunctionVehicleGetFuel, FunctionVehicleSetFuel);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("speed"), ARGUMENT_FLOAT, FunctionVehicleGetSpeed, FunctionVehicleSetSpeed);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("turnVelocity"), ARGUMENT_VECTOR3D, FunctionVehicleGetRotVelocity, FunctionVehicleSetRotVelocity);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("velocity"), ARGUMENT_VECTOR3D, FunctionVehicleGetVelocity, FunctionVehicleSetVelocity);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("engineHealth"), ARGUMENT_FLOAT, FunctionVehicleGetEngineHealth, FunctionVehicleSetEngineHealth);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("health"), ARGUMENT_FLOAT, FunctionVehicleGetHealth, FunctionVehicleSetHealth);
	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("engineRPM"), ARGUMENT_FLOAT, FunctionVehicleGetEngineRPM, FunctionVehicleSetEngineRPM);

	pClientManager->m_pClientVehicleClass->RegisterFunction(_gstr("setActState"), _gstr("ti"), FunctionVehicleSetActState, pClientManager);
	

	pClientManager->m_pClientVehicleClass->AddProperty(pClientManager, _gstr("address"), ARGUMENT_INTEGER, FunctionVehicleGetAddress);
}