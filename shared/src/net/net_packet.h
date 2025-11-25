/**
 * Packet for trasfer between client and server.
 */

#pragma once

#include <cstdint>

#include "../math/fp_24_8.h"

namespace ME {

constexpr uint16_t PACKET_SIZE_SMALL = 64;
constexpr uint16_t PACKET_SIZE_MEDIUM = 256;
constexpr uint16_t PACKET_SIZE_BIG = 1024;
constexpr uint16_t PACKET_SIZE_HUGE = 2048;

/**
 * Base class of all packets.
 * Derived from this are 4 sizes of packets.
 */
class Packet {
   public:
    uint8_t* GetData();
    uint16_t GetSize();
    uint16_t GetIndex();
    void ClearPacket();

    // Write data in the packet.
    void WriteByte(uint8_t value);
    void WriteString(const char* value);
    void WriteFP(const ME::FP_24_8& value);
    // Read data from the packet.
    uint8_t ReadByte();
    void ReadString(char* ptrString);
    ME::FP_24_8 ReadFP();

   protected:
    uint16_t index;
    uint16_t size;
    uint8_t* dataPtr;
};

// 64 byte packet.
class PacketSmall : public Packet {
   public:
    PacketSmall();
    ~PacketSmall();

   private:
};

// 256 byte packet.
class PacketMedium : public Packet {
   public:
    PacketMedium();
    ~PacketMedium();

   private:
};

// 1024 byte packet.
class PacketBig : public Packet {
   public:
    PacketBig();
    ~PacketBig();

   private:
};

// 2048 byte packet.
class PacketHuge : public Packet {
   public:
    PacketHuge();
    ~PacketHuge();

   private:
};
}  // namespace ME
