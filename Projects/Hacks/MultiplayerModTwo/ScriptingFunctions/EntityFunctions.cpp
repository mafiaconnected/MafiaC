#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientEntity.h"

static bool FunctionEntityGetPositionII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;
	
	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	CVector3D pos = { 0, 0, 0 };

	if (pClientEntity->GetPosition(pos))
	{
		pState->ReturnVector3D(pos);

		return true;
	}

	pState->Error(_gstr("entity not spawned"));
	return false;
}

static bool FunctionEntitySetPositionII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	CVector3D pos;
	if (!pState->CheckVector3D(0, pos))
		return false;
		
	if(pClientEntity->SetPosition(pos))
		return true;

	pState->Error(_gstr("entity not spawned"));
	return false;
}

static bool FunctionEntityGetRotationII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	CVector3D rot = { 0, 0, 0 };

	if (pClientEntity->GetRotation(rot))
	{
		pState->ReturnVector3D(rot);

		return true;
	}

	pState->Error(_gstr("entity not spawned"));
	return false;
}

static bool FunctionEntitySetRotationII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	CVector3D rot;
	if (!pState->CheckVector3D(0, rot))
		return false;

	if (pClientEntity->SetRotation(rot))
		return true;

	pState->Error(_gstr("entity not spawned"));
	return false;
}

static bool FunctionEntityGetHeadingII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	pState->ReturnNumber(pClientEntity->GetHeading());
	return false;
}

static bool FunctionEntitySetHeadingII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	float heading;
	if (!pState->CheckNumber(0, heading))
		return false;

	if (pClientEntity->SetHeading(heading))
		return true;

	pState->Error(_gstr("entity not spawned"));
	return false;
}

static bool FunctionEntityGetModelII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	uint32_t model = pClientEntity->GetModel();

	pState->ReturnNumber(model);

	return true;
}

static bool FunctionEntitySetModelII(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManagerII* pClientManager = (CMafiaClientManagerII*)pUser;

	CClientEntityII* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	uint32_t model;
	if (!pState->CheckNumber(0, model))
		return false;

	pClientEntity->SetModel(model);

	pState->ReturnNumber(model);

	return true;
}

void CScriptingFunctionsII::RegisterEntityFunctions(Galactic3D::CScripting* pScripting, CClientGameII* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("position"), ARGUMENT_VECTOR3D, FunctionEntityGetPositionII, FunctionEntitySetPositionII);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("rotation"), ARGUMENT_VECTOR3D, FunctionEntityGetRotationII, FunctionEntitySetRotationII);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("heading"), ARGUMENT_VECTOR3D, FunctionEntityGetHeadingII, FunctionEntitySetHeadingII);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("model"), ARGUMENT_INTEGER, FunctionEntityGetModelII, FunctionEntitySetModelII);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("modelIndex"), ARGUMENT_STRING, FunctionEntityGetModelII, FunctionEntitySetModelII);
}