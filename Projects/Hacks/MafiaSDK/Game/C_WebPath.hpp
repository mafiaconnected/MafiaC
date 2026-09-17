/*
    Ported from reMafia's C_web_path.h/.cpp (same author, MafiaOrbitCam/Vendors/reMafia).
    The pedestrian/AI "web" navigation graph.
*/

#ifndef _CWEBPATH_H_
#define _CWEBPATH_H_

namespace MafiaSDK
{
    enum E_web_node_type : unsigned short
    {
        WPT_Pedestrian = 0x1,
        WPT_AI = 0x2,
        WPT_Traffic = 0x4,
        WPT_TramStation = 0x8,
        WPT_Special = 0x10,
        WPT_NadzemkaOnboard = 0x201,
        WPT_NadzemkaWaypoint = 0x1004,
        WPT_NadzemkaStop = 0x1008,
        WPT_TramUnknown = 0x1804,
        WPT_AIUnknown = 0x8002
    };

    enum E_web_connection_type : unsigned short
    {
        WCT_Pedestrian = 0x1,
        WCT_AI = 0x2,
        WCT_TrafficForward = 0x4,
        WCT_Nadzemka = 0x84,
        WCT_TrafficBackward = 0x8400,
        WCT_Other = 0x1000
    };

    inline const char* GetWebNodeTypeName(E_web_node_type type)
    {
        switch (type)
        {
        case WPT_Pedestrian: return "Pedestrian";
        case WPT_AI: return "AI";
        case WPT_Traffic: return "Traffic";
        case WPT_TramStation: return "TramStation";
        case WPT_Special: return "Special";
        case WPT_NadzemkaOnboard: return "Train Onboard";
        case WPT_NadzemkaWaypoint: return "Train Waypoint";
        case WPT_NadzemkaStop: return "Train Stop";
        case WPT_TramUnknown: return "Tram Unknown";
        case WPT_AIUnknown: return "AI Unknown";
        default: return "Unknown";
        }
    }

    inline S_vector GetWebNodeTypeColor(E_web_node_type type)
    {
        switch (type)
        {
        case WPT_Pedestrian: return S_vector(0, 0, 1);
        case WPT_AI: return S_vector(0, 1, 0);
        case WPT_Traffic: return S_vector(1, 1, 0);
        case WPT_TramStation: return S_vector(1, 0, 0);
        case WPT_Special: return S_vector(0.59f, 0, 1);
        case WPT_NadzemkaOnboard: return S_vector(1, 0.5f, 0.5f);
        case WPT_NadzemkaWaypoint: return S_vector(0.75f, 0.75f, 0);
        case WPT_NadzemkaStop: return S_vector(0.65f, 0, 0);
        case WPT_TramUnknown: return S_vector(0.85f, 0.25f, 0.25f);
        case WPT_AIUnknown: return S_vector(0.75f, 0, 0);
        default: return S_vector(0.65f, 0.65f, 0.65f);
        }
    }

    inline const char* GetWebConnTypeName(E_web_connection_type type)
    {
        switch (type)
        {
        case WCT_Pedestrian: return "Pedestrian";
        case WCT_AI: return "AI";
        case WCT_TrafficForward: return "Traffic Forward";
        case WCT_TrafficBackward: return "Traffic Backward";
        case WCT_Nadzemka: return "Train";
        case WCT_Other: return "Other";
        default: return "Unknown";
        }
    }

    inline S_vector GetWebConnTypeColor(E_web_connection_type type)
    {
        switch (type)
        {
        case WCT_Pedestrian: return S_vector(0, 0, 1);
        case WCT_AI: return S_vector(0, 1, 0);
        case WCT_TrafficForward: return S_vector(1, 1, 0);
        case WCT_TrafficBackward: return S_vector(0, 1, 1);
        case WCT_Nadzemka: return S_vector(0.75f, 0.75f, 0);
        case WCT_Other: return S_vector(0.59f, 0, 1);
        default: return S_vector(0.45f, 0.45f, 0.45f);
        }
    }

    struct C_WebConnection
    {
        unsigned short endPoint;
        E_web_connection_type type;
        float length;
    };

    struct C_WebNode
    {
        S_vector position;
        E_web_node_type type;
        unsigned short id;
        unsigned long radius;
        int unk;
        C_WebConnection* links;
        unsigned char unk2;
        unsigned char numLinks;
    };

    class C_WebPath
    {
    public:
        void* vtable;
        C_WebNode* nodes;
        int unk;
        int numPoints;
        PADDING(C_WebPath, _pad0, 0xC);
    };
};

#endif
