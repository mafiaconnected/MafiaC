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

#ifndef _I3D_Mesh_Level_H_
#define _I3D_Mesh_Level_H_

namespace MafiaSDK
{
    struct I3D_vertex_mesh
    {
        S_vector p;
        S_vector n;
        S_vector2 uv;
    };

    struct I3D_stats_mesh
    {
        DWORD unk;
        DWORD vertex_count;
        DWORD vertex_buffer_size;	//vertex_count * 0x20
        DWORD faces_count;
        DWORD unk2;					//faces_count * 6
    };

    class I3D_mesh_level
    {
    public:																						//  Index Offset
        virtual int __stdcall	AddRef(void);													// 0000  0000
        virtual struct I3D_vertex_mesh * __stdcall LockVertices(unsigned int);					// 0001  0004
        virtual void __stdcall UnlockVertices(void);											// 0002  0008
        virtual enum  LS3D_RESULT __stdcall Duplicate(class I3D_mesh_level * const);			// 0003  000C
        virtual unsigned int __stdcall NumFaces(void)const;										// 0004  0010
        virtual class I3D_face_group * __stdcall GetFGroup(int);								// 0005  0014
        virtual enum  LS3D_RESULT __stdcall GetStats(struct I3D_stats_mesh &)const;				// 0006  0018
        virtual void __stdcall AddFGroup(class I3D_face_group * const, bool);					// 0007  001C
        virtual enum  LS3D_RESULT __stdcall SetVertexMesh(struct I3D_vertex_mesh * const, int);	// 0008  0020
        virtual struct I3D_vertex_mesh * __stdcallCreateVertexMesh(int);						// 0009  0024
        virtual void __stdcall UpdateBBox(void);												// 0010  0028
    };
};

#endif
