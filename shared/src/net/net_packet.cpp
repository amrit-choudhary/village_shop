#include "net_packet.h"

uint8_t* ME::Packet::GetData() { return dataPtr; }

uint16_t ME::Packet::GetSize() { return size; }

uint16_t ME::Packet::GetIndex() { return index; }

void ME::Packet::ClearPacket() {
    for (int i = 0; i < size; ++i) {
        dataPtr[i] = 0;
    }
}

void ME::Packet::WriteByte(uint8_t value) {
    *(dataPtr + index) = value;
    index += 1;
}

uint8_t ME::Packet::ReadByte() {
    uint8_t value;
    value = *(dataPtr + index);
    index += 1;
    return value;
}

ME::PacketSmall::PacketSmall() {
    index = 0;
    size = ME::PACKET_SIZE_SMALL;
    dataPtr = new uint8_t[size]();
}

ME::PacketSmall::~PacketSmall() { delete[] dataPtr; }

ME::PacketMedium::PacketMedium() {
    index = 0;
    size = ME::PACKET_SIZE_MEDIUM;
    dataPtr = new uint8_t[size]();
}

ME::PacketMedium::~PacketMedium() { delete[] dataPtr; }

ME::PacketBig::PacketBig() {
    index = 0;
    size = ME::PACKET_SIZE_BIG;
    dataPtr = new uint8_t[size]();
}

ME::PacketBig::~PacketBig() { delete[] dataPtr; }

ME::PacketHuge::PacketHuge() {
    index = 0;
    size = ME::PACKET_SIZE_HUGE;
    dataPtr = new uint8_t[size]();
}

ME::PacketHuge::~PacketHuge() { delete[] dataPtr; }
