#ifndef _SHAREDUTILS_DEFINES_H_
#define _SHAREDUTILS_DEFINES_H_

// Macros
#define PADDING(prefix, name, size)	  BYTE prefix##_##name##[size]

#ifdef WIN32
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#endif

// Constants
#define C_MISSION_CLASS 0x063788C
#define LS3D_Alloc 0x100EF57C
#define D3D_DEVICE_PTR 0x101C597C
#define D3D_CAMERA_PTR 0x101C4CF8
#define D3D_VIEWPORT_PTR 0x101C5590

class S_GameItem
{
public:
    int		weaponId;
    int		ammoLoaded;
    int		ammoHidden;
    int		_unk;
};

/*
    Ported from reMafia's G_Inventory.h (same author, MafiaOrbitCam/Vendors/reMafia), which
    also explains a previously-unnamed offset: C_Human's "active weapon id" write at
    this+1184 (see CClientHuman::SetActiveWeapon in MultiplayerModOne) is
    this+0x480(inventory)+32 = selectedItem.weaponId, exactly - not an arbitrary flat-array
    slot like the old 8-element model implied.
*/
class G_Inventory
{
public:
    PADDING(G_Inventory, _pad0, 0x8);
    int numItems;											// 8-12
    PADDING(G_Inventory, _pad1, 0x14);
    S_GameItem selectedItem;								// 32-48 (this+0x480+32 - see note above)
    S_GameItem smallItems[4];								// 48-112
    S_GameItem unkWeapon1;									// 112-128
    S_GameItem coatWeapon;									// 128-144
    S_GameItem unkItems[5];									// 144-224
    int unk;												// 224-228
};

// Lightweight array-view template the game uses in a few places (e.g. weapon/ammo tables).
// Ported from reMafia's S_Collection.h (same author, MafiaOrbitCam/Vendors/reMafia).
template<class T>
class S_Collection
{
public:
    int NumElements()
    {
        unsigned long funcAddress = 0x0047BBB0;
        int result = 0;

        __asm
        {
            mov ecx, this
            call funcAddress
            mov result, eax
        }

        return result;
    }

    T& GetElement(int index)
    {
        unsigned long funcAddress = 0x0047BE10;

        __asm
        {
            push index
            mov ecx, this
            call funcAddress
        }
    }

    T& operator[](int index)
    {
        return GetElement(index);
    }

private:
    int unk;
    T* start;
    T* end;
};
#endif
