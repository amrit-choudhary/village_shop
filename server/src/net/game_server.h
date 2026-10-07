/**
 * UDP game server: receives datagrams from clients, keeps the client list, relays chat / data / scores.
 * Wire format in shared/src/net/game_protocol.h; socket in shared/src/net/udp_socket.h.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "server/src/db/score_db.h"
#include "shared/src/net/net_address.h"
#include "shared/src/net/udp_socket.h"
#include "shared/src/time/timer_manager.h"

namespace ME {

class ByteReader;

/**
 * One client the server has accepted. clientID is assigned on CONNECT and sent back in every datagram.
 */
class ConnectedClient {
   public:
    uint8_t clientID = 0;
    Net::Address address;
};

class GameServer {
   public:
    /**
     * Opens the UDP port and the score database. False if the port could not be opened.
     */
    bool Init(uint16_t port);

    /**
     * Handles every datagram that has arrived since the last call.
     */
    void Update(double deltaTime);
    void End();

    // Timers the server schedules on; owned and ticked by the server main loop.
    void SetTimerManagerRef(Time::TimerManager* ptrTimerManager);

   private:
    void ProcessDatagram(const uint8_t* data, int size, const Net::Address& from);

    void HandleConnect(const Net::Address& from);
    void HandleChat(ByteReader& reader, uint8_t clientID);
    void HandleData(ByteReader& reader, uint8_t clientID);
    void HandleScore(ByteReader& reader, uint8_t clientID);

    void SendConnected(uint8_t clientID);
    void SendPong(uint8_t clientID);
    void SendHighScore(uint8_t clientID);

    void SendDatagram(const uint8_t* data, size_t size, uint8_t clientID);

    /**
     * Sends to every connected client except exceptClientID.
     */
    void SendToOthers(const uint8_t* data, size_t size, uint8_t exceptClientID);

    // Upper bound per Update, so a flood of datagrams can't hold up the server's tick.
    static constexpr int MAX_DATAGRAMS_PER_UPDATE = 256;

    Net::UdpSocket socket;
    std::vector<ConnectedClient> connectedClients;
    ScoreDB scoreDB;
    Time::TimerManager* timerManager = nullptr;
};

}  // namespace ME
