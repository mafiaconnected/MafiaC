#pragma once

#include "ClientEntity.h"
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

class CClientHuman : public CClientEntity
{
private:
	MafiaSDK::C_Human* m_MafiaHuman;

	CVector3D prevPos{ 0,0,0 }, prevRot{ 0,0,0 }, relPos{ 0,0,0 }, relRot{ 0,0,0 }, targetPos{ 0,0,0 }, targetRot{ 0,0,0 };

	InventoryItem items[8];

public:
	CClientHuman(CMafiaClientManager* pClientManager);

	bool m_isLocalPlayer = false;

	int32_t m_nVehicleNetworkIndex;
	uint32_t m_nVehicleSeatIndex;

	bool m_bEnteredVehicleEvent = true;
	bool m_bEnteringVehicleEvent = false;

	bool m_bExitedVehicleEvent = false;
	bool m_bExitingVehicleEvent = false;

	CClientVehicle* m_pVehicleEvent = nullptr;
	
	Galactic3D::Weak<CClientVehicle> m_pCurrentVehicle;

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual MafiaSDK::C_Human* GetGameHuman();

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

	CClientVehicle* GetOccupiedVehicle(void);
	CClientVehicle* GetEnteringExitingVehicle(void);

	bool IsInVehicle(void);
	bool IsInVehicle(CClientVehicle* pClientVehicle);
	bool IsInVehicle(CClientVehicle* pClientVehicle, uint8_t ucSeat);
	void EnterVehicle(CClientVehicle* pVehicle, uint8_t ucSeat);
	void RemoveFromVehicle(void);
	void ExitVehicle(void);
	bool WarpIntoVehicle(CClientVehicle* pClientVehicle, uint8_t byteSeat);

	virtual bool SetModel(const GChar* modelName) override;
	virtual const GChar* GetModel() override;

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
	int GetFirstEmptyWeaponIndex();
	int GetIndexOfWeapon(unsigned short ucWeapon);
	void TakeWeapon(unsigned short ucWeapon);
	void DropWeapon();

	virtual int GetAnimationState();
	virtual int GetAnimationStateLocal();

	virtual void SetFromExistingEntity(MafiaSDK::C_Human* human);

	void CClientHuman::SetBehavior(uint32_t iBehavior);
};
