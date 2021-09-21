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

#ifndef _CCAR_H_
#define _CCAR_H_



namespace MafiaSDK
{
	struct C_Car_Interface
	{
		C_Entity_Interface		 entity;
		PADDING(C_Car_Interface, _pad0, 0x4);
		C_Vehicle_Interface	vehicle_interface;
	};

	namespace C_Car_Enum
	{
		enum FunctionsAddresses
		{
			Engine = 0x004E1CE0,
			SetEngineOn = 0x004CB5B0,
            SetTransparency = 0x004233E0,
            RepairPosition = 0x0041E690,
			SetGear = 0x004CB070,
			GearSnd = 0x004ED810,
			GetSeatProperty = 0x0041DC30,
			GetOwner = 0x0041DEC0,
			Update = 0x0041FAC0,
			PrepareDropOutWheel = 0x00426DD0,
			PrepareDropOut = 0x00426EC0,
			CarExplosion = 0x00421D60
		};
	};

	class C_Car : public C_Actor
	{
	public:
		C_Car_Interface* GetInterface()
		{
			return reinterpret_cast<C_Car_Interface*>(this);
		}

		void Update(float dt)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::Update;

			__asm
			{
				push dt
				mov ecx, this
				call functionAddress
			}
		}

		void AI(float dt)
		{
			__asm
			{
				mov ecx, this
				mov eax, dword ptr ds : [ecx]
				push dt
				call dword ptr ds : [eax + 0x34]
			}
		}

		void GetSeatProperty(int seat_idx, bool & unk1, bool & unk2, bool & unk3, bool & unk4)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::GetSeatProperty;

			__asm
			{
				push unk4
				push unk3
				push unk2
				push unk1
				push seat_idx
				mov ecx, this
				call functionAddress
			}
		}

        void SetTransparency(float alpha)
        {
            unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::SetTransparency;

            __asm
            {
                push alpha
                mov ecx, this
                call functionAddress
            }
        }

        void RepairPosition(BOOL repairWindows)
        {
            unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::RepairPosition;

            __asm
            {
                push repairWindows
                mov ecx, this
                call functionAddress
            }
        }

        void SetColsOn(bool on)
        {
            bool flipped = !on;
            __asm {
                mov esi, this
                mov dl, on
                mov cl, flipped
                mov byte ptr ds : [esi + 0x201B] , dl
                mov byte ptr ds : [esi + 0x221A] , cl
                mov byte ptr ds : [esi + 0x20F4] , cl
            }
        }

		/* --- C_Vehicle Function here :) --- */
		void SetEngineOn(BOOL arg1, BOOL arg2)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::SetEngineOn;
			__asm
			{
				mov ecx, this
				add ecx, 0x70

				push arg2
				push arg1
				call functionAddress
			}
		}

		void SetGear(int gear)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::SetGear;
			__asm
			{
				mov ecx, this
				add ecx, 0x70
				push gear
				call functionAddress
			}
		}

		void GearSnd()
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::GearSnd;
			__asm
			{
				mov ecx, this
				add ecx, 0x70
				call functionAddress
			}
		}

		void Engine(float arg1, float arg2, float arg3)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::Engine;

			__asm
			{
				mov ecx, this
				add ecx, 0x70

				push arg3
				push arg2
				push arg1
				call functionAddress
			}
		}

		unsigned long GetOwner(int seat_idx)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::GetOwner;

			__asm
			{
				push seat_idx
				mov ecx, this
				call functionAddress
			}
		}

		void Prepare_Drop_Out_Wheel(unsigned int wheel_idx, const S_vector & speed, const S_vector & unk)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::PrepareDropOutWheel;

			__asm {
				push unk
				push speed
				mov edx, wheel_idx
				mov ecx, this
				call functionAddress
			}
		}

		void Prepare_Drop_Out(unsigned int comp_idx, const S_vector & speed, const S_vector & unk)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::PrepareDropOut;

			__asm {
				push unk
				push speed
				mov edx, comp_idx
				mov ecx, this
				call functionAddress
			}
		}

		void CarExplosion(unsigned int tick_time)
		{
			unsigned long functionAddress = C_Car_Enum::FunctionsAddresses::CarExplosion;

			__asm {
				push tick_time
				mov ecx, this
				call functionAddress
			}
		}

		/*
		* TODO(DavoSK): Create tyre interface and dont do bullsheet offsets !
		*/
		DWORD* GetCarComponent(unsigned int comp_idx)
		{
            DWORD car_components = *(DWORD *)((DWORD)this + 0x288);
            if (car_components) {
                return (DWORD*)(car_components + 52 * comp_idx);
            }

            return NULL;
		}
		
		DWORD GetCarTyre(unsigned int tyre_idx) 
		{
            DWORD car_tyres = *(DWORD*)((DWORD)this + 0xCA8);
            if (car_tyres) {
                DWORD* ptr_tyre = (DWORD*)(car_tyres + 0x4 * tyre_idx);
                if (ptr_tyre) {
                    return *ptr_tyre;
                }
            }
            
            return NULL;
		}

#define GET_PTR(type,name,addr, err)\
        auto name_ptr = (type*)(addr); \
        if (!name_ptr) { err; }; \
        type* name = *(type**)(name_ptr); \

		void RemoveTyre(unsigned int tyre_idx) 
		{
			auto vehicle_tyre = GetCarTyre(tyre_idx);
			
			if (vehicle_tyre) {
				// Remove tyre frame
				if (vehicle_tyre) {
                    GET_PTR(I3D_Frame, tyre_frame, (DWORD)vehicle_tyre + 0x4, return);
                    if (tyre_frame) {
                        tyre_frame->SetOn(FALSE);
                        tyre_frame->Update();
                    }
				}

				// Mark entity to remove
				DWORD tyre_entity_flags = *(DWORD*)(vehicle_tyre + 0x120);
				tyre_entity_flags |= 0x40000000;
				*(DWORD*)(vehicle_tyre + 0x120) = tyre_entity_flags;
			}
		}

		void RemoveComponent(unsigned int component_idx)
		{
			auto component = this->GetCarComponent(component_idx);
  
			if (component) {
                auto check = *((unsigned __int16 *)component + 6);
                if (check == 2 ||
                    check == 4 ||
                    check == 5 ||
                    check == 6 ||
                    check == 7 ||
                    check == 8) {

                    *(BYTE*)(component + 0x0E) |= 1;
                    I3D_Frame* comp_frame = *(I3D_Frame**)(component);
                    if (comp_frame) {
                        comp_frame->SetOn(FALSE);
                        comp_frame->Update();
                    }
                }
            }
		}

        void EnumerateVehicleMeshes(std::function<void(I3D_mesh_object*)> functionPtr)
        {
            if (functionPtr != nullptr) {
                
                auto inter = &GetInterface()->vehicle_interface;
                DWORD vehicle_frame = *(DWORD*)((DWORD)this + 0x68);

                if (inter && inter->first_mesh && vehicle_frame) {
                    DWORD meshCount = (inter->last_mesh - inter->first_mesh) / I3D_mesh_object_size;

                    for (int i = 0; i < meshCount; i++) {
                       
                        DWORD meshPtr = (I3D_mesh_object_size * i) + inter->first_mesh + 0x4;
                        if (meshPtr) {
                            I3D_mesh_object* currentMesh = *(I3D_mesh_object**)(meshPtr);

                            if (currentMesh)
                                functionPtr(currentMesh);
                        }
                    }
                }
            }
        }

        I3D_mesh_object* GetMeshByIndex(unsigned int idx) 
        {
            auto inter = &GetInterface()->vehicle_interface;

            if (!inter) return nullptr;
            auto mesh_ptr = (I3D_mesh_object**)((I3D_mesh_object_size * idx) + inter->first_mesh + 0x4);

            if (mesh_ptr) {
                if (*mesh_ptr) {
                    return *mesh_ptr;
                }
                else return nullptr;
            }
            else return nullptr;
        }
	};
};

#endif
