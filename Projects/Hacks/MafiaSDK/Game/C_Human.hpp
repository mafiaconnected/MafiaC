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

#ifndef _CHUMAN_H_
#define _CHUMAN_H_

namespace MafiaSDK
{
	// All 16 float slots addressed by C_Human_Enum::Property/C_Human::SetProperty|GetProperty,
	// laid out in order. Ported from reMafia's Actors/C_human.h (same author,
	// MafiaOrbitCam/Vendors/reMafia). NOTE: reMafia's own field order here starts with
	// Strength at *relative offset 0*, but SetProperty/GetProperty compute
	// `this + 0x640 + property * 4` with `Strength = 1` (i.e. relative offset 4, which this
	// layout says is actually Health) - the two disagree by one slot. Left exactly as it was
	// (still whatever real, presumably-tested behavior existed before this file was touched)
	// since fixing the formula/enum would change what every existing setProperty(...) script
	// call actually writes; flagging it here rather than silently changing it.
	struct C_Human_Properties
	{
		float strength;
		float health;
		float healthHandL;
		float healthHandR;
		float healthLegL;
		float healthLegR;
		float reactions;
		float speed;
		float aggressivity;
		float intelligence;
		float shooting;
		float sight;
		float hearing;
		float driving;
		float mass;
		float morale;
	};

	// Ported from reMafia's Actors/C_human.h.
	enum CharacterVoice
	{
		CV_Paulie     = 0,
		CV_Sam        = 1,
		CV_Salieri    = 2,
		CV_Tommy      = 3,
		CV_Frank      = 4,
		CV_Luigi      = 5,
		CV_Ralph      = 6,
		CV_Vincenzo   = 7,
		CV_Man        = 8,
		CV_Salvatore  = 9,
		CV_Cop0       = 10,
		CV_Com1       = 11,
		CV_PortGuard0 = 12,
		CV_PortGuard1 = 13,
		CV_PortGuard2 = 14,
		CV_PortGuard3 = 15,
		CV_PortGuard4 = 16,
		CV_Enemy0     = 17,
		CV_Enemy1     = 18,
		CV_Enemy2     = 19,
		CV_Enemy3     = 20,
		CV_Enemy4     = 21,
		CV_Hoolig     = 22,
		CV_Sergio     = 23,
		CV_Enemy5     = 24,
		CV_Joe        = 25,
		CV_PedMan0    = 26,
		CV_PedMan1    = 27,
		CV_PedMan2    = 28,
		CV_PedWoman0  = 29,
		CV_PedWoman1  = 30,
		CV_PedWoman2  = 31,
		CV_NoVoice    = 32,
		CV_Lucas      = 33,
	};

	/*
		Cross-checked against reMafia's C_human (Actors/C_human.h) - its first two fields
		(m_iAnimID/m_iAnimState) are modeled here as plain ints, but this SDK had already
		identified specific *byte* meanings within that same region (animStateLocal/
		isInAnimWithCarLocal/animState/isInAnimWithCar), which is more precise, so that part
		was kept as-is. Everything from playersCar (152) through inventory (1152, an exact
		independent match with the already-relied-on GetInventory() this+0x480 offset) is a
		solid cross-check. Past m_vCollisions (~1668) reMafia's own vc6_vector<T> size had to
		be estimated (allocator+3 pointers), so the tail fields (from voice onward) may drift
		a handful of bytes - kept as best-effort, not to the same confidence as the rest.
	*/
    struct C_Human_Interface
    {
        C_Entity_Interface entity;								// 0-112 (grew by 4 bytes when its own padding got filled in - see C_Entity.hpp; this struct's own absolute-offset comments below are unaffected since they were already computed against the correct 112-byte size)
        byte animStateLocal;									// 112-113
        byte isInAnimWithCarLocal;								// 113-114
        PADDING(C_Human_Interface, _pad1, 0x2);
        byte animState;											// 116-117
        byte isInAnimWithCar;									// 117-118
        PADDING(C_Human_Interface, _pad1a, 0x1E);
        unsigned long flags;									// 148-152
        C_Car* playersCar;										// 152-156
        C_Car* carLeavingOrEntering;							// 156-160
        C_Car* currentCar;										// 160-164
        PADDING(C_Human_Interface, _pad2a, 0x4);
        C_Car* currentCar2;										// 168-172
        int seatID;												// 172-176
        PADDING(C_Human_Interface, _pad2b, 0xC);
        void* animationMachine;								// 188-192
        PADDING(C_Human_Interface, _pad2c, 0x10C);
        bool isDead;											// 460-461
        PADDING(C_Human_Interface, _pad3a, 0x17);
        bool isDucking;											// 484-485
        bool isAiming;											// 485-486
        PADDING(C_Human_Interface, _pad4, 0x6);
        bool isShooting;										// 492-493
        PADDING(C_Human_Interface, _pad4b, 0x0F);
        bool isReloading;										// 508-509
        PADDING(C_Human_Interface, _pad5a, 0x3);
        S_vector shootTarget;									// 512-524
        PADDING(C_Human_Interface, _pad5b, 0x24);
        bool doChangeWeaponModel;								// 560-561
        PADDING(C_Human_Interface, _pad5c, 0xF);
        int deathAnimID;										// 576-580
        PADDING(C_Human_Interface, _pad5d, 0x98);
        void* shotSkeleton;										// 732-736
        PADDING(C_Human_Interface, _pad5e, 0x38);
        I3D_Frame * neckFrame;									// 792-796
        PADDING(C_Human_Interface, _pad6a, 0xF0);
        int canWork;											// 1036-1040
        PADDING(C_Human_Interface, _pad6b, 0x50);
        int unk2;												// 1120-1124
        PADDING(C_Human_Interface, _pad6c, 0x18);
        void* fakeShadow;										// 1148-1152
        G_Inventory inventory;									// 1152-1380 (grew by 100 bytes when its own layout got filled in - see Utils/Helpers.hpp; fields below are unaffected since this padding was shrunk to compensate)
        I3D_Frame* weaponR;										// 1380-1384
        I3D_Frame* weaponL;										// 1384-1388
        I3D_Frame* gunR;										// 1388-1392
        I3D_Frame* gunL;										// 1392-1396
        I3D_Frame* targetR;										// 1396-1400
        I3D_Frame* targetL;									// 1400-1404
        I3D_Frame* baseMesh;									// 1404-1408
        PADDING(C_Human_Interface, _pad7a, 0x8);
        I3D_Frame* targetE;										// 1416-1420
        I3D_Frame* targetN;									// 1420-1424
        PADDING(C_Human_Interface, _pad7b, 0x54);
        int unk3;												// 1508-1512
        PADDING(C_Human_Interface, _pad7c, 0xC);
        float inCarRotation;									// 1524-1528
        PADDING(C_Human_Interface, _pad8, 0x8);
        C_Human_Properties initProperties;						// 1536-1600
        PADDING(C_Human_Interface, _pad8a, 0x4);				// properties.strength (unnamed, see health note above)
        float health;											// 1604-1608 (= properties.health, offset 4 into the block above)
        PADDING(C_Human_Interface, _pad8b, 0x38);				// rest of properties (healthHandL..morale)
        CharacterVoice voice;									// 1664-1668
        vc6_vector<tDynamicCollObject*> collisions;			// ~1668-1684 (approximate, see note above)
        bool hasCollisions;										// ~1684-1685
        PADDING(C_Human_Interface, _pad9, 0x3E7);				// approximate - see note above
		S_vector pose;											// 2684-2696
		PADDING(C_Human_Interface, _pad10, 0x4C);
		int32_t animTimeLeft;									// 2772-2776
    };

	const size_t Offset = offsetof(C_Human_Interface, entity);
	const size_t Size = sizeof(C_Human_Interface::entity);

    namespace C_Human_Enum
    {
		enum FunctionsAddresses
		{
			Hit = 0x5762A0,
			Intern_UseCar = 0x57E020,
			Intern_FromCar = 0x57E500,
			Intern_ForceDeath = 0x5878D0,
			RepairWeaponPosition = 0x57E980,
			ChangeWeaponModel = 0x57EC20,
			Do_ChangeWeapon = 0x57F550,
			Do_WeaponDrop = 0x585D90,
			Use_Actor = 0x582180,
			Do_Shoot = 0x583590,
			Do_ThrowGranade = 0x583F40,
			Do_Reload = 0x585B40,
			Do_Holster = 0x585C60,
			Do_Jump = 0x586190,
			Do_PlayAnim = 0x585E80,
			Do_ThrowCocotFromCar = 0x587D70,
			EineMeineKleineAutoInLuft = 0x58A5A0,
			NewScream = 0x58B470,
			Intern_ChangeModel = 0x587190,
			Do_StopAnim = 0x57D2E0,
			Do_Breath = 0x57D310,
			Do_Aimed = 0x57F830,
			Do_Crouched = 0x57F8A0,
			G_Inventory_Insert = 0x6092B0,
			G_Inventory_Remove = 0x6095E0,
			G_Inventory_GetWeaponInRightHand = 0x607B80,
			G_Inventory_GetWeaponInLeftHand = 0x607BB0,
			G_Inventory_GetAmmo = 0x609D20,
			InventoryTruncate = 0x005878D0,
			Go_Front = 0x0057D360,
			Go_Back = 0x0057D430,
			Go_Left = 0x0057D4D0,
			Go_Right = 0x0057D5A0,
			Go_FrontLeft = 0x0057D670,
			Go_FrontRight = 0x0057D860,
			Go_BackLeft = 0x0057DA50,
			Go_BackRight = 0x0057DC10,
			PoseSetPoseAimed = 0x00579EA0,
			PoseSetPoseNormal = 0x00579630,
			EraseDynColls = 0x00575ED0,
			Death = 0x00570570,
			RecompileDeathPos = 0x0058B8D0,
			Do_DeadBodyDrop = 0x0058B7E0,

			// Patch targets (see C_Human_Patches / MultiplayerModOne's Hooks.cpp)
			RemoveDroppedClipPatch = 0x0058D4C6,
			RemoveDroppedClipPatchSkip = 0x0058D553,
			DisableWeaponDropsPatch = 0x0057FAA0,
			DisableWeaponDropsPatchSkip = 0x00580196,
			Do_WeaponDropPatchSkip = 0x00585DCB,
			ExitingVehiclePassengerFix = 0x00595040,
        };

        //Thanks for DjBozkosz Documentation
        enum BehaviorStates : byte
        {
            ReactsAll = 0, //Reacts on attacks, shots. Ducks only, moves away if char was hit multiple times � Duck.
            ReactsAllUnknown = 1, //Reacts on attacks, shots. Ducks only, moves away if char was hit multiple times � ???
            DoesntReactGuard = 2, //Does not react to anything. Guards the player � No reaction.
            DoesntReactOnWeapon = 4, //Does not react on anything � No reaction on weapon.
            ReactsPlayerOnHit = 8, //Reacts on player only, Attacks player � React when hit.
            ReactsPlayerMovesAway = 16, //Reacts on player, Moves away � Reaction on Player.
            ReactsOnAttacksAwayOrAttack = 32, //Reacts on attacks to other characters. He either moves away or attacks attacker � React on Attack.
            ReactsOnAttacksAttack = 64, //Reacts on attacks to other characters. Attacks attacker � React on Attack 2.
            ProgrammedReacts = 128 //Used for Little Joe in mission 9 programmed reactions, can add a little variety to non gangster characters � Afra
        };

        /*
            POSSIBLE OFF-BY-ONE (found while cross-referencing reMafia's C_human_properties,
            see C_Human_Properties above): SetProperty/GetProperty below compute
            `this + 0x640 + property * 4`, and 0x640 (1600) is confirmed correct as the base
            of the stats block (see C_Human_Interface::initProperties). But with Strength = 1,
            that formula lands on relative offset 4 - which reMafia's field order says is
            Health, not Strength - so every property here may currently read/write one slot
            off from its name (Strength writes what's really Health, etc). This enum is
            exposed to scripts (setProperty/PED_PROPERTY_* in MultiplayerModOne), so changing
            it would change what already-written scripts actually do - left exactly as found,
            not "fixed", pending a deliberate decision to change it.
        */
        enum Property : unsigned int
        {
            Strength = 1,
            Health,
            HealthHandL,
            HealthHandR,
            HealthLegL,
            HealthLegR,
            Reactions,
            Speed,
            Aggressivity,
            Inteligence,
            Shooting,
            Sight,
            Hearing,
            Driving,
            Mass,
            Morale
        };
    };

    class C_Human : public C_Actor
    {
    public:
        C_Human_Interface* GetInterface()
        {
            return reinterpret_cast<C_Human_Interface*>(this);
        }

        ai_action_manager* GetActionManager()
        {
            __asm
            {
                mov esi, this
                lea eax, dword ptr ds : [esi + 0x1164]
            }
        }

        void SetProperty(C_Human_Enum::Property property, float value)
        {
            *(float*)((unsigned long)this + (0x640 + property * 4)) = value;
        }

        float GetProperty(C_Human_Enum::Property property)
        {
            return *(float*)((unsigned long)this + (0x640 + property * 4));
        }

        int Hit(int hitType, const S_vector & unk1, const S_vector & unk2, const S_vector & unk3, float damage, MafiaSDK::C_Actor* atacker, unsigned long hittedPart, MafiaSDK::I3D_Frame* targetFrame)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Hit;
            int returnVal = 0;

            __asm
            {
                push targetFrame
                push hittedPart
                push atacker
                push damage
                push unk3
                push unk2
                push unk1
                push hitType
                mov ecx, this
                call funcAddress
                mov returnVal, eax
            }

            return returnVal;
        }

        void SetBehavior(C_Human_Enum::BehaviorStates behavior)
        {
            /*__asm
            {
                mov ecx, this
                mov ax, behavior
                mov byte ptr ds : [ ecx + 0x5FC ], ax
            }*/
            *(byte*)(this + 0x5FC) = behavior;
        }

        void SetShooting(float shooting)
        {
            *(float*)(this + 0x628) = shooting;
        }

        void Intern_UseCar(C_Car* car, int seatID)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Intern_UseCar;

            __asm
            {
                push seatID
                push car
                mov ecx, this
                call funcAddress
            }
        }

        void EraseDynColls()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::EraseDynColls;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Intern_FromCar(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Intern_FromCar;
            I3D_Frame* vehicleFrame  = *(I3D_Frame**)((unsigned long)this + 0x68);

            if (vehicleFrame) {
                __asm
                {
                    push vehicleFrame
                    mov ecx, this
                    call funcAddress
                }
            }
        }

        void Intern_UseRailway(BOOL usingRailway)
        {

            unsigned long funcAddress = 0x00571B90;

            __asm
            {
                push usingRailway
                mov ecx, this
                call funcAddress
            }
        }

        void PoseSetPoseAimed(S_vector pose)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::PoseSetPoseAimed;

            __asm
            {
                push pose.z
                push pose.y
                push pose.x
                mov ecx, this
                call funcAddress
            }
        }

        void PoseSetPoseNormal(const S_vector pose)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::PoseSetPoseNormal;

            __asm
            {
                push pose.z
                push pose.y
                push pose.x
                mov ecx, this
                call funcAddress
            }
        }

        void RepairWeaponPosition(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::RepairWeaponPosition;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Intern_ForceDeath(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Intern_ForceDeath;
            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void ChangeWeaponModel(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::ChangeWeaponModel;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Do_ChangeWeapon(int weaponId, BOOL unk1)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_ChangeWeapon;

            __asm
            {
                push unk1
                push weaponId
                mov ecx, this
                call funcAddress
            }
        }

        void Do_PlayAnim( const char* animationName )
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_PlayAnim;

            __asm
            {
                push 1
                push 1
                push animationName
                mov ecx, this
                call funcAddress
            }
        }

        void Do_StopAnim(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_StopAnim;


            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Use_Actor(C_Actor* actor, int unk1, int unk2, int unk3)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Use_Actor;

            __asm
            {
                push unk3
                push unk2
                push unk1
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void Do_Shoot(BOOL isShooting, const S_vector & vPos)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Shoot;

            __asm
            {
                push vPos
                push isShooting
                mov ecx, this
                call funcAddress
            }
        }

        void Do_ThrowGranade(S_vector vPos)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_ThrowGranade;

            __asm
            {
                lea eax, vPos
                push eax
                mov ecx, this
                call funcAddress
            }
        }

        void Do_Reload(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Reload;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Do_Holster(void)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Holster;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Go_Front(BOOL run)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_Front;

            __asm
            {
                push run
                mov ecx, this
                call funcAddress
            }
        }

        void Do_Crouched(BOOL crouched)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Crouched;

            __asm
            {
                push crouched
                mov ecx, this
                call funcAddress
            }
        }

        void Do_Jump()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Jump;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Do_WeaponDrop()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_WeaponDrop;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Go_Back()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_Back;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }


        void Go_Left(BOOL run)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_Left;

            __asm
            {
                push run
                mov ecx, this
                call funcAddress
            }
        }

        void Go_Right(BOOL run)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_Right;

            __asm
            {
                push run
                mov ecx, this
                call funcAddress
            }
        }

        void Go_FrontLeft(BOOL run)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_FrontLeft;

            __asm
            {
                push run
                mov ecx, this
                call funcAddress
            }
        }

        void Go_FrontRight(BOOL run)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_FrontRight;

            __asm
            {
                push run
                mov ecx, this
                call funcAddress
            }
        }

        void Go_BackLeft()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_BackLeft;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Go_BackRight()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Go_BackRight;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Do_Breath()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Breath;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Do_ThrowCocotFromCar(C_Car * car, int seatId)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_ThrowCocotFromCar;

            __asm
            {
                push seatId
                push car
                mov ecx, this
                call funcAddress
            }
        }

        void EineMeineKleineAutoInLuft(C_Car * car)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::EineMeineKleineAutoInLuft;

            __asm
            {
                push car
                mov ecx, this
                call funcAddress
            }
        }

        void NewScream(unsigned int screamId)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::NewScream;


            __asm
            {
                push screamId
                mov ecx, this
                call funcAddress
            }
        }

        void Intern_ChangeModel(const char* modelName)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Intern_ChangeModel;


            __asm
            {
                push 0
                push modelName
                mov ecx, this
                call funcAddress
            }
        }

        void G_Inventory_Insert(S_GameItem & newItem, BOOL unk)
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::G_Inventory_Insert;
            G_Inventory* inventory = GetInventory();

            __asm
            {
                push unk
                push newItem
                mov ecx, inventory
                call funcAddress
            }
        }

        void G_Inventory_AddItem(S_GameItem newItem)
        {
            this->G_Inventory_Insert(newItem, false);
            this->ChangeWeaponModel();
        }

        void G_Inventory_RemoveWeapon(short int iWeaponId)
        {
            G_Inventory* inventory = GetInventory();
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::G_Inventory_Remove;

            __asm
            {
                sub esp, 500h
                mov ax, iWeaponId
                lea ecx, dword ptr ss : [esp + 0x1EC]
                push 0
                push ecx
                mov ecx, inventory
                mov word ptr ss : [esp + 0x1F4], ax
                call funcAddress
                add esp, 500h
            }

            this->ChangeWeaponModel();
        }

        G_Inventory* GetInventory()
        {
            G_Inventory *inventory = nullptr;
            __asm
            {
                mov esi, this
                lea     ecx, [esi + 480h]
                mov inventory, ecx
            }

            return inventory;
        }

        S_GameItem* G_Inventory_GetWeaponInRightHand()
        {
            S_GameItem* wep;
            G_Inventory *inventory = GetInventory();

            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::G_Inventory_GetWeaponInRightHand;

            __asm
            {
                mov ecx, inventory
                call funcAddress
                mov wep, eax
            }

            return wep;
        }

        S_GameItem* G_Inventory_GetWeaponInLeftHand()
        {
            G_Inventory* inventory = GetInventory();
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::G_Inventory_GetWeaponInLeftHand;

            __asm
            {
                mov ecx, inventory
                call funcAddress
            }
        }

        void G_Inventory_SelectByID(int id)
        {
            G_Inventory* inventory = GetInventory();
            unsigned long funcAddress = 0x006081D0;

            __asm
            {
                push 0
                push id
                mov ecx, inventory
                call funcAddress
            }
        }

        void InventoryTruncate()
        {
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::InventoryTruncate;
            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        unsigned int G_Inventory_GetAmmo(int & unk1)
        {
            G_Inventory* inventory = GetInventory();
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::G_Inventory_GetAmmo;

            __asm
            {
                push unk1
                mov ecx, inventory
                call funcAddress
            }
        }

		void Death(void)
		{
			unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Death;
			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void RecompileDeathPos(void)
		{
			unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::RecompileDeathPos;
			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void Do_DeadBodyDrop(void)
		{
			unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_DeadBodyDrop;
			__asm
			{
				mov ecx, this
				call funcAddress
			}
		}

		void Do_Aimed(void)
		{
			unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_Aimed;
			__asm
			{
				push 0
				mov ecx, this
				mov eax, funcAddress // C_human::Do_Aimed
				call eax
			}
		}


    };

    namespace C_Human_Hooks
    {
        inline void HookOnHumanHit(std::function<int(MafiaSDK::C_Human*, int, const S_vector &, const S_vector &, const S_vector &, float, MafiaSDK::C_Actor*, unsigned long, MafiaSDK::I3D_Frame*)> funcitonPointer);
        void HookUseActor(std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Actor*, int, int, int)> functionPointer);
        void HookDoThrowCocotFromCar(std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Car*, int)> functionPointer);
        void HookHumanSetAimPose(std::function<void(MafiaSDK::C_Human*, const S_vector&)> functionPointer);
        void HookHumanSetNormalPose(std::function<void(MafiaSDK::C_Human*, const S_vector&)> functionPointer);

#ifdef MAFIA_SDK_IMPLEMENTATION
        namespace FunctionsPointers
        {
            extern std::function<int(MafiaSDK::C_Human*, int, const S_vector &, const S_vector &, const S_vector &, float, MafiaSDK::C_Actor*, unsigned long, MafiaSDK::I3D_Frame*)> humanHit;
            extern std::function<void(const S_vector &)> humanShoot;
            extern std::function<void(MafiaSDK::C_Human*, byte)> humanDoWeaponChange;
            extern std::function<void(MafiaSDK::C_Human*)> humanDoWeaponDrop;
            extern std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Actor*, int, int, int)> useActor;
            extern std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Car*, int)> doThrowCocotFromCar;
            extern std::function<void(MafiaSDK::C_Human*, const S_vector&)> humanSetAimPose;
            extern std::function<void(MafiaSDK::C_Human*, const S_vector&)> humanSetNormalPose;
        };

        namespace Functions
        {
            inline int HumanHit(MafiaSDK::C_Human* thisInstance, int hitType, const S_vector & unk1, const S_vector & unk2, const S_vector & unk3, float damage, MafiaSDK::C_Actor* atacker, unsigned long hittedPart, MafiaSDK::I3D_Frame* targetFrame)
            {
                if (FunctionsPointers::humanHit != nullptr)
                    return FunctionsPointers::humanHit(thisInstance, hitType, unk1, unk2, unk3, damage, atacker, hittedPart, targetFrame);

                else return 0;
            }

            inline void HumanShoot(const S_vector & position)
            {
                if (FunctionsPointers::humanShoot != nullptr)
                    FunctionsPointers::humanShoot(position);
            }

            inline void HumanDoWeaponDrop(MafiaSDK::C_Human* thisInstance)
            {
                if (FunctionsPointers::humanDoWeaponDrop != nullptr)
                    FunctionsPointers::humanDoWeaponDrop(thisInstance);
            }

            inline void HumanDoWeaponChange(MafiaSDK::C_Human* thisInstance, byte weaponId)
            {
                if (FunctionsPointers::humanDoWeaponChange != nullptr)
                {
                    FunctionsPointers::humanDoWeaponChange(thisInstance, weaponId);
                }
            }

            inline void UseActor(MafiaSDK::C_Human* human, MafiaSDK::C_Actor* actor, int unk1, int unk2, int unk3)
            {
                if (FunctionsPointers::useActor != nullptr)
                    FunctionsPointers::useActor(human, actor, unk1, unk2, unk3);
            }

            inline void DoThrowCocotFromCar(MafiaSDK::C_Human* human, MafiaSDK::C_Car* car, int seatId)
            {
                if (FunctionsPointers::doThrowCocotFromCar != nullptr)
                    FunctionsPointers::doThrowCocotFromCar(human, car, seatId);
            }

            inline void HumanSetAimPose(MafiaSDK::C_Human* human, const S_vector& pos)
            {
                if (FunctionsPointers::humanSetAimPose != nullptr)
                    FunctionsPointers::humanSetAimPose(human, pos);
            }

            inline void HumanSetNormalPose(MafiaSDK::C_Human* human, const S_vector& pos)
            {
                if (FunctionsPointers::humanSetNormalPose != nullptr)
                    FunctionsPointers::humanSetNormalPose(human, pos);
            }
        };

        namespace NakedFunctions
        {
            extern void HumanHitOne();

            extern void HumanHitTwo();

            extern void HumanShoot();

            extern void HumanDoWeaponChangeOne();

            extern void HumanDoWeaponChangeTwo();

            extern void HumanDoWeaponDrop();

            extern void UseActor();
            extern void* useActorReturn;

            extern void DoThrowCocotFromCar();
            extern void* doThrowCocotFromCarReturn;

            extern void SetAimPose();
            extern void* setAimPoseReturn;

            extern void SetNormalPose();
            extern void* setNormalPoseReturn;
        };

        inline void HookOnHumanHit(std::function<int(MafiaSDK::C_Human*, int, const S_vector &, const S_vector &, const S_vector &, float, MafiaSDK::C_Actor*, unsigned long, MafiaSDK::I3D_Frame*)> functionPointer)
        {
            FunctionsPointers::humanHit = functionPointer;

            MemoryPatcher::InstallJmpHook(0x00507EE1, (unsigned long)&NakedFunctions::HumanHitOne);
            MemoryPatcher::InstallJmpHook(0x00594230, (unsigned long)&NakedFunctions::HumanHitTwo);
        }

        inline void HookOnHumanShoot(std::function<void(const S_vector &)> functionPointer)
        {
            FunctionsPointers::humanShoot = functionPointer;
            MemoryPatcher::InstallJmpHook(0x00591416, (unsigned long)&NakedFunctions::HumanShoot);
        }

        inline void HookHumanDoWeaponChange(std::function<void(MafiaSDK::C_Human* thisInstance, byte weaponId)> functionPointer)
        {
            FunctionsPointers::humanDoWeaponChange = functionPointer;

            MemoryPatcher::InstallJmpHook(0x005903ED, (unsigned long)&NakedFunctions::HumanDoWeaponChangeOne);
            MemoryPatcher::InstallJmpHook(0x005902FA, (unsigned long)&NakedFunctions::HumanDoWeaponChangeTwo);
        }

        inline void HookHumanDoWeaponDrop(std::function<void(MafiaSDK::C_Human* thisInstance)> functionPointer)
        {
            FunctionsPointers::humanDoWeaponDrop = functionPointer;

            MemoryPatcher::InstallJmpHook(0x0059024D, (unsigned long)&NakedFunctions::HumanDoWeaponDrop);
        }

        inline void HookUseActor(std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Actor*, int, int, int)> functionPointer)
        {
            FunctionsPointers::useActor = functionPointer;

            NakedFunctions::useActorReturn = (void*)(C_Human_Enum::FunctionsAddresses::Use_Actor + 6);
            MemoryPatcher::InstallJmpHook(C_Human_Enum::FunctionsAddresses::Use_Actor, (unsigned long)&NakedFunctions::UseActor);
        }

        inline void HookDoThrowCocotFromCar(std::function<void(MafiaSDK::C_Human*, MafiaSDK::C_Car*, int)> functionPointer)
        {
            FunctionsPointers::doThrowCocotFromCar = functionPointer;

            NakedFunctions::doThrowCocotFromCarReturn = (void*)(C_Human_Enum::FunctionsAddresses::Do_ThrowCocotFromCar + 7);
            MemoryPatcher::InstallJmpHook(C_Human_Enum::FunctionsAddresses::Do_ThrowCocotFromCar, (unsigned long)&NakedFunctions::DoThrowCocotFromCar);
        }

        inline void HookHumanSetAimPose(std::function<void(MafiaSDK::C_Human*, const S_vector&)> functionPointer)
        {
            FunctionsPointers::humanSetAimPose = functionPointer;

            NakedFunctions::setAimPoseReturn = (void*)(C_Human_Enum::FunctionsAddresses::PoseSetPoseAimed + 6);
            MemoryPatcher::InstallJmpHook(C_Human_Enum::FunctionsAddresses::PoseSetPoseAimed, (unsigned long)&NakedFunctions::SetAimPose);
        }

        inline void HookHumanSetNormalPose(std::function<void(MafiaSDK::C_Human*, const S_vector&)> functionPointer)
        {
            FunctionsPointers::humanSetNormalPose = functionPointer;

            NakedFunctions::setNormalPoseReturn = (void*)(C_Human_Enum::FunctionsAddresses::PoseSetPoseNormal + 6);
            MemoryPatcher::InstallJmpHook(C_Human_Enum::FunctionsAddresses::PoseSetPoseNormal, (unsigned long)&NakedFunctions::SetNormalPose);
        }
#endif
    };
};

#endif
