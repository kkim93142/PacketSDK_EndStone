#pragma once
#include "SDK/Packet/Packet.h"
#include "SDK/Packet/PacketIds.h"

class SetTimePacket final : public Packet {
public:
    SetTimePacket(const int32_t time);
    explicit SetTimePacket(std::string_view packetData);

    ~SetTimePacket()() override {};

    ZigZag32 time;

    [[nodiscard]] std::string serialize() const override;

public:
    inline static const char* packetName = "SetTimePacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::SetTime;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};
