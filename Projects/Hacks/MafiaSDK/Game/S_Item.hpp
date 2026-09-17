/*
    Ported from reMafia's S_Item.h (same author, MafiaOrbitCam/Vendors/reMafia).
    Static per-weapon/item definition data (as opposed to S_GameItem in Utils/Helpers.hpp,
    which is a live inventory *slot* - weaponId/ammoLoaded/ammoHidden - not this definition).
    No addresses known for where the item definition table itself lives.
*/

#ifndef _SITEM_H_
#define _SITEM_H_

namespace MafiaSDK
{
    enum E_item_flags
    {
        ITEM_IS_LOCKED    = 0x1,
        ITEM_IS_GUN       = 0x2,
        ITEM_IS_MELEE     = 0x4,
        ITEM_IS_BIG       = 0x20,
        ITEM_IS_SNIPER    = 0x40,
        ITEM_IS_EXPLOSIVE = 0xA0,
    };

    enum E_item_setting
    {
        IS_RejectCartridgeTime,
        IS_ReloadPause,
        IS_LoadCartridgeTime,
        IS_ByteSkip,
        IS_ShootDelay
    };

    struct S_item_property
    {
        E_item_setting type;
        int* value;
    };

    struct S_Item
    {
        char internalName[32];
        E_item_flags flags;
        char modelName[32];
        PADDING(S_Item, _pad0, 0x8);
        int reloadAnimID;
        PADDING(S_Item, _pad1, 0x4);
        int shootSoundID;
        int reloadSoundID;
        int itemType;
        int magCapacity;
        int totalAmmo;
        PADDING(S_Item, _pad2, 0xC);
        float rangeOfFire;
        float damage;
        PADDING(S_Item, _pad3, 0x8);
        float recoil;
        float accuracy;
        unsigned char properties[48];
    };
};

#endif
