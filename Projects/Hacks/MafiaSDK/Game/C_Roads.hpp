/*
    Ported from reMafia's C_roads.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    The traffic AI's road-network graph (crosspoints/waypoints/lanes).
*/

#ifndef _CROADS_H_
#define _CROADS_H_

namespace MafiaSDK
{
    struct S_Roads_Lane
    {
        unsigned short type;
        float lineWidth;
        unsigned short pad0;
    };

    struct S_Roads_DirectionLink
    {
        short crosspointLink;
        unsigned short pad0;
        float distance;
        float angle;
        unsigned short priority1;
        unsigned short priority2;
        unsigned short info;
        S_Roads_Lane lanes[4];
    };

    struct S_Roads_Crosspoint
    {
        S_vector pos;
        bool semaphore;
        PADDING(S_Roads_Crosspoint, _pad0, 0x3);
        float speed;
        short waypointLinks[4];
        S_Roads_DirectionLink directionLinks[4];
    };

    struct S_Roads_Waypoint
    {
        S_vector pos;
        float speed;
        short prevWaypoint;
        short nextWaypoint;
        short prevCrosspoint;
        short nextCrosspoint;
    };

    namespace C_Roads_Enum
    {
        enum FunctionAddresses
        {
            SortNodesByX = 0x0055C890,
            SaveData = 0x00559D40,
            LoadData = 0x00559E60,
            RecompileDistances = 0x0055B370,
            // NOTE: reMafia gives GetNearNode the same address (0x55B370) as RecompileDistances
            // above - almost certainly a copy/paste mistake in that source, not verified here.
            GetNearNode = 0x0055B370,
            FindCrossing = 0x0055B570
        };
    };

    class C_Roads
    {
    public:
        void SortNodesByX()
        {
            unsigned long funcAddress = C_Roads_Enum::FunctionAddresses::SortNodesByX;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        bool SaveData(C_Chunk* chunk)
        {
            unsigned long funcAddress = C_Roads_Enum::FunctionAddresses::SaveData;
            bool result = false;

            __asm
            {
                push chunk
                mov ecx, this
                call funcAddress
                mov result, al
            }

            return result;
        }

        bool LoadData(C_Chunk* chunk)
        {
            unsigned long funcAddress = C_Roads_Enum::FunctionAddresses::LoadData;
            bool result = false;

            __asm
            {
                push chunk
                mov ecx, this
                call funcAddress
                mov result, al
            }

            return result;
        }

        void RecompileDistances()
        {
            unsigned long funcAddress = C_Roads_Enum::FunctionAddresses::RecompileDistances;

            __asm
            {
                mov ecx, this
                call funcAddress
            }
        }

        short GetNearNode(const S_vector& pos)
        {
            unsigned long funcAddress = C_Roads_Enum::FunctionAddresses::GetNearNode;
            short result = -1;

            __asm
            {
                push pos
                mov ecx, this
                call funcAddress
                mov result, ax
            }

            return result;
        }

        short FindCrossing(short firstId, short secondId)
        {
            unsigned long funcAddress = C_Roads_Enum::FunctionAddresses::FindCrossing;
            short result = -1;

            __asm
            {
                push secondId
                push firstId
                mov ecx, this
                call funcAddress
                mov result, ax
            }

            return result;
        }

        // reMafia declares SortNodesByX/SaveData/LoadData/RecompileDistances as virtual on the
        // real class, meaning there's an implicit vtable pointer at offset 0 that its own
        // (naive, offset-0) field declarations don't account for - this class calls them by
        // fixed address instead (fine, since nothing here needs vtable dispatch), but the data
        // members below still need the 4-byte pad to land at their real offsets.
        PADDING(C_Roads, _pad0, 0x4);
        vc6_vector<S_Roads_Crosspoint> crosspoints;
        vc6_vector<S_Roads_Waypoint> waypoints;
    };
};

#endif
