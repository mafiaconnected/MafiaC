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
class CClientDummy;
class CClientObject;
class CClientDoor;
class CClientBridge;

// Changable limits
#include <IncreasedLimits.h>

enum eMafiaElementType
{
	ELEMENT_ENTITY = Galactic3D::ELEMENT_TRANSFORMABLE | 4,
	ELEMENT_PED = ELEMENT_ENTITY | 8,
	ELEMENT_PLAYER = ELEMENT_PED | 16,
	ELEMENT_VEHICLE = ELEMENT_ENTITY | 32,
	ELEMENT_DUMMY = ELEMENT_ENTITY | 64,
	ELEMENT_OBJECT = ELEMENT_ENTITY | 128,
	ELEMENT_DOOR = ELEMENT_ENTITY | 256,
	ELEMENT_BRIDGE = ELEMENT_ENTITY | 512,
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
	Galactic3D::ReflectedClass* m_pClientDummyClass;
	Galactic3D::ReflectedClass* m_pClientObjectClass;
	Galactic3D::ReflectedClass* m_pClientDoorClass;
	Galactic3D::ReflectedClass* m_pClientBridgeClass;

public:
	//void RegisterFunctions(Galactic3D::CScripting* pScripting);
	virtual CNetObject* Create(int Type) override;
	virtual void Remove(CNetObject* pNetObject);
	virtual bool IsConnecting(void) override;
	virtual bool Connect(const GChar* pszHost, unsigned short usPort, const GChar* pszPassword) override;
	virtual bool Disconnect(void) override;
	virtual bool OnProcess(void);

	virtual CClientHuman* FindHuman(MafiaSDK::C_Human* pHuman);
	virtual CClientPlayer* FindPlayer(MafiaSDK::C_Player* pPlayer);
	virtual CClientVehicle* FindVehicle(MafiaSDK::C_Car* pVehicle);
	virtual CClientObject* FindObject(MafiaSDK::C_Actor* pObject);
	virtual CClientDoor* FindDoor(MafiaSDK::C_Door* pDoor);
	virtual CClientBridge* FindBridge(MafiaSDK::C_Bridge* pBridge);
	
	Galactic3D::Weak<CClientVehicle> m_rgpVehicles[MAX_VEHICLES];
	Galactic3D::Weak<CClientHuman> m_rgpPeds[MAX_PEDS];
	Galactic3D::Weak<CClientPlayer> m_rgpPlayers[MAX_PEDS];
	Galactic3D::Weak<CClientObject> m_rgpObjects[MAX_OBJECTS];
	Galactic3D::Weak<CClientDoor> m_rgpDoors[MAX_DOORS];
	Galactic3D::Weak<CClientBridge> m_rgpBridges[MAX_BRIDGES];
};

class CMafiaClientResourceMgr : public Galactic3D::CClientResourceMgr
{
public:
	CMafiaClientResourceMgr(Galactic3D::Context* pContext);

	virtual void RemoveThingsAssociatedWithResource(Galactic3D::CResource* pResource);
};
