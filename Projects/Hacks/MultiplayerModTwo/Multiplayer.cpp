
#include "stdafx.h"
#include "Multiplayer.h"
#include "ClientGame.h"
#include "ClientManager.h"
#include "Elements/Elements.h"
#include <Multiplayer/ChatWindow.h>
#include "Utils/VectorTools.h"

extern Uint32 g_uiSyncedTickCountII;

using namespace Galactic3D;

CMultiplayerII::CMultiplayerII(CClientManager* pClientManager, CVarSystem* pCVars) : CClientNetGame(pClientManager, pCVars)
{
	m_bRestartingGame = false;
	m_bNametags = true;
	m_bNetworkedEntities = true;

	g_pClientGame->m_pContext->GetSettings()->GetString(NULL, _gstr("Name"), m_LocalPlayerName);

	m_Version.m_uiVersion = NETGAME_CURRENT_VERSION;
	m_Version.m_MajorVersion = LAUNCHER_MAJOR;
	m_Version.m_MinorVersion = LAUNCHER_MINOR;
	m_Version.m_PatchVersion = LAUNCHER_PATCH;

	m_Version.m_Game = pClientManager->m_Games.GetGameId(CHackSupport::m_pInstance->m_InjectedData.m_InjectData.m_LauncherData.m_Game.m_szName);
	m_Version.m_GameVersion = 1;
}

void CMultiplayerII::ProcessPacket(const tPeerInfo& Peer, unsigned int PacketID, Galactic3D::Stream* pStream)
{
	Galactic3D::CBinaryReader Reader(pStream);

	if (PacketID == PACKET_RESPONSE)
	{
		_glogprintf(_gstr("Multiplayer Process Packet - Response packet received 1"));
		//g_pClientGame->ResetWorld();
		g_pClientGame->m_pChatWindow->FlushBuffers();
		g_pClientGame->m_pResourceMgr->ClearAllResources();
	}
	CClientNetGame::ProcessPacket(Peer, PacketID, pStream); // this is asking for problems, MOVE IT TO DEFAULT CASE IDIOT

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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					// Set player with M2 SDK

					pClient->SetPlayer(pClientPlayer);
					pClient->m_nPlayerObjectId = pClientPlayer->GetId();

					_glogprintf(L"Assigned player %d to the local client", pClientPlayer->GetId());

					m_pClientManager->SetLocalPlayer(pClientPlayer);
				}
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
			Reader.ReadSingle((float*)&vecTargetPos, 3);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
			Reader.ReadSingle((float*)&vecTargetPos, 3);

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr)
				return;

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
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

			int32_t model;
			Reader.ReadInt32(&model, 1);

			if (nPlayerNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientPlayerII* pClientPlayer = static_cast<CClientPlayerII*>(m_pClientManager->FromId(nPlayerNetworkIndex, ELEMENT_PLAYER));
				if (pClientPlayer != nullptr)
				{
					_glogprintf(L"Change skin for element #%d:\n\tModel: %i", pClientPlayer->GetId(), model);

					pClientPlayer->SetModel(model);
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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

			auto pClient = m_NetMachines.GetMachine(m_iLocalIndex);
			if (pClient == nullptr) // We didn't receive that client yet
				return;

			if (nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->SetEngine(state);
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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
				CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
				if (pClientVehicle != nullptr)
				{
					pClientVehicle->Explode();
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

			// Create explosion
		}
		break;

		case MAFIAPACKET_CHANGEMAP:
		{
			size_t size = 0;
			GChar* map = Reader.ReadString(&size);

			_glogprintf(_gstr("Recieved map name from server: %s"), map);

			UTF8String mapName(true, map);

			m_bRestartingGame = true;
			g_pClientGame->OnEndInGame();

			// Set map with game sdk
			break;
		}
		break;

		case MAFIAPACKET_HUMAN_ENTERINGVEHICLE:
		{
			uint32_t nHumanNetworkIndex;
			uint32_t nVehicleNetworkIndex;
			uint8_t nSeatId;
			uint32_t nAction;
			uint32_t nUnknown;

			Reader.ReadUInt32(&nHumanNetworkIndex, 1);
			Reader.ReadUInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadUInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nUnknown, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
				{
					CClientHumanII* pClientHuman = static_cast<CClientHumanII*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
					if (pClientHuman != nullptr)
					{
						CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
						if (pClientVehicle != nullptr)
						{
							if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
							{
								if (!pClientHuman->IsSyncer())
								{
									g_pClientGame->HumanEnteringVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nUnknown);
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
			uint32_t nHumanNetworkIndex;
			uint32_t nVehicleNetworkIndex;
			uint8_t nSeatId;
			uint32_t nAction;
			uint32_t nUnknown;

			Reader.ReadUInt32(&nHumanNetworkIndex, 1);
			Reader.ReadUInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadUInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nUnknown, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHumanII* pClientHuman = static_cast<CClientHumanII*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr) 
				{
					CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
							if (!pClientHuman->IsSyncer())
							{
								g_pClientGame->HumanEnteredVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nUnknown);
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_EXITINGVEHICLE:
		{
			uint32_t nHumanNetworkIndex;
			uint32_t nVehicleNetworkIndex;
			uint8_t nSeatId;
			uint32_t nAction;
			uint32_t nUnknown;

			Reader.ReadUInt32(&nHumanNetworkIndex, 1);
			Reader.ReadUInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadUInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nUnknown, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHumanII* pClientHuman = static_cast<CClientHumanII*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
							if (!pClientHuman->IsSyncer())
							{
								g_pClientGame->HumanExitingVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nUnknown);
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_EXITEDVEHICLE:
		{
			uint32_t nHumanNetworkIndex;
			uint32_t nVehicleNetworkIndex;
			uint8_t nSeatId;
			uint32_t nAction;
			uint32_t nUnknown;

			Reader.ReadUInt32(&nHumanNetworkIndex, 1);
			Reader.ReadUInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadUInt8(&nSeatId, 1);
			Reader.ReadUInt32(&nAction, 1);
			Reader.ReadUInt32(&nUnknown, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHumanII* pClientHuman = static_cast<CClientHumanII*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
					if (pClientVehicle != nullptr)
					{
						if (m_pClientManager->m_pLocalPlayer.GetPointer() != pClientHuman)
						{
							if (!pClientHuman->IsSyncer())
							{
								g_pClientGame->HumanExitedVehicle(pClientHuman, pClientVehicle, nSeatId, nAction, nUnknown);
							}
						}
					}
				}
			}
		}
		break;

		case MAFIAPACKET_HUMAN_SETHEALTH:
		{
			uint32_t nHumanNetworkIndex;
			double fHealth;

			Reader.ReadUInt32(&nHumanNetworkIndex, 1);
			Reader.ReadDouble(&fHealth, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHumanII* pClientHuman = static_cast<CClientHumanII*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					pClientHuman->SetHealth((float)fHealth);
				}
			}
		}
		break;
		
		case MAFIAPACKET_HUMAN_JACKVEHICLE:
		{
			uint32_t nHumanNetworkIndex;
			uint32_t nVehicleNetworkIndex;
			uint8_t nSeatId;

			Reader.ReadUInt32(&nHumanNetworkIndex, 1);
			Reader.ReadUInt32(&nVehicleNetworkIndex, 1);
			Reader.ReadUInt8(&nSeatId, 1);

			if (nHumanNetworkIndex != INVALID_NETWORK_ID && nVehicleNetworkIndex != INVALID_NETWORK_ID)
			{
				CClientHumanII* pClientHuman = static_cast<CClientHumanII*>(m_pClientManager->FromId(nHumanNetworkIndex, ELEMENT_PLAYER));
				if (pClientHuman != nullptr)
				{
					CClientVehicleII* pClientVehicle = static_cast<CClientVehicleII*>(m_pClientManager->FromId(nVehicleNetworkIndex, ELEMENT_VEHICLE));
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

		default:
		{

		}
		break;
	}
}

void CMultiplayerII::OnPlayerDisconnect(const Peer_t Peer, unsigned int uiReason)
{
	if (!g_pClientGame->m_bStopMultiplayerGame)
	{
		const GChar* rgpszReasons[] = {
			_gstr("TIMEOUT"),
			_gstr("FULL"),
			_gstr("UNSUPPORTED CLIENT"),
			_gstr("UNSUPPORTED ENGINE"),
			_gstr("WRONG PASSWORD"),
			_gstr("UNSUPPORTED EXECUTABLE"),
			_gstr("GRACEFUL"),
			_gstr("BANNED"),
			_gstr("FAILED"),
			_gstr("INVALID NAME"),
			_gstr("CRASH"),
			_gstr("PUBLIC KEY MISMATCH"),
			_gstr("NAME IN USE"),
			_gstr("KICKED")
		};
		g_pClientGame->StopMultiplayerGameWhenSafe(uiReason);

		char msg[128];
		sprintf(msg, "Disconnected [%ws]", rgpszReasons[uiReason]);

		g_pClientGame->m_pChatWindow->AddMessage(_gstr("Disconnected [%s]"), Galactic3D::COLOUR::Red, rgpszReasons[uiReason]);
	}
}

void CMultiplayerII::OnFinishConnecting(void)
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

	SendInitial();
}

void CMultiplayerII::OnSetGameMode(const GChar* pszGameMode, size_t Length)
{
	CClientNetGame::OnSetGameMode(pszGameMode,Length);
}

void CMultiplayerII::OnSetServerName(const GChar* pszName, size_t Length)
{
	CClientNetGame::OnSetServerName(pszName,Length);

	tHackEventDataDiscordState Data;
	Data.m_Size = sizeof(Data);
	Data.m_pszState = pszName;
	TriggerHackEvent(HACKEVENT_SETDISCORDDETAILS, &Data);
}

void CMultiplayerII::UpdatePlayerCounts(size_t CurrentPlayers, size_t MaxPlayers)
{
	CClientNetGame::UpdatePlayerCounts(CurrentPlayers, MaxPlayers);

	tHackEventDataDiscordUpdate Data = {};
	Data.m_Size = sizeof(Data);
	Data.m_CurrentPlayers = CurrentPlayers;
	Data.m_MaxPlayers = MaxPlayers;
	TriggerHackEvent(HACKEVENT_DISCORD_UPDATE, &Data);
}

void CMultiplayerII::OnPeerJoin(CNetMachine* pPeer)
{
}

void CMultiplayerII::OnPeerLeave(CNetMachine* pPeer)
{
}

void CMultiplayerII::EnqueuePeerElement(CClientEntityII* pElement)
{
	if (pElement->GetId() == INVALID_NETWORK_ID)
		m_PendingPeerElements.push_back(pElement);
}

bool CMultiplayerII::MigrateEntity(CClientEntityII* pElement)
{
	return false;
}

void CMultiplayerII::ProcessNewPeerElements(void)
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

void CMultiplayerII::SendLocalPlayerShoot(bool bState, CVector3D position)
{
	//if (pPed->IsLocal() || !pPed->IsSyncer())
	//	return;

	Packet Packet(MAFIAPACKET_HUMAN_SHOOT);
	Packet.Write<int32_t>(m_pClientManager->m_pLocalPlayer->GetId());
	Packet.Write<bool>(bState);
	Packet.Write<CVector3D>(position);
	SendHostPacket(&Packet);
}

void CMultiplayerII::SendHumanHit(CClientHumanII* target, CClientHumanII* attacker, CVector3D v1, CVector3D v2, CVector3D v3, int hitType, float damage, unsigned int bodyPart)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_HIT);
	//Packet.Write<int32_t>(m_pClientManager->m_pLocalPlayer->GetId());
	Packet.Write<int32_t>(target->GetId());
	Packet.Write<int32_t>(attacker->GetId());
	Packet.Write<CVector3D>(v1);
	Packet.Write<CVector3D>(v2);
	Packet.Write<CVector3D>(v3);
	Packet.Write<int32_t>(hitType);
	Packet.Write<float>(damage);
	Packet.Write<int32_t>(bodyPart);
	SendHostPacket(&Packet);
}

void CMultiplayerII::SendHumanDeath(CClientHumanII* target, CClientHumanII* attacker)
{
	if (target->IsLocal() || attacker->IsLocal())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_DIE);
	Packet.Write<int32_t>(target->GetId());
	Packet.Write<int32_t>(attacker->GetId());
	SendHostPacket(&Packet);
}

void CMultiplayerII::SendHumanDropWeapon(CClientHumanII* target)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_DROPWEAP);
	Packet.Write<int32_t>(target->GetId());
	SendHostPacket(&Packet);
}

void CMultiplayerII::SendHumanChangeWeapon(CClientHumanII* target, int8_t weapon)
{
	if (target->IsLocal() || !target->IsSyncer())
		return;

	Packet Packet(MAFIAPACKET_HUMAN_CHANGEWEAP);
	Packet.Write<int32_t>(target->GetId());
	Packet.Write<int32_t>((int32_t)weapon);
	SendHostPacket(&Packet);
}
