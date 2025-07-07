#pragma once

#include <Multiplayer/Multiplayer.h>
#include <Multiplayer/ClientManager.h>

class CClientEntity;
class CClientHuman;
class CClientPlayer;
class CClientVehicle;
class CClientDummy;

#define MAFIA_P2P_SERVER_IDS 1

CNetObject* FromGUID(uint64_t uiGuid);

class CMultiplayer : public Galactic3D::CClientNetGame
{
public:
	CMultiplayer(Galactic3D::CClientManager* pClientManager, CVarSystem* pCVars);

	bool m_bRestartingGame = false;
	bool m_bNametags = true;
	bool m_bNetworkedEntities = true;

	std::vector<Weak<CClientEntity>> m_PendingPeerElements;

	virtual void ProcessPacket(uint32_t PacketID, Galactic3D::Stream* pStream) override;
	virtual void OnPlayerDisconnect(const Peer_t Peer, unsigned int uiReason) override;
	virtual void OnFinishConnecting() override;
	virtual void OnSetGameMode(const GChar* pszGameMode, size_t Length) override;
	virtual void OnSetServerName(const GChar* pszName, size_t Length) override;
	virtual void UpdatePlayerCounts(size_t CurrentPlayers, size_t MaxPlayers) override;
	virtual void OnPeerJoin(CNetMachine* pPeer) override;
	virtual void OnPeerLeave(CNetMachine* pPeer) override;

	void EnqueuePeerElement(CClientEntity* pElement);
	bool MigrateEntity(CClientEntity* pElement);
	void ProcessNewPeerElements();

	void SendLocalPlayerShoot(bool bState, CVector3D position);
	void SendHumanHit(CClientHuman* target, CVector3D v1, CVector3D v2, CVector3D v3, int hitType, float damage, int bodyPart);
	void SendHumanDeath(CClientHuman* target, CClientEntity* attacker);
	void SendHumanDropWeapon(CClientHuman* target);
	void SendHumanChangeWeapon(CClientHuman* target, int8_t weapon);
	void SendHumanReloadWeapon(CClientHuman* target);
};