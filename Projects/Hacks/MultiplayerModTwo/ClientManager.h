#pragma once

#include <Scripting/Scripting.h>
#include <Scripting/GalacticFunctions.h>
#include <Multiplayer/Multiplayer.h>
#include <Multiplayer/ClientManager.h>

class CMafiaClientManager;

class CClientEntity;
class CClientHuman;
class CClientPlayer;
class CClientVehicle;

// Changable limits
#include <IncreasedLimits.h>

enum eMafiaElementType
{
	ELEMENT_ENTITY = Galactic3D::ELEMENT_TRANSFORMABLE | 4,
	ELEMENT_PED = ELEMENT_ENTITY | 8,
	ELEMENT_PLAYER = ELEMENT_PED | 16,
	ELEMENT_VEHICLE = ELEMENT_ENTITY | 32,
};

class CMafiaClientManager : public Galactic3D::CClientManager
{
public:
	CMafiaClientManager(Galactic3D::Context* pContext, Galactic3D::CClientResourceMgr* pResourceMgr);

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

	virtual CClientHuman* FindHuman(M2::C_Human2* pHuman);
	virtual CClientPlayer* FindPlayer(M2::C_Player2* pPlayer);
	virtual CClientVehicle* FindVehicle(M2::C_Car* pVehicle);
	
	Galactic3D::Weak<CClientVehicle> m_rgpVehicles[MAX_VEHICLES];
	Galactic3D::Weak<CClientHuman> m_rgpPeds[MAX_PEDS];
	Galactic3D::Weak<CClientPlayer> m_rgpPlayers[MAX_PEDS];
};

class CMafiaClientResourceMgr : public Galactic3D::CClientResourceMgr
{
public:
	CMafiaClientResourceMgr(Galactic3D::Context* pContext);

	virtual void RemoveThingsAssociatedWithResource(Galactic3D::CResource* pResource);
};
