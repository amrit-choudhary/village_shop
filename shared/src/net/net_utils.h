/**
 * This has all the helper function for networking.
 */

#pragma once

#include <cstdint>
#include <string>

#include "net_packet.h"
#include "net_protocol.h"

namespace ME::Net {
// Convert an uint8_t to Net::Verb name. Mainly for debug.
std::string GetVerbName(Verb verb);
}  // namespace ME::Net
