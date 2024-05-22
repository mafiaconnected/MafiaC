
#include "stdafx.h"
#include "pch.h"
//#include "m2sdk.h"

tHack* g_pHack;

HACKEVENTRESULT HackMain(uint32_t Event, tHackEventData* pData)
{
	switch (Event)
	{
		case HACKEVENT_LOAD:
			g_pHack = ((tHackEventDataLoad*)pData)->m_pHack;
			return HACKEVENTRESULT_NORMAL;
			break;
		default:
			break;
	}
	return HACKEVENTRESULT_UNREQUESTEVENT;
}
