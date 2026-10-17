#include "village_server.h"

#include "moksha/logging/logging.h"
#include "moksha/server/server_app.h"

void VS::VillageServer::OnStart(ME::ServerApp& app) {
    gameServer = &app.GetGameServer();
    ME::LogInfo("Village server started");
}

void VS::VillageServer::OnEnd() {
    gameServer = nullptr;
}

void VS::VillageServer::OnClientConnected(uint8_t clientID) {
    ME::LogInfo("Villager joined: client ", static_cast<unsigned>(clientID));
}

void VS::VillageServer::OnGameMessage(uint8_t clientID, uint8_t verb, ME::ByteReader& /*reader*/) {
    ME::LogWarning("Dropped unknown game verb ", static_cast<unsigned>(verb), " from client ",
                   static_cast<unsigned>(clientID));
}
