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
		PADDING(C_Vehicle_Interface, _pad0, 0x68);
		bool roof;												// 104-105 - Needs more testing
		PADDING(C_Vehicle_Interface, _pad1, 0x14B);
		float engine_health;									// 436-440
		PADDING(C_Vehicle_Interface, _pad2, 0x34);
		float health;											// 492-496
		PADDING(C_Vehicle_Interface, _pad3, 0x28);
		DWORD first_mesh;										// 536-540
		DWORD last_mesh;										// 540-544
		PADDING(C_Vehicle_Interface, _pad4, 0x100);
		S_vector position;										// 800-812
		PADDING(C_Vehicle_Interface, _pad5, 0xF4);
		S_vector rot_speed;										// 1056-1068
		PADDING(C_Vehicle_Interface, _pad6, 0x4);
		bool horn;												// 1072-1073
		bool siren;												// 1073-1074
		bool sound_enabled;										// 1074-1075
		float hand_break;										// 1076-1080
		PADDING(C_Vehicle_Interface, _pad10, 0x6C);
		float speed_limit;										// 1188-1192
		PADDING(C_Vehicle_Interface, _pad11, 0x98);
		float accelerating;										// 1344-1348
		PADDING(C_Vehicle_Interface, _pad12, 0x4);
		float engine_rpm;										// 1352-1356
		PADDING(C_Vehicle_Interface, _pad13, 0x14);
		DWORD gear;												// 1376-1380
		PADDING(C_Vehicle_Interface, _pad14, 0x54);
		float break_val;										// 1464-1468
		PADDING(C_Vehicle_Interface, _pad15, 0x24);
		float clutch;											// 1504-1508
		PADDING(C_Vehicle_Interface, _pad16, 0x40);
		float wheel_angle;										// 1572-1576
		PADDING(C_Vehicle_Interface, _pad17, 0x604);
		bool engine_on;											// 3116-3117  
		float fuel;												// 3120-3124
		PADDING(C_Vehicle_Interface, _pad19, 0x8);
		S_vector rot_forward;									// 3132-3144
		S_vector rot_right;										// 3144-3156
		S_vector rot_up;										// 3156-3168
		PADDING(C_Vehicle_Interface, _pad20, 0x1330);
		S_vector speed;											// 8080-8092
		PADDING(C_Vehicle_Interface, _pad21, 0xC8);
		bool lights;											// 8292-8293
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

	/*
		Ported from reMafia's C_Vehicle.h (same author, MafiaOrbitCam/Vendors/reMafia) -
		a far more complete field-by-field layout (engine/gearbox/wheel/damage/sound state)
		than C_Vehicle_Interface above, which only names ~20 fields amid large PADDING gaps.

		Deliberately kept as its OWN separate, parallel type rather than merged into
		C_Vehicle_Interface: cross-checking a handful of offsets against C_Vehicle_Interface's
		already-relied-upon, production-tested numbers (used throughout ClientVehicle.cpp's
		network sync) turned up a small (~2 byte) drift partway through that couldn't be
		confidently resolved without a live disassembler/test rig - unlike the vtable-pointer
		offset issue found and fixed elsewhere in this pass, this one has no equally clean
		explanation, so merging it risked silently corrupting a struct nearly every field of
		which is already read/written for real vehicle networking. Field offsets below are
		exactly as reMafia declared them (relative to its own struct start, i.e. NOT
		cross-checked against C_Vehicle_Interface's numbering) - verify before relying on it
		for anything that must agree with the existing C_Vehicle_Interface layout.
	*/
	struct S_CarInit
	{
		struct Roughness
		{
			float sizeMax;
			float sizeMin;
			float heightMax;
			float heightMin;
			float intensity;
		};

		struct Part
		{
			float resistanceMelee;
			float resistanceGun;
			float health;
			float absoluteHealth;
		};

		struct DeformablePart
		{
			Part part;
			S_vector dir;
		};

		float mass;
		float engineEffectPower;
		float engineRedLinePower;
		float engineRedLineRpmMin;
		float engineRedLineRpmMax;
		float engineIdleRpm;
		float engineTorqueStart;
		float engineTorqueMax;
		float engineTorqueEnd;
		float engineEffectRpmMin;
		float engineEffectRpmMax;
		float engineStarterRpm;
		float engineStarterTime;
		float engineFlywheelRpm;
		float gearboxShiftTransmissionTime;
		float gearboxClutchTime;
		PADDING(S_CarInit, _pad0, 0x4);
		float engineStartRpmMin;
		float engineStartRpmMax;
		float numGears;
		PADDING(S_CarInit, _pad1, 0x84);
		float sideForce;
		float forwardForce;
		float oversteerFactorMin;
		float oversteerFactorMax;
		float understeerFactorMin;
		float understeerFactorMax;
		float steeringMaxAngle;
		float steeringMaxAngleInHighestSpeed;
		float steeringDifferentialAngle;
		float steeringAngleChange;
		float steeringAngleChangeDestroyed;
		float steeringAngleChangeBack;
		float steeringAngleChangeBackDestroyed;
		S_vector center;
		float gravity;
		float fuelTank;
		float fuelConsumption;
		PADDING(S_CarInit, _pad2, 0x4A8);
		float balance1;
		float balance2;
		float balance3;
		float brakeEfficiency;
		float brakeEfficiencyRestriction;
		float handbrakeEfficiency;
		float handbrakeEfficiencyRestriction;
		bool useCrashMultipliers;
		float crashForceMultiplier;
		float crashForceBackMultiplier;
		int looseAdhesionFactor;
		bool abs;
		bool esp;
		bool steeringIsInvisible;
		PADDING(S_CarInit, _pad3, 0x5);
		bool ignoreHeadCollision;
		int exhaustParticleID;
		float exhaustParticleOff;
		float exhaustParticleVelocityIdle;
		float exhaustParticleVelocityDriving;
		PADDING(S_CarInit, _pad4, 0x10);
		bool hasRoughness;
		Roughness roughness;
		Part enginePart;
		float engineControlFuelAfterCrash;
		float engineParticle1On;
		float engineParticle2On;
		float engineParticle2Off;
		int engineParticle1Id;
		int engineParticle2Id;
		Part gearboxPart;
		Part wheelsPart;
		float wheelsSemiDestroyedAngle;
		float wheelsDamageToSemiDestroy;
		S_vector bodyDeformDir;
		S_vector resistanceDir;
		DeformablePart bumpersPart;
		DeformablePart windowsPart;
		float windowsCrack;
		char windowsBrokenTexture[16];
		char windowsBrokenTextureAlpha[16];
		Part lights;
		PADDING(S_CarInit, _pad6, 0x4);
		S_vector lightsDir;
		DeformablePart licencePlatesPart;
		DeformablePart mirrorsPart;
		DeformablePart wingsPart;
		DeformablePart doorsPart;
		DeformablePart roofPart;
		DeformablePart unkPart;
		PADDING(S_CarInit, _pad7, 0x4A0);
		char soundEngineOn[16];
		char soundEngineOff[16];
		char soundEngineBad[16];
		char soundEngineNpc[16];
		PADDING(S_CarInit, _pad8, 0xF0);
		char soundEngineNeutral[16];
		char tyreDrivingSound[16];
		char soundHorn[16];
		char soundSiren[16];
		char soundHandbrake[16];
		char soundGearNext[16];
		char soundGearPrev[16];
		char soundDrift[16];
		char soundUnk[16];
		char soundCrashAbsorber[16];
		char soundShotInWheel[16];
		char soundGlassBreak[16];
		char soundCrashA1[16];
		char soundCrashK1[16];
		char soundCrashB1[16];
		char soundCrashC1[16];
		char soundCrashA2[16];
		char soundCrashK2[16];
		char soundCrashB2[16];
		char soundCrashC2[16];
		char soundDoorOpen[16];
		char soundDoorClose[16];
		float engineNPCSoundSpeedMin;
		float engineNPCSoundSpeedMax;
		float engineNPCSoundVolumeIdle;
		float engineNPCSoundVolumeDriving;
		PADDING(S_CarInit, _pad9, 0x1F0);
		float engineNeutralFade;
		float engineNeutralVolume;
		float tyreDrivingSoundSpeedMin;
		float tyreDrivingSoundSpeedMax;
		float tyreDrivingSoundCarSpeedMin;
		float tyreDrivingSoundCarSpeedMax;
		float tyreDrivingSoundVolume;
		PADDING(S_CarInit, _pad10, 0x14);
	};

	struct S_Wheel
	{
		PADDING(S_Wheel, _pad0, 0x4);
		I3D_Frame* frame;
		PADDING(S_Wheel, _pad1, 0x50);
		S_vector frontDir;
		S_vector unk2;
		PADDING(S_Wheel, _pad2, 0x9C);
		float unk1;
		float unk2f;
		float rot;
		float unk3;
		float unk4;
		unsigned long flags;
		PADDING(S_Wheel, _pad4, 0x60);
		float semiDestroyedAngle;
		float wheelOutOfAxis;
		PADDING(S_Wheel, _pad5, 0x4);
		float health;
		PADDING(S_Wheel, _pad6, 0x4);
		float resistanceMelee;
		float resistanceGun;
		float damageToSemiDestroy;
		float roughnessSizeMin;
		float roughnessSizeMax;
		float roughnessHeightMin;
		float roughnessHeightMax;
		float roughnessIntensity;
	};

	class C_Car; // defined later in Game/C_Car.hpp - only needed here as a pointer type

	class C_Vehicle_Extended
	{
	public:
		/*
			Bodies below ported from reMafia's C_Vehicle.cpp (same author,
			MafiaOrbitCam/Vendors/reMafia) - confirmed, working bit-flag logic against
			m_uLightFlags/lightFlags. EnableLeftIndicator/EnableRightIndicator/
			DisableTurnIndicatorFlag/SetLightFlags/EnableLight are all exactly as
			reMafia implements them.

			SetLightFlags only merges bits 20-27 (mask 0xFF00000) of `flags` into
			lightFlags; which of those bits correspond to headlight high/low beam vs.
			police/roof lights has NOT been confirmed anywhere (no repo scanned - MafiaC,
			MafiaC-Server, MafiaSDK, reMafia, rc1-oakwood - identifies them), so this is
			exposed to scripts as a raw, undocumented-semantics flags word rather than
			named properties; treat any specific bit meaning within that byte as
			unverified until confirmed live.
		*/
		void EnableRightIndicator(bool enable)
		{
			if (!(lightFlags & 8))
			{
				if (enable)
				{
					lightFlags |= 2;
					if (callbackWU)
						((void(*)(const void*))(callbackWU))(car);
				}
				else
				{
					lightFlags &= 0xFD;
				}
			}
		}

		void DisableTurnIndicatorFlag(bool enable)
		{
			if (enable)
				lightFlags |= 8;
			else
				lightFlags &= 0xF7;
		}

		void EnableLeftIndicator(bool enable)
		{
			if (!(lightFlags & 8))
			{
				if (enable)
				{
					lightFlags |= 1;
					if (callbackWU)
						((void(*)(const void*))(callbackWU))(car);
				}
				else
				{
					lightFlags &= 0xFE;
				}
			}
		}

		void SetLightFlags(unsigned long flags)
		{
			lightFlags ^= (flags ^ lightFlags) & 0xFF00000;
			if (callbackWU)
				((void(*)(const void*))(callbackWU))(car);
		}

		void EnableLight(bool enable)
		{
			if (enable)
				lightFlags |= 0x80;
			else
				lightFlags &= 0x7F;
			if (callbackWU)
				((void(*)(const void*))(callbackWU))(car);
		}

		// Not ported: reMafia's EnableLightsValue writes a byte at a fixed offset
		// (_pad28[360]) that falls outside its own declared padding array, landing in
		// guessed/unconfirmed territory - left undeclared rather than porting an
		// out-of-bounds write with no verified meaning.

		PADDING(C_Vehicle_Extended, _pad1, 0x64);
		unsigned short moveFramesCnt;
		unsigned short currentMoveFrame;
		S_vector movPosition;
		S_vector movForward;
		S_vector movUp;
		S_vector movRight;
		bool ignoreHeadCollision;
		PADDING(C_Vehicle_Extended, _pad2, 0x1F);
		int steeringVisibility;
		PADDING(C_Vehicle_Extended, _pad2_0, 0x4);
		int lockCount;
		float lightsResistanceMelee;
		float lightsResistanceGun;
		float lightsHealth;
		float lightsAbsoluteHealth;
		PADDING(C_Vehicle_Extended, _pad2_1, 0x48);
		unsigned long lightFlags;
		PADDING(C_Vehicle_Extended, _pad2_1_0, 0x1C);
		bool useCrashMultipliers;
		bool hasRoughness;
		PADDING(C_Vehicle_Extended, _pad2_2, 0x58);
		float steeringLinearity;
		float clutchLinearity;
		PADDING(C_Vehicle_Extended, _pad3, 0x4);
		unsigned long flags;
		PADDING(C_Vehicle_Extended, _pad3_0, 0x4);
		float engineControlFuelAfterCrash;
		PADDING(C_Vehicle_Extended, _pad4, 0x8);
		float engineHealth;
		float engineAbsoluteHealth;
		float engineResistanceMelee;
		float engineResistanceGun;
		float engineParticle1On;
		float engineParticle2On;
		float engineParticle2Off;
		int engineParticle1Id;
		int engineParticle2Id;
		PADDING(C_Vehicle_Extended, _pad5, 0x10);
		float health;
		float gearboxHealth;
		float gearboxAbsoluteHealth;
		float gearboxResistanceMelee;
		float gearboxResistanceGun;
		PADDING(C_Vehicle_Extended, _pad5_0, 0x8);
		void* doors;
		PADDING(C_Vehicle_Extended, _pad6, 0xC);
		void* firstMesh;
		void* lastMesh;
		PADDING(C_Vehicle_Extended, _pad7_0, 0x4);
		int looseAdhesionFactor;
		bool hasABS;
		bool hasESP;
		PADDING(C_Vehicle_Extended, _pad7_1, 0x9A);
		S_vector unk;
		PADDING(C_Vehicle_Extended, _pad7_2, 0x4);
		S_vector headPos;
		PADDING(C_Vehicle_Extended, _pad7, 0x4);
		void* callbackCC;
		void* callbackCC1;
		void* callbackACI;
		void* callbackAF;
		void* callbackWC;
		void* callbackPD;
		void* callbackPDW;
		void* callbackBG;
		void* callbackUnk;
		void* callbackWU;
		void* callbackCFB;
		void* callbackCFW;
		void* callbackDVP;
		void* callbackVR;
		void* callbackLP;
		S_vector position;
		PADDING(C_Vehicle_Extended, _pad8, 0x24);
		float unk_;
		float wheelRotationReleatedStuff;
		PADDING(C_Vehicle_Extended, _pad8_0, 0xC);
		S_vector unk1;
		S_vector unk2;
		PADDING(C_Vehicle_Extended, _pad8_1, 0x10);
		float crashForceMultiplier;
		float crashForceBackMultiplier;
		PADDING(C_Vehicle_Extended, _pad8_2, 0x10);
		float enginePower;
		float enginePower2;
		float engineRedLinePower;
		float handbrakeEfficiency;
		float handbrakeEfficiencyRestriction;
		float handbrakeCurrent;
		PADDING(C_Vehicle_Extended, _pad9, 0x8);
		float brakeEfficiency;
		float brakeEfficiencyRestriction;
		float brakeCurrent;
		float oversteerFactorMin;
		float oversteerFactorMax;
		float understeerFactorMin;
		float understeerFactorMax;
		S_vector unk3;
		S_vector unk4;
		S_vector unk5;
		S_vector unk6;
		S_vector gravityVelocity;
		float engineStarterTime;
		S_vector angularVelocity;
		PADDING(C_Vehicle_Extended, _pad11, 0x4);
		bool horn;
		bool siren;
		bool soundEnabled;
		PADDING(C_Vehicle_Extended, _pad12_f, 0x1);
		float handbrake;
		float balance1;
		float balance2;
		float balance3;
		float balance4;
		float balance5;
		float balance6;
		PADDING(C_Vehicle_Extended, _pad12_0, 0x10);
		int exhaustParticleID;
		PADDING(C_Vehicle_Extended, _pad12_1, 0x4);
		float exhaustParticleOff;
		float exhaustParticleIdle;
		float exhaustParticleDriving;
		float engineTorqueMax;
		float engineTorqueStart;
		PADDING(C_Vehicle_Extended, _pad12_2_0, 0x4);
		float engineTorqueEnd;
		float engineEffectRpmMin;
		float engineEffectRpmMax;
		PADDING(C_Vehicle_Extended, _pad12_2_1, 0x10);
		float gravity;
		float fuelConsumption;
		float speedLimit;
		bool dontInterpolateSteering;
		PADDING(C_Vehicle_Extended, _pad13, 0x1);
		bool dontInterpolateBrake;
		bool dontInterpolateClutch;
		PADDING(C_Vehicle_Extended, _pad14, 0x10);
		char* modelName;
		PADDING(C_Vehicle_Extended, _pad14_1, 0x4);
		unsigned long wheelCnt;
		float deltaUpdateTime;
		PADDING(C_Vehicle_Extended, _pad15, 0x64);
		float engineEffectPower;
		PADDING(C_Vehicle_Extended, _pad15_0, 0x4);
		float sideForce;
		float forwardForce;
		float accelerating;
		PADDING(C_Vehicle_Extended, _pad16, 0x4);
		float rotations;
		PADDING(C_Vehicle_Extended, _pad16_2, 0x4);
		float engineRpm;
		PADDING(C_Vehicle_Extended, _pad17, 0x4);
		float engineIdleRpmHalf;
		int lastGear;
		int gear;
		float engineRedLineRpmMin;
		int maxGear;
		int unk1i;
		float gearRatios[4];
		PADDING(C_Vehicle_Extended, _pad19, 0x1C);
		float speed;
		PADDING(C_Vehicle_Extended, _pad20, 0x18);
		float brake;
		float engineStarterRpm;
		PADDING(C_Vehicle_Extended, _pad21, 0x4);
		float engineStartRpmMin;
		float engineStartRpmMax;
		PADDING(C_Vehicle_Extended, _pad21_0, 0x4);
		float engineFlywheelRpm;
		float gearboxClutchTime;
		float gearboxShiftTransmissionTime;
		PADDING(C_Vehicle_Extended, _pad21_1, 0x4);
		float clutch;
		PADDING(C_Vehicle_Extended, _pad22, 0x4);
		int unkP0;
		float engineIdleRpm;
		PADDING(C_Vehicle_Extended, _pad22APol, 0x4);
		float mass;
		float mass2;
		S_vector center;
		float steeringMaxAngle;
		float steeringMaxAngleInHighestSpeed;
		float maxSteerAngle;
		float steeringAngleChange;
		float steeringAngleChangeDestroyed;
		float steeringAngleChangeBack;
		float steeringAngleChangeBackDestroyed;
		float steerAngle;
		PADDING(C_Vehicle_Extended, _pad24, 0x4);
		float steeringDifferentialAngle;
		PADDING(C_Vehicle_Extended, _pad24_0, 0x6C);
		I3D_Sound* hornSound2;
		I3D_Sound* sirenSound2;
		PADDING(C_Vehicle_Extended, _pad24_2, 0x288);
		I3D_Sound* engineOnSound;
		I3D_Sound* engineOffSound;
		I3D_Sound* engineBadSound;
		I3D_Sound* engineNpcSound;
		I3D_Sound* engineForwardSounds[10];
		I3D_Sound* engineReverseSounds[5];
		I3D_Sound* engineIdleSounds[2];
		I3D_Sound* hornSound;
		I3D_Sound* sirenSound;
		I3D_Sound* handbrakeSound;
		I3D_Sound* gearNextSound;
		I3D_Sound* gearPrevSound;
		I3D_Sound* driftSound;
		I3D_Sound* unkSound;
		I3D_Sound* crashAbsorberSound;
		I3D_Sound* shotInWheelSound;
		I3D_Sound* crashA1Sound;
		I3D_Sound* crashK1Sound;
		I3D_Sound* crashB1Sound;
		I3D_Sound* crashC1Sound;
		I3D_Sound* crashA2Sound;
		I3D_Sound* crashK2Sound;
		I3D_Sound* crashB2Sound;
		I3D_Sound* crashC2Sound;
		I3D_Sound* doorOpenSound;
		I3D_Sound* doorCloseSound;
		I3D_Sound* wheelPunctureSound;
		float engineNPCSoundSpeedMin;
		float engineNPCSoundSpeedMax;
		float engineNPCSoundVolumeIdle;
		float engineNPCSoundVolumeDriving;
		PADDING(C_Vehicle_Extended, _pad25, 0x1F0);
		float engineNeutralFade;
		PADDING(C_Vehicle_Extended, _pad25_0, 0x4);
		float engineNeutralVolume;
		float tyreDrivingSoundSpeedMin;
		float tyreDrivingSoundSpeedMax;
		float tyreDrivingSoundCarSpeedMin;
		PADDING(C_Vehicle_Extended, _pad25_0_0, 0x4);
		float tyreDrivingSoundCarSpeedMax;
		float tyreDrivingSoundVolume;
		PADDING(C_Vehicle_Extended, _pad25_0_1, 0x2C);
		float timePerMoveFrame;
		PADDING(C_Vehicle_Extended, _pad25_0_2, 0x4);
		bool isEngineRunning;
		PADDING(C_Vehicle_Extended, _pad25_1, 0x3);
		bool isEngineOn;
		float fuel;
		PADDING(C_Vehicle_Extended, _pad25_3, 0x2);
		S_Wheel** wheels;
		S_vector forwardDir;
		S_vector rightDir;
		S_vector upDir;
		PADDING(C_Vehicle_Extended, _pad27, 0x14);
		C_Car* car;
		PADDING(C_Vehicle_Extended, _pad27_0, 0x28);
		void* carDoors;
		PADDING(C_Vehicle_Extended, _pad27_1, 0x44);
		S_matrix unkMatrix0;
		S_matrix unkMatrix1;
		S_matrix unkMatrix2;
		S_matrix unkMatrix3;
		S_matrix unkMatrix4;
		I3D_Frame* bodyFrame;
		PADDING(C_Vehicle_Extended, _pad27_2, 0x40);
		S_CarInit carInit;
		S_vector velocity;
		float engineRedLineRpmMax;
		float fuelTank;
		PADDING(C_Vehicle_Extended, _pad28, 0xE8);
		unsigned long destructionMode;
		PADDING(C_Vehicle_Extended, _pad29, 0xD);
		bool isEngineBroken;
		PADDING(C_Vehicle_Extended, _pad30, 0x10E);
	};
};

#endif
