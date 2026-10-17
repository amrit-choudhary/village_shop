/**
 * Server side of The Village Shop. The engine's GameServer handles connects, pings, chat and data; the shared
 * village simulation and its messages (game verbs from GameProtocol::FIRST_GAME_VERB) will live here.
 */
#pragma once

#include <cstdint>

#include "moksha/server/game_server.h"

namespace VS {

class VillageServer : public ME::IGameServerListener {
   public:
    void OnStart(ME::ServerApp& app) override;
    void OnEnd() override;
    void OnClientConnected(uint8_t clientID) override;

    // No village messages exist yet; every game verb is logged and dropped.
    void OnGameMessage(uint8_t clientID, uint8_t verb, ME::ByteReader& reader) override;

   private:
    ME::GameServer* gameServer = nullptr;
};

}  // namespace VS
