#pragma once

#include <Multiplayer/Multiplayer.h>
#include <Multiplayer/ClientManager.h>

class CClientEntity;
class CClientHuman;
class CClientPlayer;
class CClientVehicle;

#define MAFIA_P2P_SERVER_IDS 1

class CMultiplayer : public Galactic3D::CClientNetGame
{
public:
	CMultiplayer(Galactic3D::CClientManager* pClientManager, CVarSystem* pCVars);

	bool m_bRestartingGame;
	bool m_bNametags;
	bool m_bNetworkedEntities;

	std::vector<Weak<CClientEntity>> m_PendingPeerElements;

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

	void EnqueuePeerElement(CClientEntity* pElement);
	bool MigrateEntity(CClientEntity* pElement);
	void ProcessNewPeerElements();

	void SendLocalPlayerShoot(CVector3D position);
	void SendHumanHit(CClientHuman* target, CClientHuman* attacker, CVector3D v1, CVector3D v2, CVector3D v3, int hitType, float damage, unsigned int bodyPart);
	void SendHumanDeath(CClientHuman* target, CClientHuman* attacker);
	void SendHumanDropWeapon(CClientHuman* target);
	void SendHumanChangeWeapon(CClientHuman* target, int8_t weapon);
};