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
	MafiaSDK::C_Human* m_MafiaHuman = nullptr;

	CVector3D prevPos{ 0,0,0 }, prevRot{ 0,0,0 }, relPos{ 0,0,0 }, relRot{ 0,0,0 }, targetPos{ 0,0,0 }, targetRot{ 0,0,0 };

	InventoryItem items[8];

public:
	CClientHuman(CMafiaClientManager* pClientManager);

	bool m_isLocalPlayer = false;

	int32_t m_nVehicleNetworkIndex = INVALID_NETWORK_ID;
	int8_t m_nVehicleSeatIndex = 0;

	bool m_bEnteredVehicleEvent = false;
	bool m_bEnteringVehicleEvent = false;

	bool m_bExitedVehicleEvent = false;
	bool m_bExitingVehicleEvent = false;

	CVector3D m_vecCamera;
	CVector3D m_AimVector;

	float m_InCarRotation;

	CClientVehicle* m_pVehicleEvent = nullptr;
	
	Galactic3D::Weak<CClientVehicle> m_pCurrentVehicle;

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual MafiaSDK::C_Human* GetGameHuman();

	void UpdateGameMatrix();

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecRot) override;
	virtual bool GetRotation(CVector3D& vecRot) override;

	virtual bool SetHeading(float heading) override;
	virtual float GetHeading() override;

	virtual bool SetHealth(float fHealth);
	virtual float GetHealth();

	virtual bool SetVelocity(const CVector3D& vecVel);
	virtual bool GetVelocity(CVector3D& vecVel);

	virtual bool SetRotationVelocity(const CVector3D& vecRotVel);
	virtual bool GetRotationVelocity(CVector3D& vecRotVel);

	virtual void OnCreated();
	virtual void Process() override;

	virtual void Spawn(const CVector3D& pos, float angle, bool isLocal);
	virtual void Kill();
	virtual void InstantDeath();
	virtual void Despawn();

	virtual bool Create();
	virtual void Delete() override;

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	CClientVehicle* GetOccupiedVehicle();
	CClientVehicle* GetEnteringExitingVehicle();
	int8_t GetVehicleSeat();

	bool IsInVehicle();
	bool IsInVehicle(CClientVehicle* pClientVehicle);
	//bool IsInVehicleSeat(CClientVehicle* pClientVehicle, uint8_t iSeat);
	void EnterVehicle(CClientVehicle* pVehicle, uint8_t iSeat);
	void RemoveFromVehicle();
	void ExitVehicle();
	bool WarpIntoVehicle(CClientVehicle* pClientVehicle, uint8_t iSeat);

	virtual bool SetModel(const GChar* modelName) override;
	virtual const GChar* GetModel() override;

	void PlayAnim(const GChar* animName);
	void StopAnim();

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

	bool IsShooting();

	virtual void SetFromExistingEntity(MafiaSDK::C_Human* human);

	void SetBehavior(uint32_t iBehavior);

	virtual void CreateNetBlender() override;

	void ForceAI(uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4);
};
