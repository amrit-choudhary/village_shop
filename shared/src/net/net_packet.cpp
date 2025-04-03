#include "net_packet.h"

#include "string.h"

uint8_t* ME::Packet::GetData() {
    return dataPtr;
}

uint16_t ME::Packet::GetSize() {
    return size;
}

uint16_t ME::Packet::GetIndex() {
    return index;
}

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

void ME::Packet::WriteString(const char* value) {
    uint8_t len = strlen(value);
    char* dest = reinterpret_cast<char*>(dataPtr + index);
    strcpy(dest, value);
    index += (len + 1);
}

void ME::Packet::WriteFP(const ME::Math::FP_24_8& value) {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(dataPtr + index);
    *ptr = value.GetRaw();
    index += 4;
}

void ME::Packet::ReadString(char* ptrString) {
    char* source = reinterpret_cast<char*>(dataPtr + index);
    strcpy(ptrString, source);
    uint8_t len = strlen(ptrString);
    index += (len + 1);
}

ME::Math::FP_24_8 ME::Packet::ReadFP() {
    uint32_t* ptr = reinterpret_cast<uint32_t*>(dataPtr + index);
    index += 4;
    return ME::Math::FP_24_8(*ptr, true);
}

ME::PacketSmall::PacketSmall() {
    index = 0;
    size = ME::PACKET_SIZE_SMALL;
    dataPtr = new uint8_t[size]();
}

ME::PacketSmall::~PacketSmall() {
    delete[] dataPtr;
}

ME::PacketMedium::PacketMedium() {
    index = 0;
    size = ME::PACKET_SIZE_MEDIUM;
    dataPtr = new uint8_t[size]();
}

ME::PacketMedium::~PacketMedium() {
    delete[] dataPtr;
}

ME::PacketBig::PacketBig() {
    index = 0;
    size = ME::PACKET_SIZE_BIG;
    dataPtr = new uint8_t[size]();
}

ME::PacketBig::~PacketBig() {
    delete[] dataPtr;
}

ME::PacketHuge::PacketHuge() {
    index = 0;
    size = ME::PACKET_SIZE_HUGE;
    dataPtr = new uint8_t[size]();
}

ME::PacketHuge::~PacketHuge() {
    delete[] dataPtr;
}
