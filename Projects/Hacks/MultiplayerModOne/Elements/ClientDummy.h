#pragma once

#include "ClientEntity.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderHuman.h"

class CClientDummy : public CClientEntity
{
public:
	CClientDummy(CMafiaClientManager* pClientManager);

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;
};
