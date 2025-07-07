
#include "stdafx.h"
#include "Multiplayer.h"
#include "ClientGame.h"
#include "ClientManager.h"
#include "Elements/Elements.h"
#include <Multiplayer/ChatWindow.h>
#include "Utils/VectorTools.h"

extern Uint32 g_uiSyncedTickCount;

using namespace Galactic3D;

CMultiplayer::CMultiplayer(CClientManager* pClientManager, CVarSystem* pCVars) : CClientNetGame(pClientManager, pCVars)
{
	g_pClientGame->m_pContext->GetSettings()->GetString(NULL, _gstr("Name"), m_LocalPlayerName);

	m_Version.m_uiVersion = NETGAME_CURRENT_VERSION;
	m_Version.m_MajorVersion = LAUNCHER_MAJOR;
	m_Version.m_MinorVersion = LAUNCHER_MINOR;
	m_Version.m_PatchVersion = LAUNCHER_PATCH;

	m_Version.m_Game = pClientManager->m_Games.GetGameId(CHackSupport::m_pInstance->m_InjectedData.m_InjectData.m_LauncherData.m_Game.m_szName);
}

void CMultiplayer::ProcessPacket(uint32_t PacketID, Galactic3D::Stream* pStream)
{
	Galactic3D::CBinaryReader Reader(pStream);

	if (PacketID == PACKET_RESPONSE)
	{
		_glogprintf(_gstr("Multiplayer Process Packet - Response packet received 1"));
		//g_pClientGame->ResetWorld();
		if (g_pClientGame->m_pChatWindow != nullptr)
			g_pClientGame->m_pChatWindow->FlushBuffers();
		g_pClientGame->m_pResourceMgr->ClearAllResources();
	}

	CClientNetGame::ProcessPacket(PacketID, pStream);

	switch (PacketID)
	{
		case PACKET_RESPONSE:
		{
			_glogprintf(_gstr("Multiplayer Process Packet - Response packet received 2"));
			m_bRestartingGame = true;
		}
		break;

		case PACKET_SETPLAYER:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex == INVALID_NETWORK_ID)
			{
				pClient->SetPlayer(nullptr);
				pClient->m_nPlayerObjectId = INVALID_NETWORK_ID;
			}
			else
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					MafiaSDK::C_Game* pGame = MafiaSDK::GetMission()->GetGame();
					pGame->GetCamera()->SetCar(NULL);
					pGame->GetCamera()->SetMode(true, 1);
					pGame->GetCamera()->SetPlayer(pClientPlayer->GetGameHuman());
					pGame->SetLocalPlayer((MafiaSDK::C_Player*)pClientPlayer->GetGameHuman());

					pClient->SetPlayer(pClientPlayer);
					pClient->m_nPlayerObjectId = pClientPlayer->GetId();

					_glogprintf(L"Assigned player %d to the local client", pClientPlayer->GetId());

					m_pClientManager->SetLocalPlayer(pClientPlayer);
				}

				//_glogprintf(_gstr("CanSendSync: %i"), m_pClientManager->m_pLocalPlayer->CanSendSync(m_iLocalIndex));
				//_glogprintf(_gstr("IsLocal: %i"), m_pClientManager->m_pLocalPlayer->IsLocal());
				//_glogprintf(_gstr("IsDirty: %i"), m_pClientManager->m_pLocalPlayer->IsDirty(m_NetMachines.GetMachine(m_iLocalIndex)));
				//_glogprintf(_gstr("IsCreatedFor: %i"), m_pClientManager->m_pLocalPlayer->IsCreatedFor(m_NetMachines.GetMachine(m_iLocalIndex)));
				//_glogprintf(_gstr("m_bSendSync: %i"), m_pClientManager->m_pLocalPlayer->m_bSendSync);
				//_glogprintf(_gstr("m_bIsServer: %i"), m_pClientManager->m_bIsServer);
			}
		}
		break;

		#pragma region Human packets
		case MAFIAPACKET_HUMAN_DIE:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->Kill();
				}
			}
			break;
		}
		break;

		case MAFIAPACKET_HUMAN_THROWGRENADE:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);

			CVector3D vecTargetPos;
			Reader.ReadVector3D(&vecTargetPos, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->ThrowGrenade(vecTargetPos);
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_SHOOT:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);

			bool state = false;
			Reader.ReadBoolean(state);

			CVector3D vecTargetPos;
			Reader.ReadVector3D(&vecTargetPos, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr)
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->Shoot(state, vecTargetPos);
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_RELOAD:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->Reload();
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_HOLSTER:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->HolsterWeapon();
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_JUMP:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->Jump();
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_ADDWEAP:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			unsigned short weapon, ammo1, ammo2;

			Reader.ReadUInt16(&weapon, 1);
			Reader.ReadUInt16(&ammo1, 1);
			Reader.ReadUInt16(&ammo2, 1);

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->GiveWeapon(weapon, ammo1, ammo2);
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_DROPWEAP:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->DropWeapon();
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_CHANGEWEAP:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			int32_t nWeaponId;
			Reader.ReadInt32(&nWeaponId, 1);

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->SetActiveWeapon(nWeaponId);
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_REMWEAP:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);

			unsigned short weaponId;
			Reader.ReadUInt16(&weaponId, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					pClientPlayer->TakeWeapon(weaponId);
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_SETMODEL:
		{
			int32_t nPlayerNetworkIndex;
			Reader.ReadInt32(&nPlayerNetworkIndex, 1);
			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			size_t size = 0;

			GChar* mdl = Reader.ReadString(&size);

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayer* pClientPlayer = static_cast<CClientPlayer*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					_glogprintf(L"Change skin for element #%d:\n\tModel: %s", pClientPlayer->GetId(), mdl);

					pClientPlayer->SetModel(mdl);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_FIX:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->Repair();
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETSIREN:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			bool state = false;
			Reader.ReadBoolean(state);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetSiren(state);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETENGINE:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			bool state = false;
			Reader.ReadBoolean(state);

			bool instant = true;
			Reader.ReadBoolean(instant);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetEngine(state, instant);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETLOCKED:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			bool state = false;
			Reader.ReadBoolean(state);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetLocked(state);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETROOF:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			bool state = false;
			Reader.ReadBoolean(state);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetRoof(state);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETLIGHTS:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			bool state = false;
			Reader.ReadBoolean(state);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetLights(state);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETFUEL:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			float fuel = false;
			Reader.ReadSingle(&fuel, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetFuel(fuel);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETWHEELANGLE:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			float wheelAngle = false;
			Reader.ReadSingle(&wheelAngle, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetWheelAngle(wheelAngle);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETENGINERPM:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			float engineRPM = false;
			Reader.ReadSingle(&engineRPM, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetEngineRPM(engineRPM);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETSPEEDLIMIT:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			float speedLimit = false;
			Reader.ReadSingle(&speedLimit, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetSpeedLimit(speedLimit);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETENGINEHEALTH:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			float engineHealth = false;
			Reader.ReadSingle(&engineHealth, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetEngineHealth(engineHealth);
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_EXPLODE:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->Explode();
				}
			}
		}
		break;

		case MAFIAPACKET_VEHICLE_SETGEAR:
		{
			int32_t nVehicleNetworkIndex;
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);

			uint32_t gear = 0;
			Reader.ReadUInt32(&gear, 1);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetGear(gear);
				}
			}
		}
		break;

		case MAFIAPACKET_EXPLOSION:
		{
			CVector3D pos;
			float radius, force;

			Reader.ReadSingle((float*)&pos, 3);
			Reader.ReadSingle(&radius, 1);
			Reader.ReadSingle(&force, 1);

			MafiaSDK::GetMission()->GetGame()->NewExplosion(nullptr, CVecTools::ConvertToMafiaVec(pos), radius, force, TRUE, TRUE, TRUE, 1);
		}
		break;

		case MAFIAPACKET_CHANGEMAP:
		{
			size_t size = 0;
			GChar* map = Reader.ReadString(&size);

			_glogprintf(_gstr("Recieved map name from server: %s"), CString(false, map).CString());

			UTF8String mapName(false, map);

			//if (!strcmp(MafiaSDK::GetCurrentMissionName(), mapName.CString()))
			//{
			//	_glogprintf(_gstr("Not gonna actually reload the level"));
			//
			//	break;
			//}

			m_bRestartingGame = true;
			g_pClientGame->OnEndInGame();

			MafiaSDK::GetMission()->MapLoad(mapName);
			break;
		}
		break;

		case MAFIAPACKET_GUI_ADDMSG:
		{
			size_t size = 0;

			const GChar* msg = Reader.ReadString(&size);
			uint32_t color;

			Reader.ReadUInt32(&color, 1);

			UTF8String text(true, msg);

			MafiaSDK::GetIndicators()->ConsoleAddText(text, color);
		}
		break;

		case MAFIAPACKET_GUI_FADE:
		{
			uint8_t fadeInOut;
			int time;
			unsigned int color;

			Reader.ReadUInt8(&fadeInOut, 1);
			Reader.ReadInt32(&time, 1);
			Reader.ReadUInt32(&color, 1);

			MafiaSDK::GetIndicators()->FadeInOutScreen(fadeInOut, time, color);
		}
		break;

		case MAFIAPACKET_GUI_ENABLEMAP:
		{
			uint8_t state;
			Reader.ReadUInt8(&state, 1);

			MafiaSDK::GetIndicators()->MapEnable(state);
		}
		break;

		case MAFIAPACKET_GUI_ANNOUNCE:
		{
			size_t size = 0;

			const GChar* msg = Reader.ReadString(&size);

			float time = 0;
			Reader.ReadSingle(&time, 1);

			UTF8String text(true, msg);

			MafiaSDK::GetIndicators()->RaceFlashText(text, time);
		}
		break;

		case MAFIAPACKET_GUI_COUNTDOWN:
		{
			uint8_t flags = 0;

			Reader.ReadUInt8(&flags, 1);

			MafiaSDK::GetIndicators()->RaceSetStartFlag(flags);
		}
		break;

		case MAFIAPACKET_HUMAN_ENTERINGVEHICLE:
		{
			int32_t nHumanNetworkIndex;
			int32_t nVehicleNetworkIndex;
			int8_t nSeatId;
			uint32_t nAction;
			uint32_t nHopSeatsBool;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nHopSeatsBool, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
				{
					CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
					if (pClientHuman != nullptr)
					{
						CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
						if (pClientVehicle != nullptr)
						{
							if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
							{
								if (!pClientHuman->IsSyncer())
								{
									g_pClientGame->HumanEnteringVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nHopSeatsBool);
								}
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_ENTEREDVEHICLE:
		{
			int32_t nHumanNetworkIndex;
			int32_t nVehicleNetworkIndex;
			int8_t nSeatId;
			uint32_t nAction;
			uint32_t nHopSeatsBool;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nHopSeatsBool, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
							if (!pClientHuman->IsSyncer())
							{
								g_pClientGame->HumanEnteredVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nHopSeatsBool);
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_EXITINGVEHICLE:
		{
			int32_t nHumanNetworkIndex;
			int32_t nVehicleNetworkIndex;
			int8_t nSeatId;
			uint32_t nAction;
			uint32_t nHopSeatsBool;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nHopSeatsBool, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
							if (!pClientHuman->IsSyncer())
							{
								g_pClientGame->HumanExitingVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nHopSeatsBool);
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_EXITEDVEHICLE:
		{
			int32_t nHumanNetworkIndex;
			int32_t nVehicleNetworkIndex;
			int8_t nSeatId;
			uint32_t nAction;
			uint32_t nHopSeatsBool;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nHopSeatsBool, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
						if (!pClientHuman->IsSyncer())
						{
							g_pClientGame->HumanExitedVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nHopSeatsBool);
						}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_USINGACTOR:
		{
			int32_t nHumanNetworkIndex;
			uint32_t nUnk1;
			uint32_t nUnk2;
			uint32_t nUnk3;

			size_t size = 0;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			GChar* szName = Reader.ReadString(&size);
			Reader.ReadUInt32(&nUnk1, 1);
			Reader.ReadUInt32(&nUnk2, 1);
			Reader.ReadUInt32(&nUnk3, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					if (!pClientHuman->IsSyncer())
					{
						UTF8String actorName(true, szName);
						g_pClientGame->HumanUsingActor(pClientHuman, MafiaSDK::GetMission()->FindActorByName(actorName), nUnk1, nUnk2, nUnk3);
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_SETHEALTH:
		{
			int32_t nHumanNetworkIndex;
			double fHealth;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			Reader.ReadDouble(&fHealth, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					pClientHuman->SetHealth((float)fHealth);
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_JACKVEHICLE:
		{
			int32_t nHumanNetworkIndex;
			int32_t nVehicleNetworkIndex;
			int8_t nSeatId;

			Reader.ReadInt32(&nHumanNetworkIndex, 1);
			Reader.ReadInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadInt8(&nSeatId, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHuman* pClientHuman = static_cast<CClientHuman*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
							if (!pClientHuman->IsSyncer())
							{
								g_pClientGame->HumanJackVehicle(pClientHuman, pClientVehicle, nSeatId);
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_ELEMENT_UPDATE_ID:
		{
			int64_t nLocalElementId;
			int32_t nServerElementId;

			Reader.ReadInt64(&nLocalElementId, 1);
			Reader.ReadInt32(&nServerElementId, 1);

			if (nServerElementId != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(FromGUID(nLocalElementId));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetId(nServerElementId);
					pClientVehicle->DoAttachments();
				}
			}
		}
		break;

		case MAFIAPACKET_ELEMENT_REMOVE:
		{
			int32_t nServerElementId;

			Reader.ReadInt32(&nServerElementId, 1);

			if (nServerElementId != INVALID_NETWORK_ID)
			{
				CClientVehicle* pClientVehicle = static_cast<CClientVehicle*>(m_pClientManager->FromId(nServerElementId));
				if (pClientVehicle != nullptr)
				{
					if (pClientVehicle->GetGameVehicle() != nullptr)
						pClientVehicle->Despawn();

					m_pClientManager->DestroyObject(pClientVehicle, false, false);
				}
			}
		}
		break;

		default:
		{
			
		}
		break;
	}
}

CNetObject* FromGUID(uint64_t uiGuid)
{
	for (CNetObject* pNetObject : g_pClientGame->m_pClientManager->m_Objects)
	{
		if (uiGuid == pNetObject->GetGUID())
		{
			return pNetObject;
		}
	}

	return nullptr;
}

void CMultiplayer::OnPlayerDisconnect(const Peer_t Peer, unsigned int uiReason)
{
	if (!g_pClientGame->m_bStopMultiplayerGame)
	{
		g_pClientGame->StopMultiplayerGameWhenSafe(uiReason);
		g_pClientGame->ShowDisconnectReason();

		//MafiaSDK::GetIndicators()->ConsoleAddText(msg, 0xFFFF0000);
		//g_pClientGame->m_pChatWindow->AddMessage(_gstr("Disconnected [%s]"), Galactic3D::COLOUR::Red, rgpszReasons[uiReason]);
	}
}

void CMultiplayer::OnFinishConnecting()
{
	tHackEventDataDiscordUpdate Data = {};
	Data.m_Size = sizeof(Data);
	GString str = g_pClientGame->m_szPreviousHost;
	str += _gstr(":");
	str += _gtostring(g_pClientGame->m_usPreviousPort);
	Data.m_pszJoinSecret = str.c_str();
	Data.m_pszPartyId = _gstr("party");
	TriggerHackEvent(HACKEVENT_DISCORD_UPDATE, &Data);

	g_pClientGame->m_pChatWindow->AddInfoMessage(_gstr("Joining..."));

	//MafiaSDK::GetIndicators()->ConsoleAddText("Connected! Joining the game...", 0xFFFFFFFF);

	SendInitial();
}

void CMultiplayer::OnSetGameMode(const GChar* pszGameMode, size_t Length)
{
	CClientNetGame::OnSetGameMode(pszGameMode,Length);
}

void CMultiplayer::OnSetServerName(const GChar* pszName, size_t Length)
{
	CClientNetGame::OnSetServerName(pszName,Length);

	tHackEventDataDiscordState Data;
	Data.m_Size = sizeof(Data);
	Data.m_pszState = pszName;
	TriggerHackEvent(HACKEVENT_SETDISCORDDETAILS, &Data);
}

void CMultiplayer::UpdatePlayerCounts(size_t CurrentPlayers, size_t MaxPlayers)
{
	CClientNetGame::UpdatePlayerCounts(CurrentPlayers, MaxPlayers);

	tHackEventDataDiscordUpdate Data = {};
	Data.m_Size = sizeof(Data);
	Data.m_CurrentPlayers = CurrentPlayers;
	Data.m_MaxPlayers = MaxPlayers;
	TriggerHackEvent(HACKEVENT_DISCORD_UPDATE, &Data);
}

void CMultiplayer::OnPeerJoin(CNetMachine* pPeer)
{
}

void CMultiplayer::OnPeerLeave(CNetMachine* pPeer)
{
}

void CMultiplayer::EnqueuePeerElement(CClientEntity* pElement)
{
	if (pElement->GetId() == INVALID_NETWORK_ID)
		m_PendingPeerElements.push_back(pElement);
}

bool CMultiplayer::MigrateEntity(CClientEntity* pElement)
{
	if (pElement->IsType(ELEMENT_VEHICLE))
	{
		auto pClientVehicle = static_cast<CClientVehicle*>(pElement);
		auto pGameVehicle = pClientVehicle->GetGameVehicle();

		if (pGameVehicle != nullptr)
		{
			Packet Packet(MAFIAPACKET_VEHICLE_CREATE);
			Packet.Write<uint64_t>(pClientVehicle->GetGUID());
			pClientVehicle->WriteCreatePacket(&Packet);
			//pClientVehicle->WriteSyncPacket(&Packet);
			SendHostPacket(&Packet);

			//_glogprintf(_gstr("PEER2PEER: Sending vehicle %d modelIndex %d"), nRef, pVehicle->GetModelIndex());

			return true;
		}
	}
	return false;
}

void CMultiplayer::ProcessNewPeerElements()
{
	for (auto it = m_PendingPeerElements.begin(); it != m_PendingPeerElements.end();)
	{
		auto pElement = (*it).GetPointer();

		if (pElement != nullptr)
		{
			if (MigrateEntity(pElement))
			{
				it = m_PendingPeerElements.erase(it);
				continue;
			}
		}
		else
		{
			it = m_PendingPeerElements.erase(it);
			continue;
		}

		it++;
	}
}

void CMultiplayer::SendLocalPlayerShoot(bool bState, CVector3D position)
{
	//if (pPed->IsLocal() || !pPed->IsSyncer())
	//	return;

	Packet Packet(MAFIAPACKET_HUMAN_SHOOT);
	Packet.Write<int32_t>(m_pClientManager->m_pLocalPlayer->GetId());
	Packet.Write<bool>(bState);
	Packet.Write<CVector3D>(position);
	SendHostPacket(&Packet);
}

void CMultiplayer::SendHumanDeath(CClientHuman* target, CClientEntity* attacker)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_DIE);
	Packet.Write<int32_t>(target->GetId());
	if (attacker != nullptr) {
		Packet.Write<int32_t>(attacker->GetId());
	}
	else 
	{
		Packet.Write<int32_t>(INVALID_NETWORK_ID);
	}
	Packet.Write<int32_t>(INVALID_NETWORK_ID);
	SendHostPacket(&Packet);
}

void CMultiplayer::SendHumanHit(CClientHuman* target, CVector3D v1, CVector3D v2, CVector3D v3, int hitType, float damage, int bodyPart)
{
	if (target == nullptr)
		return;

	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_HIT);
	Packet.Write<int32_t>(target->GetId());
	//Packet.Write<int32_t>(pClientHumanAttacker->GetId());
	Packet.Write<CVector3D>(v1);
	Packet.Write<CVector3D>(v2);
	Packet.Write<CVector3D>(v3);
	Packet.Write<int32_t>(hitType);
	Packet.Write<float>(damage);
	Packet.Write<int32_t>(bodyPart);
	SendHostPacket(&Packet);
}

void CMultiplayer::SendHumanDropWeapon(CClientHuman* target)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_DROPWEAP);
	Packet.Write<int32_t>(target->GetId());
	SendHostPacket(&Packet);
}

void CMultiplayer::SendHumanChangeWeapon(CClientHuman* target, int8_t weapon)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_CHANGEWEAP);
	Packet.Write<int32_t>(target->GetId());
	Packet.Write<int32_t>((int32_t)weapon);
	SendHostPacket(&Packet);
}

void CMultiplayer::SendHumanReloadWeapon(CClientHuman* target)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_RELOAD);
	Packet.Write<int32_t>(target->GetId());
	SendHostPacket(&Packet);
}
