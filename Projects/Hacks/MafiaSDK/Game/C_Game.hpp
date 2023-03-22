#ifndef _CGAME_H_
#define _CGAME_H_

namespace MafiaSDK
{
    struct C_Game_Interface
    {
        PADDING(C_Game_Interface, _pad0, 0x4C);
        C_Camera mCamera;									// 76-77
        PADDING(C_Game_Interface, _pad1, 0x94);
        C_Player* mLocalPlayer;								// 228-232
    };

	//const size_t Offset = offsetof(C_Game_Interface, mLocalPlayer);
	//const size_t Size = sizeof(C_Game_Interface::mLocalPlayer);

    namespace C_Game_Enum
    {
        enum FunctionAddresses
        {
            NewExplosion = 0x005AAE10,
            ScoreSet = 0x005B9FE0,
            ScoreSetOn = 0x005B9FA0,
            SetTrafficVisible = 0x5A8470,
            PlayAllSounds = 0x5B85F0,
            PauseAllSounds = 0x5B8430,
            CreateStream = 0x5B6110,
            DestroyStream = 0x5B62C0,
            StreamGetFreeId = 0x5B5E90,
            GetStreamById = 0x5B64C0,
            ScoreEnabled = 0x5B9FA0,
            SetScore = 0x5B9FE0,
            AddTemporaryActor = 0x5A77C0,
            RemoveTemporaryActor = 0x5A79A0,//
            SetCamerRotRepair = 0x005BA010,
            Init = 0x005A0810,
            SetHuman = 0x005A07E0,
            UpdateMusicVolume = 0x005B6600
        };
    };

    namespace C_Game_Patches
    {
        inline void PatchDisableInventory()
        {
            // Disable - inventory
            BYTE disableInventory[] = "\xEB\x15";
            MemoryPatcher::PatchAddress(0x00592001, disableInventory, sizeof(disableInventory));

            // Disable - inventory in vehicle
            BYTE disableInventoryCar[] = "\xEB\x0F";
            MemoryPatcher::PatchAddress(0x00590139, disableInventoryCar, sizeof(disableInventoryCar));
        }

        inline void PatchDisableLogos()
        {
            // Disable logos
            MemoryPatcher::InstallNopPatch(0x5BFDDC, 60);
        }

        inline void PatchDisablePauseMenu()
        {
            MemoryPatcher::InstallJmpHook(0x005BE919, 0x005BE9C9);
        }

        inline void PatchDisablePleaseWait()
        {
            // Disable - Please Wait text
            //MemoryPatcher::InstallNopPatch(0x60D391, 15);
        }

        inline void PatchJumpToGame(const char* mission_name)
        {
            //Replace freeride with menu
            std::string klz = std::string("missions\\") + std::string(mission_name) + std::string("\\tree.klz");
            strcpy((char*)0x637A78, klz.c_str());
            strcpy((char*)0x637A94, mission_name);

            //MemoryPatcher::PatchAddress(0x5EA2E0, 0xC3);
            //NOTE(DavoSK): Dont disable all menus only profile
            MemoryPatcher::InstallJmpHook(0x005EBB24, 0x005EBB2D);
            MemoryPatcher::PatchAddress(0x5BF3D8, 4231955727); // skip to 0x5BF01C
            MemoryPatcher::PatchAddress(0x5BEE7A, 105961); // E9 9D0100000
        }

        inline void PatchRemovePlayer()
        {
            // Never loading the FreeRide setup player
            MemoryPatcher::InstallNopPatch(0x5BF06E, 5);
            MemoryPatcher::InstallNopPatch(0x5BF0E2, 5);
        }

        inline void PatchDisableGameMenu()
        {
            // Disable - game menu
            BYTE disableGameMenu[] = "\xE9\xAB\x00";
            MemoryPatcher::PatchAddress(0x005BE919, disableGameMenu, sizeof(disableGameMenu));
        }

        inline void PatchDisableSuspendProcess()
        {
            // Disable - suspended process
            BYTE disableProcess[] = "\xEB\x69";
            MemoryPatcher::PatchAddress(0x1006DAB7, disableProcess, sizeof(disableProcess));
        }

        inline void PatchDisableGameScripting()
        {
            // Disable - mafia scripts
            BYTE disableScripts[] = "\xEB\x3F";
            MemoryPatcher::PatchAddress(0x00461636, disableScripts, sizeof(disableScripts));
        }

        inline void PatchCustomPlayerRespawning()
        {
            // Water sink respawn
            BYTE respawnWater[] = "\xE9\xC2\x00\x00\x00";
            MemoryPatcher::PatchAddress(0x005A5290, respawnWater, sizeof(respawnWater));

            BYTE noEndMissionJmp[] = "\xE9\xAD\x82\x00";
            MemoryPatcher::PatchAddress(0x004716ED, noEndMissionJmp, sizeof(noEndMissionJmp));

            //No change model when blow in car
            MemoryPatcher::InstallNopPatch(0x0058A5DA, 14);

            //Respwn no camera unbind meybe (TODO)
            //No delete actor and anims
            //BYTE noDeleteActors[] = "\xC7\x86\x0C\x04\x00\x00";*/
            /*MemoryPatcher::InstallNopPatch(0x00572F3B, 10);
            MemoryPatcher::InstallNopPatch(0x0057838F, 10);
            MemoryPatcher::InstallNopPatch(0x0057AB8C, 10);
            MemoryPatcher::InstallNopPatch(0x0057ACD1, 10);
            MemoryPatcher::InstallNopPatch(0x0058A780, 10);*/
        }
    };

    namespace C_Game_Hooks
    {
        void HookOnGameTick(std::function<void()> funcitonPointer);
        void HookOnGameInit(std::function<void()> funcitonPointer);
        void HookOnHumanShoot(std::function<void(C_Human*, S_vector)> functionPointer);
        void HookLocalPlayerFallDown(std::function<void()> functionPointer);

#ifdef MAFIA_SDK_IMPLEMENTATION
        namespace FunctionsPointers
        {
            extern std::function<void()> gameTick;
            extern std::function<void()> gameDone;
            extern std::function<void()> gameInit;
            extern std::function<void()> localPlayerFallDown;
            extern std::function<void(C_Human*, S_vector)> humanOnShoot;
        };

        namespace Functions
        {
            inline void GameTick()
            {
                if(FunctionsPointers::gameTick != nullptr)
                    FunctionsPointers::gameTick();
            }

            inline void GameInit()
            {
                if (FunctionsPointers::gameInit != nullptr)
                    FunctionsPointers::gameInit();
            }

            inline void GameDone()
            {
                if (FunctionsPointers::gameDone != nullptr)
                    FunctionsPointers::gameDone();
            }

            inline void LocalPlayerFallDown()
            {
                if (FunctionsPointers::localPlayerFallDown != nullptr)
                    FunctionsPointers::localPlayerFallDown();
            }


        };

        namespace NakedFunctions
        {
            extern void GameTick();
            extern void GameInit();
            extern void GameDone();
            extern void LocalPlayerFallDown();
        };

        inline void HookOnGameTick(std::function<void()> funcitonPointer)
        {
            FunctionsPointers::gameTick = funcitonPointer;
            MemoryPatcher::InstallJmpHook(0x005A6D40, (unsigned long)&NakedFunctions::GameTick);
        }

        inline void HookOnGameDone(std::function<void()> functionPointer)
        {
            FunctionsPointers::gameDone = functionPointer;
            MemoryPatcher::InstallJmpHook(0x005A4D15, (unsigned long)&NakedFunctions::GameDone);
        }

        inline void HookOnGameInit(std::function<void()> functionPointer)
        {
            FunctionsPointers::gameInit = functionPointer;
            MemoryPatcher::InstallJmpHook(0x005A395B, (unsigned long)&NakedFunctions::GameInit);
        }

        inline void HookHumanOnShoot(std::function<void()> functionPointer)
        {

        }

        inline void HookLocalPlayerFallDown(std::function<void()> functionPointer)
        {
            FunctionsPointers::localPlayerFallDown = functionPointer;
            MemoryPatcher::InstallJmpHook(0x005A543B, (unsigned long)&NakedFunctions::LocalPlayerFallDown);
        }
#endif
    };

    class C_Game
    {
    public:
        C_Game_Interface* GetInterface()
        {
            return reinterpret_cast<C_Game_Interface*>(this);
        }

        C_Camera* GetCamera()
        {
            return &this->GetInterface()->mCamera;
        }

        C_Player* GetLocalPlayer()
        {
            return this->GetInterface()->mLocalPlayer;
        }

        C_BloodManager* GetBloodManager()
        {
            return reinterpret_cast<C_BloodManager*>((unsigned long)this + 0x2D90);
        }

        void SetLocalPlayer(C_Player* player)
        {
            this->GetInterface()->mLocalPlayer = player;
        }

        void SetHuman(C_Actor* actor)
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::SetHuman;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void UpdateMusicVolume()
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::UpdateMusicVolume;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void AddTemporaryActor(C_Actor* actor)
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::AddTemporaryActor;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void SetCameraRotRepair()
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::SetCamerRotRepair;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        void RemoveTemporaryActor(C_Actor* actor)
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::RemoveTemporaryActor;

            __asm
            {
                push actor
                mov ecx, this
                call funcAddress
            }
        }

        void SetTrafficVisible(BOOL toggle)
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::SetTrafficVisible;

            __asm
            {
                push toggle
                call funcAddress
            }
        }

        void PlayAllSound(void)
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::PlayAllSounds;

            __asm
            {
                push 1
                mov ecx, this
                call funcAddress
            }

        }

        void PauseAllSound(void)
        {
            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::PauseAllSounds;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }


        int CreateStream(const char * streamName)
        {

            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::CreateStream;
            unsigned long funcAddressGetId = C_Game_Enum::FunctionAddresses::StreamGetFreeId;

            int returnId = -1;

            __asm
            {
                //Get Free Id for stream
                push 0
                mov ecx, this
                call funcAddressGetId
                mov returnId, eax

                //Create stream
                push eax
                push streamName
                mov ecx, this
                call funcAddress
            }

            return returnId;
        }

        unsigned long GetStreamById(int streamId)
        {

            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::GetStreamById;
            unsigned long returnAddress = NULL;

            __asm
            {
                push streamId
                mov ecx, this
                call funcAddress
                mov returnAddress, eax
            }

            return returnAddress;
        }

        //TODO(DavoSK): This is engine call reverse it from engine class
        void SetSoundsVolume(float level)
        {
            __asm
            {
                mov eax, dword ptr ds: [0x647EDC]
                test eax, eax
                je skip
                mov ecx, dword ptr ds : [eax]
                push level
                push eax
                call dword ptr ds : [ecx + 0x14]
                skip:
            }
        }

        void SetStreamVolume(int streamId, float volume)
        {
            unsigned long streamAddress = GetStreamById(streamId);

            __asm
            {
                push volume
                push esi
                mov ecx, dword ptr ds : [esi]
                call dword ptr ds : [ecx + 0x38]
            }
        }

        void PlayStream(int streamId)
        {
            unsigned long streamAddress = GetStreamById(streamId);

            __asm
            {
                mov esi, streamAddress
                mov eax, dword ptr ds : [esi]
                push esi
                call dword ptr ds : [eax + 14h]
            }

            //Set volume of stream to same value as in options
            float currentMainVolume = *reinterpret_cast<float*>(0x6D4B10);
            SetStreamVolume(streamId, currentMainVolume);
        }

        void PauseStream(int streamId)
        {
            unsigned long streamAddress = GetStreamById(streamId);

            __asm
            {
                mov esi, streamAddress
                mov eax, dword ptr ds : [esi]
                push esi
                call dword ptr ds : [eax + 0x1C]
            }
        }

        void DestroyStream(int streamId)
        {

            unsigned long funcAddress = C_Game_Enum::FunctionAddresses::DestroyStream;
            unsigned long streamAddress = GetStreamById(streamId);

            __asm
            {
                push streamAddress
                mov ecx, this
                call funcAddress
            }
        }

        void NewExplosion(C_Actor* actor, S_vector & pos, float radius, float force, BOOL b1, BOOL b2, BOOL b3, int int1)
        {
            unsigned long funcAddr = C_Game_Enum::FunctionAddresses::NewExplosion;

            __asm
            {
                push int1
                push b3
                push b2
                push b1
                push force
                push radius
                push pos
                push actor
                mov ecx, this
                call funcAddr
            }
        }


        void ScoreSet(int score)
        {
            unsigned long funcAddr = C_Game_Enum::FunctionAddresses::ScoreSet;

            __asm
            {
                push score
                mov ecx, this
                call funcAddr
            }
        }

        void ScoreOn(BOOL on)
        {
            unsigned long funcAddr = C_Game_Enum::FunctionAddresses::ScoreSetOn;

            __asm
            {
                push on
                mov ecx, this
                call funcAddr
            }
        }

        void Init()
        {
            unsigned long funcAddr = C_Game_Enum::FunctionAddresses::Init;

            __asm
            {
                mov ecx, this
                call funcAddr
            }
        }
    };
};

#endif
