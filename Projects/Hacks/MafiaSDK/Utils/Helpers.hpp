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

class S_GameItem
{
public:
    int		weaponId;
    int		ammoLoaded;
    int		ammoHidden;
    int		_unk;
};

class G_Inventory
{
public:
    S_GameItem items[8];
};
#endif
