/*
    Copyright 2017 D�vid Svitana

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _CVEHICLE_H_
#define _CVEHICLE_H_

namespace MafiaSDK
{
    struct C_Vehicle_Interface {
		uint8_t _pad1[100];
		uint16_t m_iMoveFramesCnt;
		uint16_t m_iCurrentMoveFrame;
		S_vector m_vMovPosition;
		S_vector m_vMovForward;
		S_vector m_vMovUp;
		S_vector m_vMovRight;
		bool m_bIgnoreHeadCollision;
		uint8_t _pad2[31];
		int m_iSteeringVisibility;
		uint8_t _pad2_0[4];
		int m_iLockCount;
		float m_fLightsResistanceMelee;
		float m_fLightsResistanceGun;
		float m_fLightsHealth;
		float m_fLightsAbsoluteHealth;
		char _pad2_1[72];
		uint32_t m_uLightFlags;
		char _pad2_1_0[28];
		bool m_bUseCrashMultipliers;
		bool m_bHasRoughness;
		char _pad2_2[88];
		float m_fSteeringLinearity;
		float m_fClutchLinearity;
		uint8_t _pad3[4];
		uint32_t m_uFlags;
		uint8_t _pad3_0[4];
		float m_fEngineControlFuelAfterCrash;
		uint8_t _pad4[8];
		float m_fEngineHealth;
		float m_fEngineAbsoluteHealth;
		float m_fEngineResistanceMelee;
		float m_fEngineResistanceGun;
		float m_fEngineParticle1On;
		float m_fEngineParticle2On;
		float m_fEngineParticle2Off;
		int m_iEngineParticle1Id;
		int m_iEngineParticle2Id;
		uint8_t _pad5[16];
		float m_fHealth;
		float m_fGearboxHealth;
		float m_fGearboxAbsoluteHealth;
		float m_fGearboxResistanceMelee;
		float m_fGearboxResistanceGun;
		uint8_t _pad5_0[8];
		void* m_pDoors;
		uint8_t _pad6[12];
		void* m_pFirstMesh;
		void* m_pLastMesh;
		uint8_t _pad7_0[4];
		int m_iLooseAdhesionFactor;
		bool m_bHasABS;
		bool m_bHasESP;
		uint8_t _pad7_1[154];
		S_vector m_vUnk;
		uint8_t _pad7_2[4];
		S_vector m_vHeadPos;
		uint8_t _pad7[4];
		void* m_pCallbackCC;
		void* m_pCallbackCC1;
		void* m_pCallbackACI;
		void* m_pCallbackAF;
		void* m_pCallbackWC;
		void* m_pCallbackPD;
		void* m_pCallbackPDW;
		void* m_pCallbackBG;
		void* m_pCallbackUnk;
		void* m_pCallbackWU;
		void* m_pCallbackCFB;
		void* m_pCallbackCFW;
		void* m_pCallbackDVP;
		void* m_pCallbackVR;
		void* m_pCallbackLP;
		S_vector m_vPosition;
		uint8_t _pad8[36];
		float m_fUnk;
		float m_fWheelRotationReleatedStuff;
		uint8_t _pad8_0[12];
		S_vector m_vUnk1;
		S_vector m_vUnk2;
		uint8_t _pad8_1[16];
		float m_fCrashForceMultiplier;
		float m_fCrashForceBackMultiplier;
		uint8_t _pad8_2[16];
		float m_fEnginePower;
		float m_fEnginePower2;
		float m_fEngineRedLinePower;
		float m_fHandbrakeEfficiency;
		float m_fHandbrakeEfficiencyRestriction;
		float m_fHandbrakeCurrent;
		uint8_t _pad9[8];
		float m_fBrakeEfficiency;
		float m_fBrakeEfficiencyRestriction;
		float m_fBrakeCurrent;
		float m_fOversteerFactorMin;
		float m_fOversteerFactorMax;
		float m_fUndersteerFactorMin;
		float m_fUndersteerFactorMax;
		S_vector m_vUnk3;
		S_vector m_vUnk4;
		S_vector m_vUnk5;
		S_vector m_vUnk6;
		S_vector m_vGravityVelocity;
		float m_fEngineStarterTime;
		S_vector m_vAngularVelocity;
		uint8_t _pad11[4];
		bool m_bHorn;
		bool m_bSiren;
		bool m_bSoundEnabled;
		uint8_t _pad12_f;
		float m_fHandbrake;
		float m_fBalance1;
		float m_fBalance2;
		float m_fBalance3;
		float m_fBalance4;
		float m_fBalance5;
		float m_fBalance6;
		uint8_t _pad12_0[16];
		int m_iExhaustParticleID;
		uint8_t _pad12_1[4];
		float m_fExhaustParticleOff;
		float m_fExhaustParticleIdle;
		float m_fExhaustParticleDriving;
		float m_fEngineTorqueMax;
		float m_fEngineTorqueStart;
		uint8_t _pad12_2_0[4];
		float m_fEngineTorqueEnd;
		float m_fEngineEffectRpmMin;
		float m_fEngineEffectRpmMax;
		uint8_t _pad12_2_1[16];
		float m_fGravity;
		float m_fFuelConsumption;
		float m_fSpeedLimit;
		bool m_bDontInterpolateSteering;
		uint8_t _pad13;
		bool m_bDontInterpolateBrake;
		bool m_bDontInterpolateClutch;
		uint8_t _pad14[16];
		char* m_szModelName;
		uint8_t _pad14_1[4];
		uint32_t m_iWheelCnt;
		float m_fDeltaUpdateTime;
		uint8_t _pad15[100];
		float m_fEngineEffectPower;
		uint8_t _pad15_0[4];
		float m_fSideForce;
		float m_fForwardForce;
		float m_fAccelerating;
		uint8_t _pad16[4];
		float m_fRotations;
		uint8_t _pad16_2[4];
		float m_fEngineRpm;
		uint8_t _pad17[4];
		float m_fEngineIdleRpmHalf;
		int32_t m_iLastGear;
		int32_t m_iGear;
		float m_fEngineRedLineRpmMin;
		int32_t m_iMaxGear;
		int32_t m_iUnk1;
		float m_GearRatios[4];
		uint8_t _pad19[28];
		float m_fSpeed;
		uint8_t _pad20[24];
		float m_fBrake;
		float m_fEngineStarterRpm;
		uint8_t _pad21[4];
		float m_fEngineStartRpmMin;
		float m_fEngineStartRpmMax;
		uint8_t _pad21_0[4];
		float m_fEngineFlywheelRpm;
		float m_fGearboxClutchTime;
		float m_fGearboxShiftTransmissionTime;
		uint8_t _pad21_1[4];
		float m_fClutch;
		uint8_t _pad22[4];
		int m_Punk0;
		float m_fEngineIdleRpm;
		char _pad22APol[4];
		float m_fMass;
		float m_fMass2;
		S_vector m_vCenter;
		float m_fSteeringMaxAngle;
		float m_fSteeringMaxAngleInHighestSpeed;
		float m_fMaxSteerAngle;
		float m_fSteeringAngleChange;
		float m_fSteeringAngleChangeDestroyed;
		float m_fSteeringAngleChangeBack;
		float m_fSteeringAngleChangeBackDestroyed;
		float m_fSteerAngle;
		uint8_t _pad24[4];
		float m_fSteeringDifferentialAngle;
		uint8_t _pad24_0[108];
		I3D_sound* m_pHornSound2;
		I3D_sound* m_pSirenSound2;
		uint8_t _pad24_2[648];
		I3D_sound* m_pEngineOnSound;
		I3D_sound* m_pEngineOffSound;
		I3D_sound* m_pEngineBadSound;
		I3D_sound* m_pEngineNpcSound;
		I3D_sound* m_pEngineForwardSounds[10];
		I3D_sound* m_pEngineReverseSounds[5];
		I3D_sound* m_pEngineIdleSounds[2];
		I3D_sound* m_pHornSound;
		I3D_sound* m_pSirenSound;
		I3D_sound* m_pHandbrakeSound;
		I3D_sound* m_pGearNextSound;
		I3D_sound* m_pGearPrevSound;
		I3D_sound* m_pDriftSound;
		I3D_sound* m_pUnkSound;
		I3D_sound* m_pCrashAbsorberSound;
		I3D_sound* m_pShotInWheelSound;
		I3D_sound* m_pCrashA1Sound;
		I3D_sound* m_pCrashK1Sound;
		I3D_sound* m_pCrashB1Sound;
		I3D_sound* m_pCrashC1Sound;
		I3D_sound* m_pCrashA2Sound;
		I3D_sound* m_pCrashK2Sound;
		I3D_sound* m_pCrashB2Sound;
		I3D_sound* m_pCrashC2Sound;
		I3D_sound* m_pDoorOpenSound;
		I3D_sound* m_pDoorCloseSound;
		I3D_sound* m_pWheelPunctureSound;
		float m_fEngineNPCSoundSpeedMin;
		float m_fEngineNPCSoundSpeedMax;
		float m_fEngineNPCSoundVolumeIdle;
		float m_fEngineNPCSoundVolumeDriving;
		char _pad25[496];
		float m_fEngineNeutralFade;
		char _pad25_0[4];
		float m_fEngineNeutralVolume;
		float m_fTyreDrivingSoundSpeedMin;
		float m_fTyreDrivingSoundSpeedMax;
		float m_fTyreDrivingSoundCarSpeedMin;
		char _pad25_0_0[4];
		float m_fTyreDrivingSoundCarSpeedMax;
		float m_fTyreDrivingSoundVolume;
		char _pad25_0_1[44];
		float m_fTimePerMoveFrame;
		uint8_t _pad25_0_2[4];
		bool m_bIsEngineRunning;
		uint8_t _pad25_1[3];
		bool m_bIsEngineOn;
		float m_fFuel;
		uint8_t _pad25_3[2];
		S_wheel** m_pWheels;
		S_vector m_vForwardDir;
		S_vector m_vRightDir;
		S_vector m_vUpDir;
		uint8_t _pad27[20];
		C_car* m_pCar;
		uint8_t _pad27_0[40];
		void* m_pCarDoors;
		uint8_t _pad27_1[68];
		S_matrix m_mUnk0;
		S_matrix m_mUnk1;
		S_matrix m_mUnk2;
		S_matrix m_mUnk3;
		S_matrix m_mUnk4;
		I3D_frame* m_pBodyFrame;
		uint8_t _pad27_2[64];
		S_CARINIT m_sCarInit;
		S_vector m_vVelocity;
		float m_fEngineRedLineRpmMax;
		float m_fFuelTank;
		uint8_t _pad28[232];
		uint32_t m_uDestructionMode;
		uint8_t _pad29[13];
		bool m_bIsEngineBroken;
		uint8_t _pad30[270];
    };

    //const size_t Offset = offsetof(C_Vehicle_Interface, lights);
	//const size_t Size = sizeof(C_Vehicle_Interface::lights);

    namespace C_Vehicle_Enum
    {
        enum FunctionsAddresses
        {
			OpenWindow = 0x004D7A80,
			DoLights = 0x004D7CD0
        };
    };

    class C_Vehicle
    {
    public:
        C_Vehicle_Interface* GetInterface()
        {
            return reinterpret_cast<C_Vehicle_Interface*>(this);
        }

		void OpenWindow(int windowID, BOOL arg2)
		{
			unsigned long functionAddress = C_Vehicle_Enum::FunctionsAddresses::OpenWindow;
			__asm
			{
				mov ecx, this
				push windowID
				push arg2
				call functionAddress
			}
		}

		void DoLights(float arg1)
		{
			unsigned long functionAddress = C_Vehicle_Enum::FunctionsAddresses::DoLights;
			__asm
			{
				mov ecx, this
				push arg1
				call functionAddress
			}
		}
    };
};

#endif
