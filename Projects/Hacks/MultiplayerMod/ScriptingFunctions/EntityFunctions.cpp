#include "stdafx.h"
#include "ScriptingFunctions.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "../Elements/ClientEntity.h"

static bool FunctionEntityGetPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;
	
	CClientEntity* pClientEntity;

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

static bool FunctionEntitySetPosition(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientEntity* pClientEntity;

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

static bool FunctionEntityGetRotation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientEntity* pClientEntity;

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

static bool FunctionEntitySetRotation(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientEntity* pClientEntity;

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

static bool FunctionEntityGetModel(IScriptState* pState, int argc, void* pUser)
{
	CMafiaClientManager* pClientManager = (CMafiaClientManager*)pUser;

	CClientEntity* pClientEntity;

	if (!pState->GetThis(pClientManager->m_pClientEntityClass, &pClientEntity))
		return false;

	const GChar* model = pClientEntity->GetModel();

	pState->ReturnString(model);

	return true;
}

void CScriptingFunctions::RegisterEntityFunctions(Galactic3D::CScripting* pScripting, CClientGame* pClientGame)
{
	auto pClientManager = pClientGame->m_pClientManager;

	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("position"), ARGUMENT_VECTOR3D, FunctionEntityGetPosition, FunctionEntitySetPosition);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("rotation"), ARGUMENT_VECTOR3D, FunctionEntityGetRotation, FunctionEntitySetRotation);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("model"), ARGUMENT_STRING, FunctionEntityGetModel);
	pClientManager->m_pClientEntityClass->AddProperty(pClientManager, _gstr("modelIndex"), ARGUMENT_STRING, FunctionEntityGetModel);
}