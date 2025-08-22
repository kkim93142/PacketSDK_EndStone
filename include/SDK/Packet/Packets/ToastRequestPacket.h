#pragma once
#include "SDK/Packet/Packet.h"

class ToastRequestPacket final : public Packet {
public:
    ToastRequestPacket();
    explicit ToastRequestPacket(std::string_view packetData);

    ~ToastRequestPacket() override {}

    std::string title;
    std::string message;

    [[nodiscard]] std::string serialize() const override;

public:
    inline static const char* packetName = "ToastRequestPacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::ToastRequest;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};