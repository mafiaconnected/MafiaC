#pragma once

struct I3D_vertex_mesh;

class I3D_face_group {
  public:
    uint8_t data[8];
    uint32_t numFaces;
    uint16_t* faces;
};

struct I3D_stats_mesh {
    uint32_t unk;
    uint32_t numVertices;
    uint32_t unk2;
    uint32_t numFaces;
    uint32_t unk4;
};

class I3D_mesh_level {
  public:
    virtual int __stdcall Release();
    virtual void* __stdcall LockVertices(uint32_t flags);
    virtual void __stdcall UnlockVertices();
    virtual LS3D_RESULT __stdcall Duplicate(I3D_mesh_level* other);
    virtual uint32_t __stdcall NumFaces();
    virtual I3D_face_group* __stdcall GetFGroup(int index);
    virtual LS3D_RESULT __stdcall GetStats(I3D_stats_mesh& stats);
    virtual void __stdcall AddFGroup(I3D_face_group* group, bool unk);
    virtual LS3D_RESULT __stdcall SetVertexMesh(const I3D_vertex_mesh* mesh, int unk);
    virtual I3D_vertex_mesh* __stdcall CreateVertexMesh(int unk);
    virtual void __stdcall UpdateBBox();

    uint8_t pad[4];
    uint32_t m_uNumVertices;
    uint8_t pad1[12];
    uint32_t m_uNumFGroups;
    I3D_face_group* m_pFGroups;
    uint8_t pad2[28];
    uint32_t m_uFlags;
};

class I3D_mesh_object {
  public:
    virtual int __stdcall Release();
    virtual I3D_mesh_level* __stdcall GetLOD(int index);
    virtual LS3D_RESULT __stdcall SetLOD(const I3D_mesh_level* lod, int index);
    virtual LS3D_RESULT __stdcall Duplicate(const I3D_mesh_object* other);
    virtual void __stdcall GetStats(I3D_stats_mesh& stats);
    virtual void __stdcall UpdateBoundVolume();
    virtual void __stdcall DetachAllMaterials();
    virtual bool __stdcall IsCompatible(const I3D_mesh_object* other);

    int unk;
    uint32_t m_uNumLODs;
    I3D_mesh_level* m_pLODs[10];
};