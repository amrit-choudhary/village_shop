/**
 * VillageShop_Server: the engine's game server running the village's listener.
 */

#include "moksha/server/server_app.h"
#include "server/village_server.h"

int main(int argc, char** argv) {
    VS::VillageServer listener;
    ME::ServerApp app;
    return app.Run(argc, argv, listener);
}
