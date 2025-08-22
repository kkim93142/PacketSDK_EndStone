#pragma once

#include "SDK/Packet/Packet.h"
#include "endstone/server.h"

class ServerToClientHandshakePacket final : public Packet {
public:
    ServerToClientHandshakePacket(const std::string token);
    explicit ServerToClientHandshakePacket(std::string_view packetData);

    ~ServerToClientHandshakePacket() override {};

    std::string token;

    std::string serialize() const override;

public:
    inline static const char* packetName = "ServerToClientHandshakePacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::ServerToClientHandshake;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};