#pragma once

#include "ClientEntity.h"
#include "ClientHuman.h"
#include "../ClientManager.h"
#include "../ClientGame.h"
#include "NetBlenderVehicle.h"

class CClientVehicle : public CClientEntity
{
private:
	MafiaSDK::C_Car* m_MafiaVehicle;

	CVector3D prevPos{ 0,0,0 }, prevRot{ 0,0,0 }, relPos{ 0,0,0 }, relRot{ 0,0,0 }, targetPos{ 0,0,0 }, targetRot{ 0,0,0 };

	CVector3D m_RotationFront;
	CVector3D m_RotationUp;
	CVector3D m_RotationRight;

	Galactic3D::Weak<CClientHuman> m_pOccupants[4];

	bool m_Horn = false;
	float m_EngineRPM = 0.0f;

public:
	CVector3D m_vecCachedPositionForTraffic{ 0, 0, 0 };

	bool m_bDontRemoveGameItem = false;

public:
	CClientVehicle(CMafiaClientManager* pClientManager);

	virtual Galactic3D::ReflectedClass* GetReflectedClass(void);

	virtual MafiaSDK::C_Car* GetGameVehicle();

	virtual void Process(void) override;
	virtual void Create(const GChar* model, const CVector3D& pos, const CVector3D& rot);
	virtual void Delete(void);
	virtual void Despawn(void);
	virtual void Remove(void);

	virtual bool SetModel(const GChar* modelName) override;
	virtual const GChar* GetModel() override;

	void UpdateGameMatrix(void);

	virtual bool SetPosition(const CVector3D& vecPos) override;
	virtual bool GetPosition(CVector3D& vecPos) override;

	virtual bool SetRotation(const CVector3D& vecPos) override;
	virtual bool GetRotation(CVector3D& vecPos) override;

	virtual bool SetHeading(float heading) override;
	virtual float GetHeading() override;

	virtual bool SetRotationMat(const CVector3D& rotationFront, const CVector3D& rotationUp, const CVector3D& rotationRight);
	virtual bool GetRotationMat(CVector3D& rotationFront, CVector3D& rotationUp, CVector3D& rotationRight);

	bool SetRotationQuat(CQuaternion& quatRot);
	bool GetRotationQuat(CQuaternion& quatRot);

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

	virtual CClientHuman* GetHumanInSeat(int8_t iSeat);
	virtual bool IsSeatOccupied(int8_t iSeat);
	virtual bool AssignSeat(CClientHuman* pHuman, int8_t iSeat);
	virtual bool FreeSeat(int8_t iSeat);

	virtual bool SetSiren(bool state);
	virtual bool GetSiren();

	virtual bool SetEngine(bool state, bool unknown1);
	virtual bool GetEngine();

	virtual bool SetLights(bool state);
	virtual bool GetLights();

	virtual bool Repair();
	virtual bool Explode();

	virtual void SetFromExistingEntity(MafiaSDK::C_Car* car);

	virtual void CreateNetBlender() override;

	void ForceAI(uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4);
};
