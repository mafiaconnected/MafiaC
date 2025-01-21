#pragma once

#include "ClientEntity.h"
#include "ClientHuman.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderVehicle.h"

class CClientVehicleII : public CClientEntityII
{
private:
	M2::C_Car* m_MafiaVehicle = nullptr;

	CVector3D prevPos{ 0,0,0 }, prevRot{ 0,0,0 }, relPos{ 0,0,0 }, relRot{ 0,0,0 }, targetPos{ 0,0,0 }, targetRot{ 0,0,0 };

	CVector3D m_RotationFront;
	CVector3D m_RotationUp;
	CVector3D m_RotationRight;

	Galactic3D::Weak<CClientHumanII> m_pOccupants[4];

	bool m_Horn = false;
	float m_EngineRPM = 0.0;

	bool m_Siren = false;

public:
	CClientVehicleII(CMafiaClientManagerII* pClientManager);

	virtual Galactic3D::ReflectedClass* GetReflectedClass() override;

	virtual M2::C_Car* GetGameVehicle();

	virtual void Process() override;
	virtual void Create(uint32_t model, const CVector3D& pos, const CVector3D& rot);
	virtual void Delete();
	virtual void Despawn();
	virtual void Remove();

	virtual bool SetModel(uint32_t model) override;
	virtual uint32_t GetModel() override;

	void UpdateGameMatrix();

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecPos) override;
	virtual bool GetRotation(CVector3D& vecPos) override;

	virtual bool SetHeading(float heading) override;
	virtual float GetHeading() override;

	virtual bool ReadCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool ReadSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool WriteCreatePacket(Galactic3D::Stream* pStream) override;
	virtual bool WriteSyncPacket(Galactic3D::Stream* pStream) override;

	virtual bool SetVelocity(const CVector3D& vecVel);
	virtual bool GetVelocity(CVector3D& vecVel);

	virtual bool SetRotationVelocity(const CVector3D& vecRotVel);
	virtual bool GetRotationVelocity(CVector3D& vecRotVel);

	virtual bool SetSpeed(float speed);
	virtual float GetSpeed();

	virtual bool SetFuel(float fuel);
	virtual float GetFuel();

	virtual bool SetGear(uint32_t gear);
	virtual uint32_t GetGear();

	virtual bool SetSpeedLimit(float speedLimit);
	virtual float GetSpeedLimit();

	virtual bool SetWheelAngle(float wheelAngle);
	virtual float GetWheelAngle();

	virtual bool SetEngineRPM(float engineRPM);
	virtual float GetEngineRPM();

	virtual bool SetEngineHealth(float engineHealth);
	virtual float GetEngineHealth();

	virtual bool SetHealth(float health);
	virtual float GetHealth();

	virtual bool SetOdometer(float odometer);
	virtual float GetOdometer();

	virtual bool SetRoof(bool state);
	virtual bool GetRoof();

	virtual bool SetLocked(bool state);
	virtual bool GetLocked();

	virtual CClientHumanII* GetHumanInSeat(unsigned char ucSeat);
	virtual bool IsSeatOccupied(unsigned char ucSeat);
	virtual bool AssignSeat(CClientHumanII* pHuman, unsigned char ucSeat);
	virtual bool FreeSeat(unsigned char ucSeat);

	virtual bool SetSiren(bool state);
	virtual bool GetSiren();

	virtual bool SetEngine(bool state);
	virtual bool GetEngine();

	virtual bool SetLights(bool state);
	virtual bool GetLights();

	virtual bool Repair();
	virtual bool Explode();

	virtual void SetFromExistingEntity(M2::C_Car* car);

	virtual void CreateNetBlender() override;
};
