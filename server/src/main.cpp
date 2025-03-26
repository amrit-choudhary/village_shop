#include <iostream>

#include "net/socket_server.h"

int main(int argc, char** argv) {
    ME::SocketServer socketServer;
    socketServer.Init();

    return 0;
}