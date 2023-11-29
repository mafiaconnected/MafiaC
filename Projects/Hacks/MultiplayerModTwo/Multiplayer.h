#pragma once

#include <Multiplayer/Multiplayer.h>
#include <Multiplayer/ClientManager.h>

class CClientEntityII;
class CClientHumanII;
class CClientPlayerII;
class CClientVehicleII;

#define MAFIA_P2P_SERVER_IDS 1

class CMultiplayerII : public Galactic3D::CClientNetGame
{
public:
	CMultiplayerII(Galactic3D::CClientManager* pClientManager, CVarSystem* pCVars);

	bool m_bRestartingGame;
	bool m_bNametags;
	bool m_bNetworkedEntities;

	std::vector<Weak<CClientEntityII>> m_PendingPeerElements;

	virtual void ProcessPacket(const tPeerInfo& Peer, unsigned int PacketID, Galactic3D::Stream* pStream) override;
	virtual void OnPlayerDisconnect(const Peer_t Peer, unsigned int uiReason) override;
	virtual void OnFinishConnecting(void) override;
#if LIBCAT_SECURITY && GALACTIC_NETGAME_ENCRYPTION
	virtual void OnPong(void) override;
#endif
	virtual void OnSetGameMode(const GChar* pszGameMode, size_t Length) override;
	virtual void OnSetServerName(const GChar* pszName, size_t Length) override;
	virtual void UpdatePlayerCounts(size_t CurrentPlayers, size_t MaxPlayers) override;
	virtual void OnPeerJoin(CNetMachine* pPeer) override;
	virtual void OnPeerLeave(CNetMachine* pPeer) override;

	void EnqueuePeerElement(CClientEntityII* pElement);
	bool MigrateEntity(CClientEntityII* pElement);
	void ProcessNewPeerElements();

	void SendLocalPlayerShoot(bool bState, CVector3D position);
	void SendHumanHit(CClientHumanII* target, CVector3D v1, CVector3D v2, CVector3D v3, int hitType, float damage, unsigned int bodyPart);
	void SendHumanDeath(CClientHumanII* target, CClientHumanII* attacker);
	void SendHumanDropWeapon(CClientHumanII* target);
	void SendHumanChangeWeapon(CClientHumanII* target, int8_t weapon);
};