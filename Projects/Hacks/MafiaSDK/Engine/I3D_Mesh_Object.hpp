/*
Copyright 2018 Dávid Svitana
Reversing by Cukier 
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

#ifndef _I3D_Mesh_Object_H_
#define _I3D_Mesh_Object_H_

namespace MafiaSDK
{
    constexpr DWORD I3D_mesh_object_size = 0x34;

    class I3D_mesh_object
    {
    public:																						//  Index Offset
        //virtual int __stdcall AddRef(void);												    // 0000  0000
        virtual int __stdcall Release(void);													// 0001  0004 NOTE(DavoSK): Meybe this is AddRef ? ( most likely)
        virtual class I3D_mesh_level * __stdcall GetLOD(int)const;								// 0002  0008
        virtual enum  LS3D_RESULT __stdcall SetLOD(class I3D_mesh_level * const, int);			// 0003  000C
        virtual enum  LS3D_RESULT __stdcall Duplicate(class I3D_mesh_object * const);			// 0004  0010
        virtual enum  LS3D_RESULT __stdcall GetStats(struct I3D_stats_mesh &)const;				// 0005  0014
        virtual void __stdcall UpdateBoundVolume(void);											// 0006  0018
        virtual enum  LS3D_RESULT __stdcall DetachAllMaterials(void);							// 0007  001C
        virtual bool __stdcall IsCompatible(class I3D_mesh_object * const)const;				// 0008  0020
    };
};

#endif
