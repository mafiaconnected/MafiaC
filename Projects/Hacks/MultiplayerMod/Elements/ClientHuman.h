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

	bool m_isLocalPlayer;

	int32_t m_nVehicleNetworkIndex;
	int8_t m_nVehicleSeatIndex;

	bool m_bEnteredVehicleEvent;
	bool m_bEnteringVehicleEvent;

	bool m_bExitedVehicleEvent;
	bool m_bExitingVehicleEvent;

	CVector3D m_vecCamera;
	CVector3D m_AimVector;

	float m_InCarRotation;

	CClientVehicle* m_pVehicleEvent;
	
	Galactic3D::Weak<CClientVehicle> m_pCurrentVehicle;

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual MafiaSDK::C_Human* GetGameHuman();

	void UpdateGameMatrix(void);

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

	virtual void OnCreated(void);
	virtual void Process(void) override;

	virtual void Spawn(const CVector3D& pos, float angle, bool isLocal);
	virtual void Kill(void);
	virtual void InstantDeath(void);
	virtual void Despawn(void);

	virtual bool Create(void);
	virtual void Delete(void) override;

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	CClientVehicle* GetOccupiedVehicle(void);
	CClientVehicle* GetEnteringExitingVehicle(void);
	int8_t GetVehicleSeat(void);

	bool IsInVehicle(void);
	bool IsInVehicle(CClientVehicle* pClientVehicle);
	//bool IsInVehicleSeat(CClientVehicle* pClientVehicle, uint8_t iSeat);
	void EnterVehicle(CClientVehicle* pVehicle, uint8_t iSeat);
	void RemoveFromVehicle(void);
	void ExitVehicle(void);
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
