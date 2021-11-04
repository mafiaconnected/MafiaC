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
    struct C_Human_Interface
    {
        C_Entity_Interface entity;
        PADDING(C_Human_Interface, _pad0, 0x4);
        byte animStateLocal;
        byte isInAnimWithCarLocal;
        PADDING(C_Human_Interface, _pad1, 0x2);
        byte animState;
        byte isInAnimWithCar;
        PADDING(C_Human_Interface, _pad2, 0x22);
        C_Car* playersCar;
        C_Car* carLeavingOrEntering;
        PADDING(C_Human_Interface, _pad3, 0x144);
        bool isDucking;
        bool isAiming;
        PADDING(C_Human_Interface, _pad4, 0x6);
        bool isShooting;
        PADDING(C_Human_Interface, _pad4b, 0x0F);
        bool isReloading;
        PADDING(C_Human_Interface, _pad5, 0x11B);
        I3D_Frame * neckFrame;
        PADDING(C_Human_Interface, _pad6, 0x164);
        G_Inventory inventory;
        PADDING(C_Human_Interface, _pad7, 0xF4);
        float inCarRotation;
        PADDING(C_Human_Interface, _pad8, 0x4C);
        float health;
    };

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
            EraseDynColls = 0x00575ED0
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
            unsigned long funcAddress = C_Human_Enum::FunctionsAddresses::Do_PlayAnim;


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
    };

    namespace C_Human_Hooks
    {
        inline void HookOnHumanHit(std::function<int(MafiaSDK::C_Human*, int, const S_vector &, const S_vector &, const S_vector &, float, MafiaSDK::C_Actor*, unsigned long, MafiaSDK::I3D_Frame*)> funcitonPointer);

#ifdef MAFIA_SDK_IMPLEMENTATION
        namespace FunctionsPointers
        {
            extern std::function<int(MafiaSDK::C_Human*, int, const S_vector &, const S_vector &, const S_vector &, float, MafiaSDK::C_Actor*, unsigned long, MafiaSDK::I3D_Frame*)> humanHit;
            extern std::function<void(const S_vector &)> humanShoot;
            extern std::function<void(MafiaSDK::C_Human*, byte)> humanDoWeaponChange;
            extern std::function<void(MafiaSDK::C_Human*)> humanDoWeaponDrop;
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
        };

        namespace NakedFunctions
        {
            extern void HumanHitOne();
            
            extern void HumanHitTwo();
            
            extern void HumanShoot();
           
            extern void HumanDoWeaponChangeOne();
            
            extern void HumanDoWeaponChangeTwo();

            extern void HumanDoWeaponDrop();
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
#endif
    };
};

#endif
