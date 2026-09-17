/*
Copyright 2018 Dávid Svitana

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

#ifndef _CMISSION_H_
#define _CMISSION_H_

namespace MafiaSDK
{
    namespace C_Mission_Enum
    {
        enum FunctionAddresses
        {
            CLASS_CMISSION = C_MISSION_CLASS,
            CreateActor = 0x0053F7D0,
            AddActor = 0x0053FEA0,
            UnloadActor = 0x00540080,
            DelActor = 0x00540130,
            DelActors = 0x00540240,
            FindActorByName = 0x00540490,
            Open = 0x005409D0,
            Close = 0x005405E0,

            // Scene-graph actor creation path (separate from C_Mission::CreateActor); hook target/return
            // for MultiplayerModOne's Hooks.cpp SceneCreateActor. Unfinished - see comment at its call site.
            SceneCreateActor = 0x00544AFF,
            SceneCreateActorReturn = 0x00544B07
        };

		enum MissionID
		{
			FREERIDE = 29,
			FREERIDE_NOC = 30,
            TUTORIAL = 17,
		};

        enum ObjectTypes
        {
            GhostObject = 1,
            Player = 2,
            Car = 4,
            Script = 5,
            Door = 6,
            Trolley = 8,
            Model = 9,
            Bottle = 10,
            Traffic = 12,
            Pedestrian = 18,
            Bridge = 20,
            Dog = 21,
            Plane = 22,
            RailRoute = 24,
            Pumpar = 25,
            Human = 27,
            RaceCamera = 28,
            Wagon = 30,
            Clock = 34,
            Physical = 35,
            Truck = 36,
            InitScript = 155
        };

        // Ported from reMafia's GetActorTypeName (Actors/C_actor.cpp, same author,
        // MafiaOrbitCam/Vendors/reMafia), adapted to this enum's own names/values.
        inline const char* GetObjectTypeName(ObjectTypes type)
        {
            switch (type)
            {
                case GhostObject: return "GhostObject";
                case Player: return "Player";
                case Car: return "Car";
                case Script: return "Script";
                case Door: return "Door";
                case Trolley: return "Trolley";
                case Model: return "Model";
                case Bottle: return "Bottle";
                case Traffic: return "Traffic";
                case Pedestrian: return "Pedestrian";
                case Bridge: return "Bridge";
                case Dog: return "Dog";
                case Plane: return "Plane";
                case RailRoute: return "RailRoute";
                case Pumpar: return "Pumpar";
                case Human: return "Human";
                case RaceCamera: return "RaceCamera";
                case Wagon: return "Wagon";
                case Clock: return "Clock";
                case Physical: return "Physical";
                case Truck: return "Truck";
                case InitScript: return "InitScript";
                default: break;
            }
            return "Unknown";
        }

        enum PhysicsTypes
        {
            Stone = 0,
            Crate = 1,
            Barrel = 2,
            TrashBin = 3,
            TrafficSign = 4,
            PaperBox = 5,
            Concrete = 6,
            MetalPlate = 7,
            SmallWeapon0 = 8,
            SmallWeapon1 = 9,
            WoodenPlank = 10,
            SmallMetal = 11,
            BigWeapon = 12,
            CarLight = 13
        };

        enum MaterialTypes
        {
            Invalid = 0,
            Asphalt = 1,
            Pavement = 2,
            Sidewalk = 3,
            Concrete1 = 4,
            Grass = 5,
            Sand = 6,
            Clay = 7,
            Gravel = 8,
            Rock = 9,
            RailwayLine = 10,
            Leaves = 11,
            DoNotUse1 = 12,
            Wood0 = 13,
            Wood30 = 14,
            Wood70 = 15,
            Parquet = 16,
            Marble = 17,
            Carpet = 18,
            MetalConstruction = 19,
            MetalPlate0 = 20,
            MetalPlate30 = 21,
            MetalPlate70 = 22,
            DoNotUse2 = 23,
            Glass = 24,
            GlassUnbreakable = 25,
            DoNotUse = 26,
            WiredFence = 27,
            WoodenFence = 28,
            Hedge = 29,
            DoNotUse3 = 30,
            DeadlyWater = 31,
            DoNotUse4 = 32,
            Universal = 33,
            DoNotUse5 = 34,
            Body = 35,
            Plaster = 36,
            Junk = 37,
            Bricks = 38,
            DoNotUse6 = 39,
            DeadlyCollision = 40,
            DestructibleCollision = 41,
            Straw = 42,
            Butter = 43,
            SlippingWater = 44,
            StickingWater = 45,
            DirtyRoad = 46
        };
    }

    struct C_Mission_Interface
    {
        PADDING(C_Mission_Interface, _pad0, 0x24);
        C_Game * mGame;
    };

    namespace C_Mission_Hooks
    {
        void HookCreateActor(std::function<void(C_Mission_Enum::ObjectTypes)> functionPointer);

#ifdef MAFIA_SDK_IMPLEMENTATION
        namespace FunctionsPointers
        {
            extern std::function<void(C_Mission_Enum::ObjectTypes)> createActor;
        };

        namespace Functions
        {
            inline void CreateActor(C_Mission_Enum::ObjectTypes actorType)
            {
                if (FunctionsPointers::createActor != nullptr)
                    FunctionsPointers::createActor(actorType);
            }
        };

        namespace NakedFunctions
        {
            extern void CreateActor();
            extern void* createActorReturn;
        };

        inline void HookCreateActor(std::function<void(C_Mission_Enum::ObjectTypes)> functionPointer)
        {
            FunctionsPointers::createActor = functionPointer;

            NakedFunctions::createActorReturn = (void*)(C_Mission_Enum::FunctionAddresses::CreateActor + 6);
            MemoryPatcher::InstallJmpHook(C_Mission_Enum::FunctionAddresses::CreateActor, (unsigned long)&NakedFunctions::CreateActor);
        }
#endif
    };

    class C_Mission
    {
    public:
        C_Mission_Interface* GetMissionInterface()
        {
            return reinterpret_cast<C_Mission_Interface*>(this);
        }

        C_Game* GetGame()
        {
            return GetMissionInterface()->mGame;
        }

        // Defined below, after C_Mission_Extended (which it reads through).
        I3D_Sector* GetScene();

        C_Actor* CreateActor(C_Mission_Enum::ObjectTypes actorType)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::CreateActor;

            __asm
            {
                mov ecx, this
                push actorType
                call funcAddress

            }
        }

        void MapLoad(const char* mapName)
        {
            strcpy((char*)0x00647BA0, mapName);
            *(BYTE*)(0x00647B9C) = 1;
        }

        void AddActor(C_Actor* actor, BOOL unk)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::AddActor;

            __asm
            {
                push unk
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void UnloadActor(C_Actor* actor)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::UnloadActor;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void DelActor(C_Actor* actor)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::DelActor;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void DelActors()
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::DelActors;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        C_Actor* FindActorByName(const char* actorName)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::FindActorByName;

            __asm
            {
                push actorName
                mov ecx, this
                call funcAddress
            }
        }

        void Close()
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::Close;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void Open(const char* mission_name, BOOL unk1, unsigned int unk2, BOOL unk3)
        {
            unsigned long funcAddress = C_Mission_Enum::FunctionAddresses::Open;

            __asm
            {
                push unk3
                push unk2
                push unk1
                push mission_name
                mov ecx, this
                call funcAddress
            }
        }
    };

    inline C_Mission* GetMission()
    {
        return *reinterpret_cast<C_Mission**>(C_Mission_Enum::FunctionAddresses::CLASS_CMISSION);
    }

    inline C_Mission_Enum::MissionID GetCurrentMissionID() {
        return (C_Mission_Enum::MissionID)*(int*)(0x6BD8A8);
    }

    inline char* GetCurrentMissionName() {
        DWORD addr = (DWORD)GetModuleHandle(NULL) + 0x00247E60;
        return *(char**)(addr);
    }

	class C_TShift;
	class C_WebPath;
	class C_Roads;
	class C_ParticleManager;

	/*
		Ported from reMafia's C_mission.h (same author, MafiaOrbitCam/Vendors/reMafia) - a
		fuller field layout than C_Mission_Interface above, which only names `mGame` amid a
		single opaque 0x24-byte padding gap. Kept as its own separate type rather than merged
		in: cross-checking reMafia's field order against that already-relied-upon mGame offset
		(0x24 = 36) requires knowing vc6_vector<T>'s exact compiled size, and the estimate used
		elsewhere in this pass (allocator + 3 pointers = ~12-16 bytes) lands m_pGame a handful
		of bytes short of 36 - close enough to suspect the fields are right, not close enough
		to safely renumber a struct nothing has broken by leaving alone (see C_Vehicle.hpp's
		C_Vehicle_Extended for the same situation in more detail). Field offsets below are
		exactly as reMafia declared them, uncorrected - verify before relying on them.
	*/
	struct C_Mission_Extended
	{
		vc6_vector<C_Actor*> actors;
		I3D_Sector* scene; // reMafia calls this I3D_scene; MafiaSDK's own name for that type is I3D_Sector
		vc6_vector<void*> animModels; // element type is C_anim_model*, not yet reverse-engineered
		C_Game* game;
		C_TShift* tShift;
		C_WebPath* webPath;
		C_Roads* roads;
		vc6_vector<C_Actor*> sceneActors;
		vc6_vector<C_Actor*> actors2;
		vc6_vector<C_Program*> programs;
		PADDING(C_Mission_Extended, _pad6, 0x4);
		char* missionName;
		C_ParticleManager* particleManager;
		PADDING(C_Mission_Extended, _pad7, 0x10);
		vc6_vector<C_Actor*> transparentObjects;
		PADDING(C_Mission_Extended, _pad8, 0x1C);
		vc6_vector<C_Actor*> models;
		vc6_vector<C_Actor*> activeActors;
		vc6_vector<C_Actor*> actorSounds;
	};

	/*
		Resolves C_Mission_Extended's own "uncorrected, verify before relying on them" caveat
		above, for its first three fields (actors, scene, animModels) specifically:
		MafiaSDK::vc6_vector<T*>'s real layout ({ allocator; _First; _Last; _End; }) is a
		1-byte empty allocator padded to 4 plus three 4-byte pointers = 16 bytes on this
		build's x86 target, so actors(0x10) + scene(0x04) lands `game` at offset 0x24 -
		exactly the offset already verified independently via C_Mission_Interface::mGame.
		That match is what justifies reading `scene` here; fields after `game` are still
		unverified.
	*/
	inline I3D_Sector* C_Mission::GetScene()
	{
		return reinterpret_cast<C_Mission_Extended*>(this)->scene;
	}
}

#endif
