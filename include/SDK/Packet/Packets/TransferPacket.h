#pragma once
#include "SDK/Packet/Packet.h"

class TransferPacket final : public Packet {
public:
    ~TransferPacket() {};
    explicit TransferPacket(const std::string &packetData);
    TransferPacket(const std::string &serverAddress, uint16_t port, bool reloadWorld);

    std::string serverAddress;
    uint16_t port;
    bool reloadWorld;

    [[nodiscard]] std::string serialize() const override;

public:
    inline static const char* packetName = "TransferPacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::Transfer;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};