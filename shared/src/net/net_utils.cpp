#include "net_utils.h"

#include <map>

#include "net_protocol.h"

using namespace ME::Net;

namespace {
static const std::map<Verb, std::string> verbStringMap = {{Verb::NONE, "None"},
                                                          {Verb::ACK, "Ack"},
                                                          {Verb::AUTH, "Auth"},
                                                          {Verb::PING, "Ping"},
                                                          {Verb::PONG, "Pong"},
                                                          {Verb::DATA, "Data"},
                                                          {Verb::CONNECT, "Connect"},
                                                          {Verb::CONNECTED, "Connected"},
                                                          {Verb::DISCONNECT, "Disconnect"},
                                                          {Verb::GET, "Get"}};
}

std::string ME::Net::GetVerbName(Verb verb) { return verbStringMap.find(verb)->second; }
