#pragma once

#include "SDK/Packet/Packet.h"

class PlayStatusPacket : public Packet {
public:
    enum class Status : int32_t {
        LoginSuccess,
        OutdatedClient,
        OutdatedServer,
        PlayerSpawn,
        FailedInvalidTenant,
        FailedVanillaEdu,
        FailedEduVanilla,
        FailedServerFull,
        FailedEditorVanillaMismatch,
        FailedVanillaEditorMismatch
    };

public:
    explicit PlayStatusPacket(Status status);
    explicit PlayStatusPacket(std::string_view packetData);

    ~PlayStatusPacket() override {};

    std::string serialize() const override;

    Status status;

public:
    inline static const char* packetName = "PlayStatusPacket";
    static constexpr MinecraftPacketIds packetId = MinecraftPacketIds::PlayStatus;
    static constexpr int packetIdAsInt = static_cast<int>(packetId);
};