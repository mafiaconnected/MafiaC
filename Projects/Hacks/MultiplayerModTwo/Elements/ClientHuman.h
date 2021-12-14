#pragma once

#include "ClientEntity.h"
#include "ClientVehicle.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderHuman.h"

class InventoryItem
{
public:
	unsigned short weapId = 0;
	unsigned short ammo1 = 0;
	unsigned short ammo2 = 0;
};

class CClientHumanII : public CClientEntityII
{
private:
	M2::C_Human2* m_MafiaHuman;

	CVector3D prevPos{ 0,0,0 }, prevRot{ 0,0,0 }, relPos{ 0,0,0 }, relRot{ 0,0,0 }, targetPos{ 0,0,0 }, targetRot{ 0,0,0 };

	InventoryItem items[8];

public:
	CClientHumanII(CMafiaClientManagerII* pClientManager);

	bool m_isLocalPlayer = false;

	int32_t m_nVehicleNetworkIndex;
	uint32_t m_nVehicleSeatIndex;

	bool m_bEnteredVehicleEvent = true;
	bool m_bEnteringVehicleEvent = false;

	bool m_bExitedVehicleEvent = false;
	bool m_bExitingVehicleEvent = false;

	CVector3D m_vecCamera;

	CClientVehicleII* m_pVehicleEvent = nullptr;
	
	Galactic3D::Weak<CClientVehicleII> m_pCurrentVehicle;

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual M2::C_Human2* GetGameHuman();

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecRot) override;
	virtual bool GetRotation(CVector3D& vecRot) override;

	virtual bool SetHeading(float heading);
	virtual float GetHeading();

	virtual bool SetHealth(float fHealth);
	virtual float GetHealth();

	virtual bool SetVelocity(const CVector3D& vecVel);
	virtual bool GetVelocity(CVector3D& vecVel);

	virtual bool SetRotationVelocity(const CVector3D& vecRotVel);
	virtual bool GetRotationVelocity(CVector3D& vecRotVel);

	virtual void OnCreated(void);
	virtual void Process(void) override;

	virtual void Spawn(const CVector3D& pos, float angle, bool isLocal);
	virtual void Kill(void);
	virtual void Despawn(void);

	virtual bool Create(void);
	virtual void Delete(void) override;

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	CClientVehicleII* GetOccupiedVehicle(void);
	CClientVehicleII* GetEnteringExitingVehicle(void);
	int8_t GetVehicleSeat(void);

	bool IsInVehicle(void);
	bool IsInVehicle(CClientVehicleII* pClientVehicle);
	bool IsInVehicle(CClientVehicleII* pClientVehicle, uint8_t ucSeat);
	void EnterVehicle(CClientVehicleII* pVehicle, uint8_t ucSeat);
	void RemoveFromVehicle(void);
	void ExitVehicle(void);
	bool WarpIntoVehicle(CClientVehicleII* pClientVehicle, uint8_t byteSeat);

	virtual bool SetModel(uint32_t modelName) override;
	virtual uint32_t GetModel() override;

	void PlayAnim(const char* animName);

	void Shoot(bool state, const CVector3D& dstPos);
	void Jump();
	void ThrowGrenade(const CVector3D& dstPos);
	void Reload();
	void HolsterWeapon();
	void Crouch(bool state);

	virtual InventoryItem GetInventoryItem(int index) { if (index >= 0 && index < 8) return items[index]; else return { 0 }; }
	virtual InventoryItem* GetInventory() { return items; }
	virtual void ClearInventory();
	void GiveWeapon(unsigned short ucWeapon, unsigned short ucAmmo1, unsigned short ucAmmo2);
	bool HasWeapon(unsigned short ucWeapon);
	void SetActiveWeapon(unsigned short usWeapon);
	int GetFirstEmptyWeaponIndex();
	int GetIndexOfWeapon(unsigned short ucWeapon);
	void TakeWeapon(unsigned short ucWeapon);
	void DropWeapon();

	virtual int GetAnimationState();
	virtual int GetAnimationStateLocal();

	virtual void SetFromExistingEntity(M2::C_Human2* human);

	void SetBehavior(uint32_t iBehavior);

	virtual void CreateNetBlender() override;
};
