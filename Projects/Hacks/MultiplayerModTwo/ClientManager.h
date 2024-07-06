#pragma once

#include <Scripting/Scripting.h>
#include <Scripting/GalacticFunctions.h>
#include <Multiplayer/Multiplayer.h>
#include <Multiplayer/ClientManager.h>

class CMafiaClientManagerII;

class CClientEntityII;
class CClientHumanII;
class CClientPlayerII;
class CClientVehicleII;

// Changable limits
#include <IncreasedLimits.h>

enum eMafiaElementType
{
	ELEMENT_ENTITY = Galactic3D::ELEMENT_TRANSFORMABLE | 4,
	ELEMENT_PED = ELEMENT_ENTITY | 8,
	ELEMENT_PLAYER = ELEMENT_PED | 16,
	ELEMENT_VEHICLE = ELEMENT_ENTITY | 32,
};

class CMafiaClientManagerII : public Galactic3D::CClientManager
{
public:
	CMafiaClientManagerII(Galactic3D::Context* pContext, Galactic3D::CClientResourceMgr* pResourceMgr);

	Galactic3D::ReflectedClass* m_pSurfaceClass;
	Galactic3D::ReflectedClass* m_pTextureClass;

	Galactic3D::ReflectedClass* m_pClientEntityClass;
	Galactic3D::ReflectedClass* m_pClientHumanClass;
	Galactic3D::ReflectedClass* m_pClientPlayerClass;
	Galactic3D::ReflectedClass* m_pClientVehicleClass;

public:
	//void RegisterFunctions(Galactic3D::CScripting* pScripting);
	virtual CNetObject* Create(int Type) override;
	virtual bool IsConnecting(void) override;
	virtual bool Connect(const GChar* pszHost, unsigned short usPort, const GChar* pszPassword) override;
	virtual bool Disconnect(void) override;
	virtual bool OnProcess(void);

	virtual CClientHumanII* FindHuman(M2::C_Human2* pHuman);
	virtual CClientPlayerII* FindPlayer(M2::C_Player2* pPlayer);
	virtual CClientVehicleII* FindVehicle(M2::C_Car* pVehicle);
	
	Galactic3D::Weak<CClientVehicleII> m_rgpVehicles[MAX_VEHICLES];
	Galactic3D::Weak<CClientHumanII> m_rgpPeds[MAX_PEDS];
	Galactic3D::Weak<CClientPlayerII> m_rgpPlayers[MAX_PEDS];
};

class CMafiaClientResourceMgrII : public Galactic3D::CClientResourceMgr
{
public:
	CMafiaClientResourceMgrII(Galactic3D::Context* pContext);

	virtual void RemoveThingsAssociatedWithResource(Galactic3D::CResource* pResource);
};
